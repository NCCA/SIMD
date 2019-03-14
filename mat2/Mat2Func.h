#ifndef MAT2_H_
#define MAT2_H_
#include <array>
#include <immintrin.h>

constexpr float  EPSILON = 0.001f;

#define FCompare(a,b) \
    ( ((a)-EPSILON)<(b) && ((a)+EPSILON)>(b) )

struct Mat2
{
  Mat2()=default;
  Mat2(float _a, float _b, float _c, float _d)
  {
    m[0][0]=_a;
    m[0][1]=_b;
    m[1][0]=_c;
    m[1][1]=_d;
  }
  bool operator ==(const Mat2 &_b)
  {
    return
        FCompare(m[0][0] , _b.m[0][0]) &&
        FCompare(m[0][1] , _b.m[0][1]) &&
        FCompare(m[1][0] , _b.m[1][0]) &&
        FCompare(m[1][1] , _b.m[1][1]) ;

  }


  Mat2 operator*( const Mat2 &_m  ) const noexcept
  {
    Mat2 temp;
    // With pre-fetch
    float b00 = m[0][0], b01 = m[0][1];
    float b10 = m[1][0], b11 = m[1][1];
    float c00 = _m.m[0][0], c01 = _m.m[0][1];
    float c10 = _m.m[1][0], c11 = _m.m[1][1];
    temp.m[0][0] = b00*c00 + b01*c10;
    temp.m[0][1] = b00*c01 + b01*c11;
    temp.m[1][0] = b10*c00 + b11*c10;
    temp.m[1][1] = b10*c01 + b11*c11;

    // SIMD
//    __m128 b=_mm_setr_ps(m_00,m_00,m_10,m_10);
//    __m128 c=_mm_setr_ps(_m.m_00,_m.m_01,_m.m_00,_m.m_01);
//    __m128 d=_mm_setr_ps(m_01,m_01,m_11,m_11);
//    __m128 e=_mm_setr_ps(_m.m_10,_m.m_11,_m.m_10,_m.m_11);
//    // first compute d*e
//    __m128 f=_mm_mul_ps(d,e);
//    // fma  result = (a * b) + c
//    __m128 a=_mm_fmadd_ps(b,c,f);
//    _mm_storeu_ps(static_cast<float *>(&temp.array[0]), a);


    // without
//    temp.m_00=m_00*_m.m_00 + m_01 * _m.m_10;
//    temp.m_01=m_00*_m.m_01 + m_01 * _m.m_11;

//    temp.m_10=m_10*_m.m_00 + m_11 * _m.m_10;
//    temp.m_11=m_10*_m.m_01 + m_11 * _m.m_11;

    return temp;
  }


  union
  {
    float m[2][2];
    std::array<float,4> array={{1.0f,0.0f,
                                0.0f,1.0f}};
    struct
    {
      float m_00;
      float m_01;
      float m_10;
      float m_11;
    };
  };
};


extern void MultMat2Normal(Mat2 &o_a, const Mat2 &_b, const Mat2 &_c) noexcept;
extern void MultMat2Unroll(Mat2 &o_a, const Mat2 &_b, const Mat2 &_c) noexcept;
extern void MultMat2Prefetch(Mat2 &o_a, const Mat2 &_b, const Mat2 &_c) noexcept;
extern void MultMat2Operator(Mat2 &o_a, const Mat2 &_b, const Mat2 &_c) noexcept;
extern void MultMat2SIMD(Mat2 &o_a, const Mat2 &_b, const Mat2 &_c) noexcept;
extern void MultMat2AVX2(Mat2 &o_a, const Mat2 &_b, const Mat2 &_c) noexcept;




#endif
