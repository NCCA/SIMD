// build
// clang++ -std=c++11 -mfma -mSSE4 -m64 -O3 -ffast-math main.cpp -o test

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

void testAndReport4f(const f128 v, std::initializer_list<TestResultF> testResults)
{
  float res[4];
  store4f(res,v);
  std::cout<<"result ["<<res[0]<<' '<<res[1]<<' '<<res[2]<<' '<<res[3]<<"]\n";
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


TEST(SSE4,loadu4f)
{
  float data[]={1.0f, 2.0f, 3.0f, 4.0f};
  f128 a=loadu4f(&data[0]);
  // Note Ordering
  testAndReport4f(a,{{1.0f},{2.0f},{3.0f},{4.0f}});
}

TEST(SSE4,load4f)
{
  float data[]={1.0f, 2.0f, 3.0f, 4.0f};
  f128 a=load4f(&data[0]);
  testAndReport4f(a,{{1.0f},{2.0f},{3.0f},{4.0f}});
}

TEST(SSE4,set4f)
{
  f128 a=set4f(1.0f, 2.0f, 3.0f, 4.0f);
  testAndReport4f(a,{{1.0f},{2.0f},{3.0f},{4.0f}});
}


TEST(SSE4,zero4f)
{
  // _mm_setzero_ps sets all elements to zero
  f128 a=zero4f();
  testAndReport4f(a,{{0.0f},{0.0f},{0.0f},{0.0f}});
}

TEST(SSE4,splat4f)
{
  // _mm_set1_ps sets all elements to value passed (splat!)
  f128 a=splat4f(0.9991f);
  testAndReport4f(a,{{0.9991f},{0.9991f},{0.9991f},{0.9991f}});
}

TEST(SSE4,add4f)
{
  // load data into data type
  f128 a=set4f(5, 6, 7, 8);
  f128 b=set4f(1, 2, 3, 4);
  // execute an add
  f128 res=add4f(a, b);
  testAndReport4f(res,{{6.0f},{8.0f},{10.0f},{12.0f}});
}



TEST(SSE4,sub4f)
{
  f128 a=set4f(5, 6, 7, 8);
  f128 b=set4f(1, 2, 3, 4);
  f128 res=sub4f(a, b);
  testAndReport4f(res,{{4.0f},{4.0f},{4.0f},{4.0f}});
}


TEST(SSE4,mul4f)
{
  f128 a=set4f(1.0f, 2.0f, 3.0f, 4.0f);
  f128 b=splat4f(0.5f);
  f128 res=mul4f(a, b);
  testAndReport4f(res,{{0.5f},{1.0f},{1.5f},{2.0f}});
}


TEST(SSE4,div4f)
{
  f128 a=set4f(1.0f, 2.0f, 3.0f, 4.0f);
  f128 b=splat4f(2.0f);
  f128 res=div4f(a, b);
  testAndReport4f(res,{{0.5f},{1.0f},{1.5f},{2.0f}});
}


TEST(SSE4,divZero)
{
  // test to see what division by zero does
  f128 a=set4f(1.0f, 2.0f, 3.0f, 4.0f);
  f128 b=splat4f(0.0f);
  f128 res=div4f(a, b);
  float r[4];
  store4f(r,res);
  // should set all values to inf
  std::cout<<"result "<<r[0]<<' '<<r[1]<<' '<<r[2]<<' '<<r[3]<<'\n';
  ASSERT_TRUE (isinf(r[0]));
  ASSERT_TRUE (isinf(r[1]));
  ASSERT_TRUE (isinf(r[2]));
  ASSERT_TRUE (isinf(r[3]));
  ASSERT_TRUE(fpclassify(r[0])!=FP_NORMAL);
}

TEST(SSE4,length)
{
  // create a 3 float vector with last component 0
  f128 a=set4f(1.0f, 2.0f, 3.0f, 0.0f);

  float length=convertf32(sqrt1f(dot4f<DotMask::XYZW,DotMask::XYZW>(a, a )));

  std::cout<<"length is "<<length<<'\n';
  EXPECT_NEAR(length,3.7416f,0.001f);
}


TEST(SSE4,dot4f)
{
  // create a 3 float vector with last component 0
  f128 a=set4f(1.0f, 2.0f, 3.0f, 0.0f);

  float length=convertf32(sqrt1f(dot4f<117>(a, a )));

  std::cout<<"length is "<<length<<'\n';
  EXPECT_NEAR(length,3.7416f,0.001f);
}


TEST(SSE4,reciprocal4f)
{
  // create a 4 float vector
  f128 a=set4f(1.0f, 2.0f, 3.0f, 4.0f);
  // evauate 1/a for each vector component
  f128 res = reciprocal4f(a);
  testAndReport4f(res,{{1.0f/1.0f,TestModef::NEAR},
                       {1.0f/2.0f,TestModef::NEAR},
                       {1.0f/3.0f,TestModef::NEAR},
                       {1.0f/4.0f,TestModef::NEAR}});

}

TEST(SSE4,negate)
{
  f128 a=set4f(1.0f, 9.0f, 3.0f, 3.0f);
  f128 res=negate4f(a);
  testAndReport4f(res,{{-1.0f},{-9.0f},{-3.0f},{-3.0f}});
}

TEST(SSE4,fmad4f)
{
  // result = (a * b) + c
  f128 a={1.0f,2.0f,3.0f,4.0f};
  f128 b={0.5f,0.5f,0.5f,0.5f};
  f128 c={2.0f,2.0f,2.0f,2.0f};

  f128 res=fmadd4f(a,b,c);
  testAndReport4f(res,{{2.5f},{3.0f},{3.5f},{4.0f}});
}

TEST(SSE4,fnmad4f)
{
  // result = -(a * b) - c
  f128 a={1.0f,2.0f,3.0f,4.0f};
  f128 b={0.5f,0.5f,0.5f,0.5f};
  f128 c={1.0f,1.0f,1.0f,1.0f};

  f128 res=fnmadd4f(a,b,c);
  testAndReport4f(res,{{0.5f},{0.0f},{-0.5f},{-1.0f}});
}


TEST(SSE4,fmsub4f)
{
  // result = (a * b) - c
  f128 a={1.0f,2.0f,3.0f,4.0f};
  f128 b={0.5f,0.5f,0.5f,0.5f};
  f128 c={2.0f,2.0f,2.0f,2.0f};

  f128 res=fmsub4f(a,b,c);
  testAndReport4f(res,{{-1.5f},{-1.0f},{-0.5f},{0.0f}});
}

TEST(SSE4,fnmsub4f)
{
  // result = -(a * b) - c
  f128 a={1.0f,2.0f,3.0f,4.0f};
  f128 b={0.5f,0.5f,0.5f,0.5f};
  f128 c={1.0f,1.0f,1.0f,1.0f};

  f128 res=fnmsub4f(a,b,c);
  testAndReport4f(res,{{-1.5f},{-2.0f},{-2.5f},{-3.0f}});
}


TEST(SSE4,sqrt4f)
{
  f128 a={100.0f,25.0f,144.0f,2.0f};
  f128 res=sqrt4f(a);
  testAndReport4f(res,{{10.0f},{5.0f},{12.0f},{1.4141f,TestModef::NEAR}});
}

TEST(SSE4,_mm_max_ps)
{
  // create a 4 float vector
  f128 a=set4f(1.0f, 9.0f, 3.0f, 3.0f);
  f128 b=set4f(2.0f, 5.0f, 6.0f, 4.0f);
  f128 res = max4f(a,b);
  testAndReport4f(res,{{2.0f},{9.0f},{6.0f},{4.0f}});
}

TEST(SSE4,_mm_min_ps)
{
  // create a 4 float vector
  f128 a=set4f(1.0f, 9.0f, 3.0f, 3.0f);
  f128 b=set4f(2.0f, 5.0f, 6.0f, 4.0f);
  f128 res=min4f(a,b);
  testAndReport4f(res,{{1.0f},{5.0f},{3.0f},{3.0f}});
}

TEST(SSE4,dot)
{
  // create a 4 float vector
  f128 a=set4f(1.0f, 9.0f, 3.0f, 3.0f);
  f128 b=set4f(2.0f, 5.0f, 6.0f, 4.0f);


  f128 mulRes, shufReg, sumsReg;
  mulRes = mul4f(a, b);
  // Calculates the sum of SSE Register - https://stackoverflow.com/a/35270026/195787
  // duplicates the second and fourth 32-bit data elements
  // if source is { A0, A1, A2, A3 }, the return value is { A1, A1, A3, A3 }.
  shufReg = movehdup4f(mulRes);        // Broadcast elements 3,1 to 2,0
  // add
  sumsReg = add4f(mulRes, shufReg);
  shufReg = movehl4f(shufReg, sumsReg); // High Half -> Low Half
  sumsReg = add1f(sumsReg, shufReg);
  float dot=convertf32(sumsReg); // Result in the lower part of the SSE Register
  std::cout<<"result "<<dot<<'\n';
  ASSERT_FLOAT_EQ(dot,77.0f);

}


TEST(SSE4,and4f)
{
  // result = (a * b) + c
  f128 a={1.0f,0.0f,1.0f,0.0f};
  f128 b={1.0f,0.0f,0.0f,1.0f};
  f128 res=and4f(a,b);
  testAndReport4f(res,{{1.0f},{0},{0},{0}});
}


int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}


