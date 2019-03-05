#ifndef MAT2_H_
#define MAT2_H_
#include <array>
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
    temp.m_00=m_00*_m.m_00 + m_01 * _m.m_10;
    temp.m_01=m_00*_m.m_01 + m_01 * _m.m_11;

    temp.m_10=m_10*_m.m_00 + m_11 * _m.m_10;
    temp.m_11=m_10*_m.m_01 + m_11 * _m.m_11;

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
