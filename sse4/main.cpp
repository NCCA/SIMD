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
  alignas(16) float data[]={1.0f, 2.0f, 3.0f, 4.0f};
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

  float length=convertf32(sqrt1f(dot4f<0x75>(a, a )));

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


TEST(SSE4,sqrt1f)
{
  f128 a={100.0f,25.0f,144.0f,2.0f};
  f128 res=sqrt1f(a);
  testAndReport4f(res,{{10.0f},{25.0f},{144.0f},{2.0}});
}

TEST(SSE4,rsqrt1f)
{
  f128 a={100.0f,25.0f,144.0f,2.0f};
  f128 res=rsqrt1f(a);
  testAndReport4f(res,{{1.0f/sqrt(100.0f),TestModef::NEAR},{25.0f},{144.0f},{2.0}});
}


TEST(SSE4,sqrt4f)
{
  f128 a={100.0f,25.0f,144.0f,2.0f};
  f128 res=sqrt4f(a);
  testAndReport4f(res,{{10.0f},{5.0f},{12.0f},{1.4141f,TestModef::NEAR}});
}

TEST(SSE4,rsqrt4f)
{
  f128 a={100.0f,25.0f,144.0f,2.0f};
  f128 res=rsqrt4f(a);
  testAndReport4f(res,{{1.0f/sqrtf(100.0),TestModef::NEAR},
                       {1.0f/sqrtf(25.0f),TestModef::NEAR},
                       {1.0f/sqrtf(144.0f),TestModef::NEAR},
                       {1.0f/sqrtf(2.0f),TestModef::NEAR}});
}


TEST(SSE4,max)
{
  // create a 4 float vector
  f128 a=set4f(1.0f, 9.0f, 3.0f, 3.0f);
  f128 b=set4f(2.0f, 5.0f, 6.0f, 4.0f);
  f128 res = max4f(a,b);
  testAndReport4f(res,{{2.0f},{9.0f},{6.0f},{4.0f}});
}

TEST(SSE4,min)
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
  f128 a=set4f(1.0f,0.0f,1.0f,0.0f);
  f128 b=set4f(1.0f,0.0f,0.0f,1.0f);
  f128 res=and4f(a,b);
  testAndReport4f(res,{{1.0f},{0},{0},{0}});
}

TEST(SSE4,andnot4f)
{
  f128 a=set4f(1.0f,0.0f,1.0f,0.0f);
  f128 b=set4f(1.0f,0.0f,0.0f,1.0f);
  f128 res=andnot4f(a,b);
  testAndReport4f(res,{{0},{0},{0},{1.0f}});
}

TEST(SSE4,or4f)
{
  f128 a=set4f(1.0f,0.0f,1.0f,0.0f);
  f128 b=set4f(1.0f,0.0f,0.0f,1.0f);
  f128 res=or4f(a,b);
  testAndReport4f(res,{{1.0f},{0},{1.0f},{1.0f}});
}

TEST(SSE4,xor4f)
{
  f128 a=set4f(1.0f,0.0f,1.0f,0.0f);
  f128 b=set4f(1.0f,0.0f,0.0f,1.0f);
  f128 res=xor4f(a,b);
  testAndReport4f(res,{{0},{0},{1.0f},{1.0f}});
}

TEST(SSE4,cmpeq4f)
{
  f128 a=set4f(1.0f,5.0f,99.2f,2.2f);
  f128 b=set4f(1.0f,5.0f,99.2f,0.2f);
  f128 res=cmpeq4f(a,b);
  float r[4];
  store4f(r,res);
  // should set all values to inf
  std::cout<<"result "<<r[0]<<' '<<r[1]<<' '<<r[2]<<' '<<r[3]<<'\n';

  ASSERT_TRUE (isnan(r[0]));
  ASSERT_TRUE (isnan(r[1]));
  ASSERT_TRUE (isnan(r[2]));
  ASSERT_FLOAT_EQ(r[3],0.0f);

}

TEST(SSE4,cmpeq1f)
{
  f128 a=set4f(1.0f,5.0f,99.2f,2.2f);
  f128 b=set4f(1.0f,5.0f,99.2f,0.2f);
  f128 res=cmpeq1f(a,b);
  float r[4];
  store4f(r,res);
  // should set all values to inf
  std::cout<<"result "<<r[0]<<' '<<r[1]<<' '<<r[2]<<' '<<r[3]<<'\n';

  ASSERT_TRUE (isnan(r[0]));
  ASSERT_FLOAT_EQ(r[1],5.0f);
  ASSERT_FLOAT_EQ(r[2],99.2f);
  ASSERT_FLOAT_EQ(r[3],2.2f);
}


