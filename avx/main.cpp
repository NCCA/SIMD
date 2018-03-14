#include "simd.h"
#include <iostream>
#include <cmath>
#include <gtest/gtest.h>

enum class TestModef  {FLOATEQ,NEAR};
struct TestResultF
{
  TestResultF(float _r) : expected(_r){}
  TestResultF(float _r, TestModef _mode) : expected(_r),mode(_mode){}
  float expected;
  TestModef mode=TestModef::FLOATEQ;
};

void testAndReport8f(const f256 v, std::initializer_list<TestResultF> testResults)
{
  float res[8];
  store8f(res,v);
  std::cout<<"result ["<<res[0]<<' '<<res[1]<<' '<<res[2]<<' '<<res[3]<<
                   ' '<<res[4]<<' '<<res[5]<<' '<<res[6]<<' '<<res[7]<<"]\n";
  size_t index=0;
  for(auto t : testResults)
  {
    switch (t.mode)
    {
      case TestModef::FLOATEQ :
        ASSERT_FLOAT_EQ(v[index++],t.expected);
      break;
      case TestModef::NEAR :
        ASSERT_NEAR(v[index++],t.expected,0.001f);
      break;
    }
  }

}


TEST(AVX,set8f)
{
  f256 a=set8f(1.0f, 2.0f, 3.0f, 4.0f,5.0f,6.0f,7.0f,8.0f);
  testAndReport8f(a,{{1.0f},{2.0f},{3.0f},{4.0f},{5.0f},{6.0f},{7.0f},{8.0f}});

}


TEST(AVX,add8f)
{
  f256 a=set8f(1.0f, 2.0f, 3.0f, 4.0f,5.0f,6.0f,7.0f,8.0f);
  f256 b=set8f(1.0f, 2.0f, 3.0f, 4.0f,5.0f,6.0f,7.0f,8.0f);
  f256 r=add8f(a,b);

  testAndReport8f(r,{{2.0f},{4.0f},{6.0f},{8.0f},
                     {10.0f},{12.0f},{14.0f},{16.0f}});
}

TEST(AVX,sub8f)
{
  f256 a=set8f(1.0f, 2.0f, 3.0f, 4.0f,5.0f,6.0f,7.0f,8.0f);
  f256 b=set8f(1.0f, 2.0f, 3.0f, 4.0f,5.0f,6.0f,7.0f,8.0f);
  f256 r=sub8f(a,b);
  testAndReport8f(r,{{0.0f},{0.0f},{0.0f},{0.0f},{0.0f},{0.0f},{0.0f},{0.0f}});
}

TEST(AVX,mul8f)
{
  f256 a=set8f(1.0f, 2.0f, 3.0f, 4.0f,5.0f,6.0f,7.0f,8.0f);
  f256 b=splat8f(0.5f);
  f256 r=mul8f(a,b);
  testAndReport8f(r,{{0.5f},{1.0f},{1.5f},{2.0f},
                     {2.5f},{3.0f},{3.5f},{4.0f}});

}


TEST(AVX,negate8f)
{
  f256 a=set8f(1.0f, 2.0f, 3.0f, 4.0f,5.0f,6.0f,7.0f,8.0f);

  f256 r=negate8f(a);
  testAndReport8f(r,{{-1.0f},{-2.0f},{-3.0f},{-4.0f},
                     {-5.0f},{-6.0f},{-7.0f},{-8.0f}});
}


TEST(AVX,fmadd8f)
{
  f256 a={1.0f,2.0f,3.0f,4.0f,5.0,6.0,7.0,8.0};
  f256 b=splat8f(0.5f);
  f256 c=splat8f(2.0f);
  f256 r=fmadd8f(a,b,c);
  testAndReport8f(r,{{2.5f},{3.0f},{3.5f},{4.0f},
                     {4.5f},{5.0f},{5.5f},{6.0f}});
}


TEST(AVX,fmnadd8f)
{
  f256 a={1.0f,2.0f,3.0f,4.0f,5.0,6.0,7.0,8.0};
  f256 b=splat8f(0.5f);
  f256 c=splat8f(1.0f);
  f256 r=fnmadd8f(a,b,c);
  testAndReport8f(r,{{0.5f},{0.0f},{-0.5f},{-1.0f},
                     {-1.5f},{-2.0f},{-2.5f},{-3.0f}});
}

