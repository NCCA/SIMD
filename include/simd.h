#ifndef SIMD_H_
#define SIMD_H_

#ifdef _MSC_VER
// visual C++ only sets AVX2 flag :(
# ifndef __AVX2__
#  error Set the compile option:  /arch:AVX2   in project settings -> code generation -> enhanced instruction set
# endif
#else
# if !defined(__AVX2__) || !defined(__FMA__) || !defined(__F16C__)
#  error add the following to your compiler flags: -mavx2 -mfma -mf16c
# endif
#endif


#include <immintrin.h>
#include <cstdint>

#if defined(_MSC_VER)
#define ALIGNED(x) __declspec(align(x))
#else
#if defined(__GNUC__)
#define ALIGNED(x) __attribute__ ((aligned(x)))
#endif
#endif

// SSE3/4 types AVX128
using f128=__m128; // float[4]
using i128=__m128i; // int64[2], int32[4], etc
using d128=__m128d; // double[2]
// AVX2 types AVX256
using  f256=__m256; // float[8]
using  i256=__m256i; // int64[4], int32[8], etc
using  d256=__m256d; // double[4]
#ifdef __AVX512F__
  // AVX 512 (may not be supported on all chips)
  using  f512=__m512; // float[16]
  using  i512=__m512i; // int64[8], int32[16], etc
  using  d512=__m512d; // double[8]
#endif
// set operations
inline f128 set4f(const float _a, const float _b, const float _c, const float _d) 
{
  return _mm_setr_ps(_a, _b, _c, _d); 
}

inline f256 set8f(const float _a, const float _b, const float _c, const float _d,
                  const float _e, const float _f, const float _g, const float _h)
{
  return _mm256_setr_ps(_a, _b, _c, _d,_e,_f,_g,_h);
}


inline i128 set4i(const int32_t _a, const int32_t _b, const int32_t _c, const int32_t _d) 
{
  return _mm_setr_epi32(_a, _b, _c, _d); 
}

inline i256 set8i(const int32_t _a, const int32_t _b, const int32_t _c, const int32_t _d,
                  const int32_t _e, const int32_t _f, const int32_t _g, const int32_t _h)
{
  return _mm256_setr_epi32(_a, _b, _c, _d,_e,_f,_g,_h);
}


inline d128 set2d(const double _a, const double _b) 
{
  return _mm_setr_pd(_a, _b); 
}
// splat will set single values
inline f128 splat4f(float f) { return _mm_set1_ps(f); }
inline d128 splat2d(double f) { return _mm_set1_pd(f); }
inline i128 splat4i(int32_t f) { return _mm_set1_epi32(f); }
inline i128 splat2i64(const int64_t f) { return _mm_set1_epi64x(f); }
inline f256 splat8f(const float f) { return _mm256_set1_ps(f); }
inline d256 splat4d(const double f) { return _mm256_set1_pd(f); }
inline i256 splat8i(const int32_t f) { return _mm256_set1_epi32(f); }
inline i256 splat4i64(const int64_t f) { return _mm256_set1_epi64x(f); }


// load operations
inline f128 loadu4f(const void* const ptr) 
{ 
  return _mm_loadu_ps(static_cast<const float*>(ptr));
}