TEST(SSE4,cmpneq4f)
{
  f128 a=set4f(1.0f,5.0f,99.2f,2.2f);
  f128 b=set4f(1.0f,5.0f,99.2f,0.2f);
  f128 res=cmpneq4f(a,b);
  float r[4];
  store4f(r,res);
  // should set all values to inf
  std::cout<<"result "<<r[0]<<' '<<r[1]<<' '<<r[2]<<' '<<r[3]<<'\n';

  ASSERT_FLOAT_EQ(r[0],0.0f);
  ASSERT_FLOAT_EQ(r[1],0.0f);
  ASSERT_FLOAT_EQ(r[2],0.0f);
  ASSERT_TRUE (isnan(r[3]));

}

TEST(SSE4,cmpneq1f)
{
  f128 a=set4f(0.0f,5.0f,99.2f,2.2f);
  f128 b=set4f(1.0f,5.0f,99.2f,0.2f);
  f128 res=cmpneq1f(a,b);
  float r[4];
  store4f(r,res);
  // should set all values to inf
  std::cout<<"result "<<r[0]<<' '<<r[1]<<' '<<r[2]<<' '<<r[3]<<'\n';

  ASSERT_TRUE (isnan(r[0]));
  ASSERT_FLOAT_EQ(r[1],5.0f);
  ASSERT_FLOAT_EQ(r[2],99.2f);
  ASSERT_FLOAT_EQ(r[3],2.2f);
}


TEST(SSE4,cmplt4f)
{
  f128 a=set4f(1.0f,5.0f,99.2f,2.2f);
  f128 b=set4f(2.0f,5.0f,99.2f,4.2f);
  f128 res=cmplt4f(a,b);
  float r[4];
  store4f(r,res);
  // should set all values to inf
  std::cout<<"result "<<r[0]<<' '<<r[1]<<' '<<r[2]<<' '<<r[3]<<'\n';

  ASSERT_TRUE (isnan(r[0]));
  ASSERT_FLOAT_EQ(r[1],0);
  ASSERT_FLOAT_EQ(r[2],0);
  ASSERT_TRUE (isnan(r[3]));

}

TEST(SSE4,cmplt1f)
{
  f128 a=set4f(0.01f,5.0f,99.2f,0.2f);
  f128 b=set4f(1.0f,5.0f,99.2f,1.2f);
  f128 res=cmplt1f(a,b);
  float r[4];
  store4f(r,res);
  // should set all values to inf
  std::cout<<"result "<<r[0]<<' '<<r[1]<<' '<<r[2]<<' '<<r[3]<<'\n';

  ASSERT_TRUE (isnan(r[0]));
  ASSERT_FLOAT_EQ(r[1],5.0f);
  ASSERT_FLOAT_EQ(r[2],99.2f);
  ASSERT_FLOAT_EQ(r[3],0.2f);
}

TEST(SSE4,cmplteq4f)
{
  f128 a=set4f(1.0f,5.0f,99.2f,2.2f);
  f128 b=set4f(2.0f,5.0f,99.2f,4.2f);
  f128 res=cmplteq4f(a,b);
  float r[4];
  store4f(r,res);
  // should set all values to inf
  std::cout<<"result "<<r[0]<<' '<<r[1]<<' '<<r[2]<<' '<<r[3]<<'\n';

  ASSERT_TRUE (isnan(r[0]));
  ASSERT_TRUE (isnan(r[1]));
  ASSERT_TRUE (isnan(r[2]));
  ASSERT_TRUE (isnan(r[3]));

}

TEST(SSE4,cmplteq1f)
{
  f128 a=set4f(0.01f,5.0f,99.2f,0.2f);
  f128 b=set4f(1.0f,5.0f,99.2f,1.2f);
  f128 res=cmplteq1f(a,b);
  float r[4];
  store4f(r,res);
  // should set all values to inf
  std::cout<<"result "<<r[0]<<' '<<r[1]<<' '<<r[2]<<' '<<r[3]<<'\n';

  ASSERT_TRUE (isnan(r[0]));
  ASSERT_FLOAT_EQ(r[1],5.0f);
  ASSERT_FLOAT_EQ(r[2],99.2f);
  ASSERT_FLOAT_EQ(r[3],0.2f);
}