TEST(AVX,sqrt8f)
{
  f256 a=set8f(100.0f,25.0f,144.0f,2.0f,100.0f,25.0f,144.0f,2.0f);
  f256 r=sqrt8f(a);
  testAndReport8f(r,{{10.0f},{5.0f},{12.0f},{1.4141f,TestModef::NEAR},
                  {10.0f},{5.0f},{12.0f},{1.4141f,TestModef::NEAR}});

}

TEST(AVX,comp8f)
{
  f256 a=set8f(1.0f, 2.0f, 3.0f, 4.0f,5.0f,6.0f,7.0f,8.0f);
  f256 b=set8f(1.0f, 0.0f, 3.0f, 0.0f,5.0f,0.0f,7.0f,0.0f);
  f256 res=cmpeq8f(a,b);
  float r[8];
  store8f(r,res);
  std::cout<<"result ["<<r[0]<<' '<<r[1]<<' '<<r[2]<<' '<<r[3]<<
                   ' '<<r[4]<<' '<<r[5]<<' '<<r[6]<<' '<<r[7]<<"]\n";

  ASSERT_TRUE(std::isnan(r[0]));
  ASSERT_FLOAT_EQ(0.0,r[1]);
  ASSERT_TRUE(std::isnan(r[2]));
  ASSERT_FLOAT_EQ(0.0,r[3]);
  ASSERT_TRUE(std::isnan(r[4]));
  ASSERT_FLOAT_EQ(0.0,r[5]);
  ASSERT_TRUE(std::isnan(r[6]));
  ASSERT_FLOAT_EQ(0.0,r[7]);
}

TEST(AVX,isnegative)
{
  f256 a=set8f(-1.0f, -2.0f, -3.0f, -4.0f,-5.0f,-6.0f,-7.0f,-8.0f);
  f256 b=set8f(1.0f, 2.0f, 3.0f, 4.0f,5.0f,6.0f,7.0f,8.0f);
  f256 res=isnegative(a);
  float r[8];
  store8f(r,res);
  std::cout<<"result ["<<r[0]<<' '<<r[1]<<' '<<r[2]<<' '<<r[3]<<
                   ' '<<r[4]<<' '<<r[5]<<' '<<r[6]<<' '<<r[7]<<"]\n";

  for(size_t i=0; i<7; ++i)
    ASSERT_TRUE(std::isnan(r[i]));

  res=isnegative(b);
  testAndReport8f(res,{{0},{0},{0},{0},
                  {0},{0},{0},{0}});

}

TEST(AVX,isnegativeMix)
{
  f256 a=set8f(-1.0f, 2.0f, -0.0f, 0.0f,-5.0f,-2.03f,-0.0002f,8.0f);
  f256 res=isnegative(a);
  float r[8];
  store8f(r,res);
  std::cout<<"result ["<<r[0]<<' '<<r[1]<<' '<<r[2]<<' '<<r[3]<<
                   ' '<<r[4]<<' '<<r[5]<<' '<<r[6]<<' '<<r[7]<<"]\n";


  ASSERT_TRUE(std::isnan(r[0]));
  ASSERT_FLOAT_EQ(r[1],0.0f);
  ASSERT_TRUE(std::isnan(r[2]));
  ASSERT_FLOAT_EQ(r[3],0.0f);
  ASSERT_TRUE(std::isnan(r[4]));
  ASSERT_TRUE(std::isnan(r[5]));
  ASSERT_TRUE(std::isnan(r[6]));
  ASSERT_FLOAT_EQ(r[7],0.0f);
}

TEST(AVX,Half)
{
  f256 a=set8f(1.0f, 2.0f, 3.0f, 4.0f,5.0f,6.0f,7.0f,8.0f);
  i128 b=floatToHalf<Rounding::Nearest>(a);

  f256 c=halfToFloat(b);
  testAndReport8f(c,{{1.0f},{2.0f},{3.0f},{4.0f},
                     {5.0f},{6.0f},{7.0f},{8.0f}});

}

int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