inline f256 loadu8f(const void* const ptr)
{
  return _mm256_loadu_ps(static_cast<const float*>(ptr));
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

inline f256 load8f(const void* const ptr)
{
  return _mm256_load_ps(static_cast<const float*>(ptr));
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
inline f256 zero8f() { return _mm256_setzero_ps(); }
inline i128 zero4i() { return _mm_setzero_si128(); }
inline d128 zero2d() { return _mm_setzero_pd(); }

// store

inline void store4f(void* const ptr, const f128 reg)
{
  _mm_store_ps(static_cast<float *>(ptr), reg);
}

inline void store8f(void* const ptr, const f256 reg)
{
  _mm256_store_ps(static_cast<float *>(ptr), reg);
}
inline void storeu8f(void* const ptr, const f256 reg)
{
  _mm256_storeu_ps(static_cast<float *>(ptr), reg);
}

inline void store4i(void* const ptr, const i128 reg)
{
  _mm_store_si128(static_cast<i128*>(ptr), reg);
}
inline void store2d(void* const ptr, const d128 reg)
{
  _mm_store_pd(static_cast<double*>(ptr), reg);
}

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
inline f256 mul8f(const f256 a, const f256 b) { return _mm256_mul_ps(a, b); }
inline d128 mul2d(const d128 a, const d128 b) { return _mm_mul_pd(a, b); }

inline f128 add4f(const f128 a, const f128 b) { return _mm_add_ps(a, b); }
inline f256 add8f(const f256 a, const f256 b) { return _mm256_add_ps(a, b); }
inline i128 add4i(const i128 a, const i128 b) { return _mm_add_epi32(a, b); }
inline d128 add2d(const d128 a, const d128 b) { return _mm_add_pd(a, b); }
inline i128 add2i64(const i128 a, const i128 b) { return _mm_add_epi64(a, b); }

inline f128 add1f(const f128 a, const f128 b) { return _mm_add_ss(a, b); }
inline d128 add1d(const d128 a, const d128 b) { return _mm_add_sd(a, b); }


inline f128 sub4f(const f128 a, const f128 b) { return _mm_sub_ps(a, b); }
inline f256 sub8f(const f256 a, const f256 b) { return _mm256_sub_ps(a, b); }
inline i128 sub4i(const i128 a, const i128 b) { return _mm_sub_epi32(a, b); }
inline d128 sub2d(const d128 a, const d128 b) { return _mm_sub_pd(a, b); }
inline i128 sub2i64(const i128 a, const i128 b) { return _mm_sub_epi64(a, b); }

// note no integer versions (not implemented)
inline f128 div4f(const f128 a, const f128 b) { return _mm_div_ps(a, b); }
inline d128 div2d(const d128 a, const d128 b) { return _mm_div_pd(a, b); }

inline f256 div8f(const f256 a, const f256 b) { return _mm256_div_ps(a, b); }

inline f128 negate4f(const f128 a)
{
  return _mm_sub_ps(_mm_set1_ps(0.0f), a);
}

inline f256 negate8f(const f256 a)
{
  return _mm256_sub_ps(_mm256_set1_ps(0.0f), a);
}

inline f128 fnmadd4f(const f128 a, const f128 b, const f128 c)
{
  return _mm_fnmadd_ps(a,b,c);
}

inline f128 fmadd4f(const f128 a, const f128 b, const f128 c)
{
  return _mm_fmadd_ps(a,b,c);
}

inline f128 fnmsub4f(const f128 a, const f128 b, const f128 c)
{
  return _mm_fnmsub_ps(a,b,c);
}

inline f128 fmsub4f(const f128 a, const f128 b, const f128 c)
{
  return _mm_fmsub_ps(a,b,c);
}


inline f256 fnmadd8f(const f256 a, const f256 b, const f256 c)
{
  return _mm256_fnmadd_ps(a,b,c);
}

inline f256 fmadd8f(const f256 a, const f256 b, const f256 c)
{
  return _mm256_fmadd_ps(a,b,c);
}

inline f256 fnmsub8f(const f256 a, const f256 b, const f256 c)
{
  return _mm256_fnmsub_ps(a,b,c);
}

inline f256 fmsub8f(const f256 a, const f256 b, const f256 c)
{
  return _mm256_fmsub_ps(a,b,c);
}


// conversion

inline float convertf32(const f128 a)
{
  return _mm_cvtss_f32(a);
}

inline f128 movehdup4f(const f128 a)
{
  return _mm_movehdup_ps(a);
}


inline f128 movehl4f(const f128 a, const f128 b)
{
  return _mm_movehl_ps(a,b);
}

inline f256 halfToFloat(const i128 a)
{
  return _mm256_cvtph_ps(a);
}

enum class Rounding : int
{
  Nearest  = 0b000,
  Down     = 0b001,
  Up       = 0b010,
  Truncate = 0b011,
  ///    1XX: Use MXCSR.RC for rounding
  MXCSRNearest  = 0b100,
  MXCSRDown     = 0b101,
  MXCSRUp       = 0b110,
  MXCSRTruncate = 0b111
};

template<Rounding imm>
inline i128 floatToHalf(const f256 a)
{
  return _mm256_cvtps_ph(a,static_cast<int>(imm));
}


// math
// sqrt lowest register pass rest
inline f128 sqrt1f(const f128 a)
{
  return _mm_sqrt_ss(a);
}

// 1/sqrt lowest register pass rest
inline f128 rsqrt1f(const f128 a)
{
  return _mm_rsqrt_ss(a);
}

inline f128 sqrt4f(const f128 a)
{
  return _mm_sqrt_ps(a);
}

inline f128 rsqrt4f(const f128 a)
{
  return _mm_rsqrt_ps(a);
}

inline f256 sqrt8f(const f256 a)
{
  return _mm256_sqrt_ps(a);
}

inline f128 reciprocal4f(const f128 a)
{
  return _mm_rcp_ps(a);
}

enum class DotMask
{
  None = 0,
  X = 0x1,
  Y = 0x2,
  Z = 0x4,
  W = 0x8,
  XY = X | Y,
  XZ = X | Z,
  XW = X | W,
  YZ = Y | Z,
  YW = Y | W,
  ZW = Z | W,
  XYZ = X | Y | Z,
  XYW = X | Y | W,
  XZW = X | Z | W,
  YZW = Y | Z | W,
  XYZW = X | Y | Z | W
};


template<DotMask inmask, DotMask outmask>
inline f128 dot4f(const f128 a, const f128 b)
{
  constexpr uint8_t mask = static_cast<uint8_t>(outmask) | (static_cast<uint8_t>(inmask) << 4);

  return _mm_dp_ps(a, b, mask);
}

template< int8_t mask>
inline f128 dot4f(const f128 a, const f128 b)
{
  return _mm_dp_ps(a, b, mask);
}

// comparisons
inline i128 cmpeq4i(const i128 a, const i128 b) { return _mm_cmpeq_epi32(a, b); }
inline i128 cmpeq16i8(const i128 a, const i128 b) { return _mm_cmpeq_epi8(a, b); }
inline i128 cmplt16i8(const i128 a, const i128 b) { return _mm_cmplt_epi8(a, b); }
inline i128 cmpgt16i8(const i128 a, const i128 b) { return _mm_cmpgt_epi8(a, b); }

inline f128 cmpeq4f(const f128 a, f128 b){ return _mm_cmpeq_ps(a,b);}
inline f128 cmpeq1f(const f128 a, f128 b){ return _mm_cmpeq_ss(a,b);}
inline f128 cmpneq4f(const f128 a, f128 b){ return _mm_cmpneq_ps(a,b);}
inline f128 cmpneq1f(const f128 a, f128 b){ return _mm_cmpneq_ss(a,b);}


inline f128 cmplt4f(const f128 a, f128 b){ return _mm_cmplt_ps(a,b);}
inline f128 cmplt1f(const f128 a, f128 b){ return _mm_cmplt_ss(a,b);}
inline f128 cmplteq4f(const f128 a, f128 b){ return _mm_cmple_ps(a,b);}
inline f128 cmplteq1f(const f128 a, f128 b){ return _mm_cmple_ss(a,b);}

inline f128 cmpgt4f(const f128 a, f128 b){ return _mm_cmpgt_ps(a,b);}
inline f128 cmpgt1f(const f128 a, f128 b){ return _mm_cmpgt_ss(a,b);}
inline f128 cmpgteq4f(const f128 a, f128 b){ return _mm_cmpge_ps(a,b);}
inline f128 cmpgteq1f(const f128 a, f128 b){ return _mm_cmpge_ss(a,b);}
// todo add in the not versions of above at some stage

inline f256 cmpeq8f(const f256 a, const f256 b){  return _mm256_cmp_ps(a,b,_CMP_EQ_OQ);}
inline f256 cmplt8f(const f256 a, const f256 b){  return _mm256_cmp_ps(a,b,_CMP_LT_OQ);}
inline f256 cmplteq8f(const f256 a, const f256 b){  return _mm256_cmp_ps(a,b,_CMP_LE_OS);}

inline f256 cmpgt8f(const f256 a, const f256 b){  return _mm256_cmp_ps(a,b,_CMP_GT_OQ);}
inline f256 cmpgteq8f(const f256 a, const f256 b){  return _mm256_cmp_ps(a,b,_CMP_GE_OS);}



inline f128 max4f(const f128 a, const f128 b)
{
  return _mm_max_ps(a,b);
}

inline f128 min4f(const f128 a, const f128 b)
{
  return _mm_min_ps(a,b);
}

// boolean
inline f128 and4f(const f128 a, const f128 b) { return _mm_and_ps(a,b); }
inline f128 andnot4f(const f128 a, const f128 b) { return _mm_andnot_ps(a,b); }
inline f128 or4f(const f128 a, const f128 b) { return _mm_or_ps(a,b); }
inline f128 xor4f(const f128 a, const f128 b) { return _mm_xor_ps(a,b); }

inline i128 xor4i(const i128 a, const i128 b) { return _mm_xor_si128(a,b); }


inline f256 and8f(const f256 a, const f256 b) { return _mm256_and_ps(a, b); }
inline f256 andnot8f(const f256 a, const f256 b) { return _mm256_andnot_ps(a, b); }
inline f256 or8f(const f256 a, const f256 b) { return _mm256_or_ps(a, b); }
inline f256 xor8f(const f256 a, const f256 b) { return _mm256_xor_ps(a, b); }
#define shiftBitsLeft4i32(reg, count) _mm_slli_epi32(reg, count)
#define shiftBitsRight4i32(reg, count) _mm_srli_epi32(reg, count)
// packing

// return [a0, b0, a2, b2, a4, b4, a6, b6]
inline f256 unpacklo8f(f256 a, f256 b) { return _mm256_unpacklo_ps(a, b); }
// return [a1, b1, a3, b3, a5, b5, a7, b7]
inline f256 unpackhi8f(f256 a, f256 b) { return _mm256_unpackhi_ps(a, b); }
inline f256 permute8f(f256 in, i256 order){ return _mm256_permutevar8x32_ps(in, order); }
inline f128 unpackhi4f(f128 a, f128 b) { return _mm_unpackhi_ps(a, b); }
inline f128 unpacklo4f(f128 a, f128 b) { return _mm_unpacklo_ps(a, b); }

// using a=set8f(1,2,3,4,5,6,7,8) will return [1,2,3,4]
inline f128 cast8fHi(f256 a)
{
  return   _mm256_castps256_ps128(a);
}

// using a=set8f(1,2,3,4,5,6,7,8) will return [5,6,7,8]
inline f128 cast8fLow(f256 a)
{
  return   _mm256_castps256_ps128(_mm256_permute2f128_ps(a,a,1));
}


#define shuffle4f(a, b, W, Z, Y, X) _mm_shuffle_ps(a, b, _MM_SHUFFLE(W, Z, Y, X))

#define shuffle8f(a, b, W, Z, Y, X) _mm256_shuffle_ps(a, b, _MM_SHUFFLE(W, Z, Y, X))

inline f128 cast4f(const d128 reg) { return _mm_castpd_ps(reg); }
inline f128 cast4f(const i128 reg) { return _mm_castsi128_ps(reg); }
inline i128 cast4i(const d128 reg) { return _mm_castpd_si128(reg); }
inline i128 cast4i(const f128 reg) { return _mm_castps_si128(reg); }
inline d128 cast2d(const f128 reg) { return _mm_castps_pd(reg); }
inline d128 cast2d(const i128 reg) { return _mm_castsi128_pd(reg); }


inline int32_t movemask16i8(const i128 reg) { return _mm_movemask_epi8(reg); }
inline int32_t movemask4i(const i128 reg) { return _mm_movemask_ps(cast4f(reg)); }
inline int32_t movemask4f(const f128 reg) { return _mm_movemask_ps(reg); }
inline int32_t movemask2d(const d128 reg) { return _mm_movemask_pd(reg); }


inline int64_t movemask32i8(const i256 reg) { return _mm256_movemask_epi8(reg); }
inline int64_t movemask8f(const f256 reg) { return _mm256_movemask_ps(reg); }


inline f256 isnegative(const f256 a)
{
  #ifdef __APPLE__
     return _mm256_srai_epi32(a,31);
  #else
  // have to have loads of casts for linux
    return _mm256_castsi256_ps(  _mm256_srai_epi32(_mm256_castps_si256(  a),31));
  #endif
}



#endif