TEST(SSE4,cmpgt4f)
{
  f128 a=set4f(2.0f,5.0f,99.2f,9.2f);
  f128 b=set4f(1.0f,5.0f,99.2f,4.2f);
  f128 res=cmpgt4f(a,b);
  float r[4];
  store4f(r,res);
  // should set all values to inf
  std::cout<<"result "<<r[0]<<' '<<r[1]<<' '<<r[2]<<' '<<r[3]<<'\n';

  ASSERT_TRUE (isnan(r[0]));
  ASSERT_FLOAT_EQ(r[1],0);
  ASSERT_FLOAT_EQ(r[2],0);
  ASSERT_TRUE (isnan(r[3]));

}

TEST(SSE4,cmpgt1f)
{
  f128 a=set4f(8.01f,5.0f,99.2f,0.2f);
  f128 b=set4f(1.0f,5.0f,99.2f,1.2f);
  f128 res=cmpgt1f(a,b);
  float r[4];
  store4f(r,res);
  // should set all values to inf
  std::cout<<"result "<<r[0]<<' '<<r[1]<<' '<<r[2]<<' '<<r[3]<<'\n';

  ASSERT_TRUE (isnan(r[0]));
  ASSERT_FLOAT_EQ(r[1],5.0f);
  ASSERT_FLOAT_EQ(r[2],99.2f);
  ASSERT_FLOAT_EQ(r[3],0.2f);
}

TEST(SSE4,cmpgteq4f)
{
  f128 a=set4f(1.0f,5.0f,99.2f,2.2f);
  f128 b=set4f(2.0f,5.0f,99.2f,4.2f);
  f128 res=cmpgteq4f(a,b);
  float r[4];
  store4f(r,res);
  // should set all values to inf
  std::cout<<"result "<<r[0]<<' '<<r[1]<<' '<<r[2]<<' '<<r[3]<<'\n';

  ASSERT_FLOAT_EQ(r[0],0);
  ASSERT_TRUE (isnan(r[1]));
  ASSERT_TRUE (isnan(r[2]));
  ASSERT_FLOAT_EQ(r[3],0);

}

TEST(SSE4,cmpgteq1f)
{
  f128 a=set4f(8.01f,5.0f,99.2f,0.2f);
  f128 b=set4f(1.0f,5.0f,99.2f,1.2f);
  f128 res=cmpgteq1f(a,b);
  float r[4];
  store4f(r,res);
  // should set all values to inf
  std::cout<<"result "<<r[0]<<' '<<r[1]<<' '<<r[2]<<' '<<r[3]<<'\n';

  ASSERT_TRUE (isnan(r[0]));
  ASSERT_FLOAT_EQ(r[1],5.0f);
  ASSERT_FLOAT_EQ(r[2],99.2f);
  ASSERT_FLOAT_EQ(r[3],0.2f);
}

static i128 s_c=set4i(123456789,123456790,123456791,123456792);

TEST(SSE4,StaticInit)
{


 i128 a=set4i(123456789,(123456789+1),(123456789+2),(123456789+3));
 int ra[4];
 store4i(ra,a);
 std::cout<<"result "<<ra[0]<<' '<<ra[1]<<' '<<ra[2]<<' '<<ra[3]<<'\n';
 union i4{
     i128 simd;
     int32_t b[4];
     long long c;
 };

 i4 b;
 int rb[4];
 b.b[0]=123456789;
 b.b[1]=123456790;
 b.b[2]=123456791;
 b.b[3]=123456792;
 std::cout<<b.c<<'\n';
 store4i(rb,b.simd);
 std::cout<<"result "<<rb[0]<<' '<<rb[1]<<' '<<rb[2]<<' '<<rb[3]<<'\n';

 ASSERT_EQ(ra[0],rb[0]);
 ASSERT_EQ(ra[1],rb[1]);
 ASSERT_EQ(ra[2],rb[2]);
 ASSERT_EQ(ra[3],rb[3]);

 int rc[4];
 store4i(rc,s_c);
 ASSERT_EQ(rc[0],rb[0]);
 ASSERT_EQ(rc[1],rb[1]);
 ASSERT_EQ(rc[2],rb[2]);
 ASSERT_EQ(rc[3],rb[3]);



}


int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}


