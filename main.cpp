// build
// clang++ -std=c++11 -mfma -mavx2 -m64 -O3 -ffast-math main.cpp -o test

#include <immintrin.h>
#include <iostream>

typedef __m128 f128; // float[4]
typedef __m128i i128; // int64[2], int32[4], etc 
typedef __m128d d128; // double[2]
// AVX256
typedef __m256 f256; // float[8]
typedef __m256i i256; // int64[4], int32[8], etc 
typedef __m256d d256; // double[4]
// AVX512
typedef __m512 f512; // float[16]
typedef __m512i i512; // int64[8], int32[16], etc 
typedef __m512d d512; // double[8]

inline f128 add4f(f128 a, f128 b)
{ 
  return _mm_add_ps(a, b); 
}
inline f256 add8f(f256 a, f256 b)
{ 
  return _mm256_add_ps(a, b); 
}

inline f512 add16f(f512 a, f512 b)
{ 
  return _mm512_add_ps(a, b); 
}

inline f128 set4f(float _a,float _b, float _c, float _d)
{
  return _mm_set_ps(_d, _c, _b, _a);
}


void add()
{
  auto a=_mm_set_ps(5, 6, 7, 8);
  auto b=_mm_set_ps(1, 2, 3, 4);
  auto res=_mm_add_ps(a, b);
  float r[4];
  
  _mm_store_ps(r,res);

  std::cout<<"result "<<r[0]<<' '<<r[1]<<' '<<r[2]<<' '<<r[3]<<'\n';

}


int main()
{
f128 a = set4f(4, 3, 2, 1);
f128 b = set4f(4, 3, 2, 1);

auto res=add4f(a,b);
float r[4];
_mm_store_ps(r,res);

std::cout<<"result "<<r[0]<<' '<<r[1]<<' '<<r[2]<<' '<<r[3]<<'\n';

add();

// std::cout<<"result "<<extract4f(res,0)<<' '<<extract4f(res,1)<<' '
// <<extract4f(res,2)<<' '<<extract4f(res,3)<<'\n';
}


