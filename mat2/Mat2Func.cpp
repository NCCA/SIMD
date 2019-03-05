#include "Mat2Func.h"
#include <immintrin.h>
#include <iostream>

void MultMat2Normal(Mat2 &o_a, const Mat2 &_b, const Mat2 &_c) noexcept
{
  for (int i=0;i<2; ++i)
  {
    for (int j=0; j<2; ++j)
    {
      o_a.m[i][j]=0.0f;
      for(int k=0; k<2; ++k)
      {
        o_a.m[i][j]+=_b.m[i][k] * _c.m[k][j];
      }
    }
  }
}

void MultMat2Unroll(Mat2 &o_a, const Mat2 &_b, const Mat2 &_c) noexcept
{
  o_a.m[0][0] = _b.m[0][0]*_c.m[0][0] + _b.m[0][1]*_c.m[1][0];
  o_a.m[0][1] = _b.m[0][0]*_c.m[0][1] + _b.m[0][1]*_c.m[1][1]; //(1)
  o_a.m[1][0] = _b.m[1][0]*_c.m[0][0] + _b.m[1][1]*_c.m[1][0]; //(2)
  o_a.m[1][1] = _b.m[1][0]*_c.m[0][1] + _b.m[1][1]*_c.m[1][1]; //(3)

}


void MultMat2Prefetch(Mat2 &o_a, const Mat2 &_b, const Mat2 &_c) noexcept
{
  float b00 = _b.m[0][0], b01 = _b.m[0][1];
  float b10 = _b.m[1][0], b11 = _b.m[1][1];
  float c00 = _c.m[0][0], c01 = _c.m[0][1];
  float c10 = _c.m[1][0], c11 = _c.m[1][1];
  o_a.m[0][0] = b00*c00 + b01*c10;
  o_a.m[0][1] = b00*c01 + b01*c11;
  o_a.m[1][0] = b10*c00 + b11*c10;
  o_a.m[1][1] = b10*c01 + b11*c11;
}

void MultMat2Operator(Mat2 &o_a, const Mat2 &_b, const Mat2 &_c) noexcept
{
  o_a=_b*_c;
}

void MultMat2SIMD(Mat2 &o_a, const Mat2 &_b, const Mat2 &_c) noexcept
{

  __m128 a;
  //                 b    c    d    e
  //  o_a.m[0][0] = b00*c00 + b01*c10;
  //  o_a.m[0][1] = b00*c01 + b01*c11;
  //  o_a.m[1][0] = b10*c00 + b11*c10;
  //  o_a.m[1][1] = b10*c01 + b11*c11;

  __m128 b=_mm_setr_ps(_b.m_00,_b.m_00,_b.m_10,_b.m_10);
  __m128 c=_mm_setr_ps(_c.m_00,_c.m_01,_c.m_00,_c.m_01);
  __m128 d=_mm_setr_ps(_b.m_01,_b.m_01,_b.m_11,_b.m_11);
  __m128 e=_mm_setr_ps(_c.m_10,_c.m_11,_c.m_10,_c.m_11);

  // first compute d*e
  __m128 f=_mm_mul_ps(d,e);
  // fma  result = (a * b) + c
  a=_mm_fmadd_ps(b,c,f);
  _mm_storeu_ps(static_cast<float *>(&o_a.array[0]), a);

}

void MultMat2AVX2(Mat2 &o_a, const Mat2 &_b, const Mat2 &_c) noexcept
{

  //                 b    c    d    e
  //  o_a.m[0][0] = b00*c00 + b01*c10;
  //  o_a.m[0][1] = b00*c01 + b01*c11;
  //  o_a.m[1][0] = b10*c00 + b11*c10;
  //  o_a.m[1][1] = b10*c01 + b11*c11;

  // first order the data so we get it in the format above
  __m256 b=_mm256_setr_ps(_b.m_00,_b.m_01,_b.m_00,_b.m_01,
                          _b.m_10,_b.m_11,_b.m_10,_b.m_11);

  __m256 c=_mm256_setr_ps(_c.m_00,_c.m_10,_c.m_01,_c.m_11,
                          _c.m_00,_c.m_10,_c.m_01,_c.m_11);
  // now we multiply the elements
  __m256 mul=_mm256_mul_ps(b,c);
  // horizontal add will add [0] + [1] [2] + [3]
  auto res=_mm256_hadd_ps(mul,mul); //bottleneck!
  __m256i mask=_mm256_setr_epi32(0,1,4,5,0,1,4,5);
  res=_mm256_permutevar8x32_ps(res,mask);
  // copy out.
  auto a=_mm256_castps256_ps128(res);
  _mm_storeu_ps(static_cast<float *>(&o_a.array[0]), a);

}

