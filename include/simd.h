#ifndef SIMD_H_
#define SIMD_H_

#include <immintrin.h>

// SSE3/4 types AVX128
using f128=__m128; // float[4]
using i128=__m128i; // int64[2], int32[4], etc
using d128=__m128d; // double[2]
// AVX2 types AVX256
using  f256=__m256; // float[8]
using  i256=__m256i; // int64[4], int32[8], etc
using  d256=__m256d; // double[4]
// AVX 512 (may not be supported on all chips)
using  f512=__m512; // float[16]
using  i512=__m512i; // int64[8], int32[16], etc 
using  d512=__m512d; // double[8]

// set operations
inline f128 set4f(const float _a, const float _b, const float _c, const float _d) 
{
  return _mm_setr_ps(_a, _b, _c, _d); 
}

inline i128 set4i(const int32_t _a, const int32_t _b, const int32_t _c, const int32_t _d) 
{
  return _mm_setr_epi32(_a, _b, _c, _d); 
}
 
inline d128 set2d(const double _a, const double _b) 
{
  return _mm_setr_pd(_a, _b); 
}
// spat will set single values
inline f128 splat4f(float f) { return _mm_set1_ps(f); }
inline d128 splat2d(double f) { return _mm_set1_pd(f); }
inline i128 splat4i(int32_t f) { return _mm_set1_epi32(f); }
inline i128 splat2i64(const int64_t f) { return _mm_set1_epi64x(f); }


// load operations
inline f128 loadu4f(const void* const ptr) 
{ 
  return _mm_loadu_ps(static_cast<const float*>(ptr));
}
inline i128 loadu4i(const void* const ptr) 
{ 
  return _mm_loadu_si128(static_cast<const i128*>(ptr));
}
inline d128 loadu2d(const void* const ptr) 
{ 
  return _mm_loadu_pd(static_cast<const double*>(ptr));
}

inline f128 load4f(const void* const ptr) 
{ 
  return _mm_load_ps(static_cast<const float*>(ptr));
}
inline i128 load4i(const void* const ptr) 
{ 
  return _mm_load_si128(static_cast<const i128*>(ptr));
}
inline d128 load2d(const void* const ptr) 
{ 
  return _mm_load_pd(static_cast<const double*>(ptr));
}


// zero

inline f128 zero4f() { return _mm_setzero_ps(); }
inline i128 zero4i() { return _mm_setzero_si128(); }
inline d128 zero2d() { return _mm_setzero_pd(); }

// store

inline void storeu4f(void* const ptr, const f128 reg) 
{ 
  _mm_storeu_ps(static_cast<float *>(ptr), reg);
}
inline void storeu4i(void* const ptr, const i128 reg) 
{ 
  _mm_storeu_si128(static_cast<i128*>(ptr), reg);
}

inline void storeu2d(void* const ptr, const d128 reg) 
{ 
  _mm_storeu_pd(static_cast<double *>(ptr), reg);
}


// arithmetic
inline f128 mul4f(const f128 a, const f128 b) { return _mm_mul_ps(a, b); }
inline d128 mul2d(const d128 a, const d128 b) { return _mm_mul_pd(a, b); }

inline f128 add4f(const f128 a, const f128 b) { return _mm_add_ps(a, b); }
inline i128 add4i(const i128 a, const i128 b) { return _mm_add_epi32(a, b); }
inline d128 add2d(const d128 a, const d128 b) { return _mm_add_pd(a, b); }
inline i128 add2i64(const i128 a, const i128 b) { return _mm_add_epi64(a, b); }

inline f128 sub4f(const f128 a, const f128 b) { return _mm_sub_ps(a, b); }
inline i128 sub4i(const i128 a, const i128 b) { return _mm_sub_epi32(a, b); }
inline d128 sub2d(const d128 a, const d128 b) { return _mm_sub_pd(a, b); }
inline i128 sub2i64(const i128 a, const i128 b) { return _mm_sub_epi64(a, b); }

// note no integer versions (not implemented)
inline f128 div4f(const f128 a, const f128 b) { return _mm_div_ps(a, b); }
inline d128 div2d(const d128 a, const d128 b) { return _mm_div_pd(a, b); }


#endif
