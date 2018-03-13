// build
// clang++ -std=c++11 -mfma -mSSE4 -m64 -O3 -ffast-math main.cpp -o test

#include "simd.h"
#include <iostream>
#include <cmath>
#include <gtest/gtest.h>


TEST(SSE4,_mm_loadu_ps)
{
  float data[]={1.0f, 2.0f, 3.0f, 4.0f};
  __m128 a=loadu4f(&data[0]);
  printf("result %f %f %f %f \n",a[0],a[1],a[2],a[3]);
  // Note Ordering
  ASSERT_FLOAT_EQ((a[0]),1.0f);
  ASSERT_FLOAT_EQ(a[1],2.0f);
  ASSERT_FLOAT_EQ(a[2],3.0f);
  ASSERT_FLOAT_EQ(a[3],4.0f);

}

TEST(SSE4,_mm_load_ps)
{
  float data[]={1.0f, 2.0f, 3.0f, 4.0f};
  __m128 a=load4f(&data[0]);
  printf("result %f %f %f %f \n",a[0],a[1],a[2],a[3]);
  // Note Ordering
  ASSERT_FLOAT_EQ((a[0]),1.0f);
  ASSERT_FLOAT_EQ(a[1],2.0f);
  ASSERT_FLOAT_EQ(a[2],3.0f);
  ASSERT_FLOAT_EQ(a[3],4.0f);

}

TEST(SSE4,set4f)
{
  __m128 a=set4f(1.0f, 2.0f, 3.0f, 4.0f);
  // we can access vector elements directly (not recommneded due to alignment issues)
  printf("result %f %f %f %f \n",a[0],a[1],a[2],a[3]);
  // Note Ordering
  ASSERT_FLOAT_EQ((a[0]),1.0f);
  ASSERT_FLOAT_EQ(a[1],2.0f);
  ASSERT_FLOAT_EQ(a[2],3.0f);
  ASSERT_FLOAT_EQ(a[3],4.0f);
}

TEST(SSE4,set4fCast)
{
  __m128 a=set4f(1.0f, 2.0f, 3.0f, 4.0f);
  // note we can access the data by re-casting to float
  // again best not too
  float *r = reinterpret_cast<float*>(&a);

  std::cout<<"result "<<r[0]<<' '<<r[1]<<' '<<r[2]<<' '<<r[3]<<'\n';
  // Note order of data (due to the (r)everse function )
  ASSERT_FLOAT_EQ(r[0],1.0f);
  ASSERT_FLOAT_EQ(r[1],2.0f);
  ASSERT_FLOAT_EQ(r[2],3.0f);
  ASSERT_FLOAT_EQ(r[3],4.0f);

}


TEST(SSE4,zero4f)
{
  // _mm_setzero_ps sets all elements to zero
  __m128 a=zero4f();
  // get result by using the storeu_ps is prefered
  float r[4];
  storeu4f(r,a);
  std::cout<<"result "<<r[0]<<' '<<r[1]<<' '<<r[2]<<' '<<r[3]<<'\n';
  ASSERT_FLOAT_EQ(r[0],0.0f);
  ASSERT_FLOAT_EQ(r[1],0.0f);
  ASSERT_FLOAT_EQ(r[2],0.0f);
  ASSERT_FLOAT_EQ(r[3],0.0f);
}

TEST(SSE4,splat4f)
{
  // _mm_set1_ps sets all elements to value passed (splat!)
  __m128 a=splat4f(0.9991f);
  // get result by using the storeu_ps is prefered
  float r[4];
  storeu4f(r,a);
  std::cout<<"result "<<r[0]<<' '<<r[1]<<' '<<r[2]<<' '<<r[3]<<'\n';

  ASSERT_FLOAT_EQ(r[0],0.9991f);
  ASSERT_FLOAT_EQ(r[1],0.9991f);
  ASSERT_FLOAT_EQ(r[2],0.9991f);
  ASSERT_FLOAT_EQ(r[3],0.9991f);

}





TEST(SSE4,add4f)
{
  // load data into data type
  __m128 a=set4f(5, 6, 7, 8);
  __m128 b=set4f(1, 2, 3, 4);
  // execute an add
  __m128 res=add4f(a, b);
  // get result by using
  float r[4];  
  storeu4f(r,res);

  std::cout<<"add result "<<r[0]<<' '<<r[1]<<' '<<r[2]<<' '<<r[3]<<'\n';
  ASSERT_FLOAT_EQ(r[0],6);
  ASSERT_FLOAT_EQ(r[1],8);
  ASSERT_FLOAT_EQ(r[2],10);
  ASSERT_FLOAT_EQ(r[3],12);
}



TEST(SSE4,sub4f)
{
  __m128 a=set4f(5, 6, 7, 8);
  __m128 b=set4f(1, 2, 3, 4);
  __m128 res=sub4f(a, b);
  float r[4];  
  _mm_store_ps(r,res);

  std::cout<<"sub result "<<r[0]<<' '<<r[1]<<' '<<r[2]<<' '<<r[3]<<'\n';
  ASSERT_FLOAT_EQ(r[0],4);
  ASSERT_FLOAT_EQ(r[1],4);
  ASSERT_FLOAT_EQ(r[2],4);
  ASSERT_FLOAT_EQ(r[3],4);

}




TEST(SSE4,mul4f)
{
  __m128 a=set4f(1.0f, 2.0f, 3.0f, 4.0f);
  __m128 b=splat4f(0.5f);
  __m128 res=mul4f(a, b);
  float r[4];
  _mm_store_ps(r,res);

  std::cout<<"result "<<r[0]<<' '<<r[1]<<' '<<r[2]<<' '<<r[3]<<'\n';
  ASSERT_FLOAT_EQ(r[0],0.5f);
  ASSERT_FLOAT_EQ(r[1],1.0f);
  ASSERT_FLOAT_EQ(r[2],1.5f);
  ASSERT_FLOAT_EQ(r[3],2.0f);

}


TEST(SSE4,div4f)
{
  __m128 a=set4f(1.0f, 2.0f, 3.0f, 4.0f);
  __m128 b=splat4f(2.0f);
  __m128 res=div4f(a, b);
  float r[4];
  _mm_store_ps(r,res);

  std::cout<<"result "<<r[0]<<' '<<r[1]<<' '<<r[2]<<' '<<r[3]<<'\n';
  ASSERT_FLOAT_EQ(r[0],0.5f);
  ASSERT_FLOAT_EQ(r[1],1.0f);
  ASSERT_FLOAT_EQ(r[2],1.5f);
  ASSERT_FLOAT_EQ(r[3],2.0f);

}


TEST(SSE4,_mm_div_psZero)
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

  float length=convertf32(sqrt1f(dot4f(a, a, 0x71)));

  std::cout<<"length is "<<length<<'\n';
  EXPECT_NEAR(length,3.7416f,0.001f);

}


TEST(SSE4,_mm_rcpps_ps)
{
  // create a 4 float vector
  __m128 a=set4f(1.0f, 2.0f, 3.0f, 4.0f);
  // evauate 1/a for each vector component
  auto r1 = _mm_rcp_ps(a);
  float r[4];
  storeu4f(r,r1);
  // should set all values to inf
  std::cout<<"result "<<r[0]<<' '<<r[1]<<' '<<r[2]<<' '<<r[3]<<'\n';
  EXPECT_NEAR(r[0],1.0f/1.0f,0.001);
  EXPECT_NEAR(r[1],1.0f/2.0f,0.001);
  EXPECT_NEAR(r[2],1.0f/3.0f,0.001);
  EXPECT_NEAR(r[3],1.0f/4.0f,0.001);

}

TEST(SSE4,negate)
{
  f128 a=set4f(1.0f, 9.0f, 3.0f, 3.0f);
  f128 res=negate4f(a);
  float r[4];
  _mm_store_ps(r,res);

  std::cout<<"result "<<r[0]<<' '<<r[1]<<' '<<r[2]<<' '<<r[3]<<'\n';
  ASSERT_FLOAT_EQ(r[0],-1.0f);
  ASSERT_FLOAT_EQ(r[1],-9.0f);
  ASSERT_FLOAT_EQ(r[2],-3.0f);
  ASSERT_FLOAT_EQ(r[3],-3.0f);

}

TEST(SSE4,fmad4f)
{
  // result = (a * b) + c
  f128 a={1.0f,2.0f,3.0f,4.0f};
  f128 b={0.5f,0.5f,0.5f,0.5f};
  f128 c={2.0f,2.0f,2.0f,2.0f};

  f128 res=fmadd4f(a,b,c);
  float r[4];
  _mm_store_ps(r,res);

  std::cout<<"result "<<r[0]<<' '<<r[1]<<' '<<r[2]<<' '<<r[3]<<'\n';
  ASSERT_FLOAT_EQ(r[0],2.5f);
  ASSERT_FLOAT_EQ(r[1],3.0f);
  ASSERT_FLOAT_EQ(r[2],3.5f);
  ASSERT_FLOAT_EQ(r[3],4.0f);
}

TEST(SSE4,fnmad4f)
{
  // result = -(a * b) - c
  f128 a={1.0f,2.0f,3.0f,4.0f};
  f128 b={0.5f,0.5f,0.5f,0.5f};
  f128 c={1.0f,1.0f,1.0f,1.0f};

  f128 res=fnmadd4f(a,b,c);
  float r[4];
  _mm_store_ps(r,res);

  std::cout<<"result "<<r[0]<<' '<<r[1]<<' '<<r[2]<<' '<<r[3]<<'\n';
  ASSERT_FLOAT_EQ(r[0],0.5f);
  ASSERT_FLOAT_EQ(r[1],0.0f);
  ASSERT_FLOAT_EQ(r[2],-0.5f);
  ASSERT_FLOAT_EQ(r[3],-1.0f);
}

TEST(SSE4,sqrt4f)
{
  f128 a={100.0f,25.0f,144.0f,2.0f};
  f128 res=sqrt4f(a);
  float r[4];
  _mm_store_ps(r,res);

  std::cout<<"result "<<r[0]<<' '<<r[1]<<' '<<r[2]<<' '<<r[3]<<'\n';
  ASSERT_FLOAT_EQ(r[0],10.0f);
  ASSERT_FLOAT_EQ(r[1],5.0f);
  ASSERT_FLOAT_EQ(r[2],12.0f);
  ASSERT_NEAR(r[3],1.4141f,0.001);
}

TEST(SSE4,_mm_max_ps)
{
  // create a 4 float vector
  f128 a=set4f(1.0f, 9.0f, 3.0f, 3.0f);
  f128 b=set4f(2.0f, 5.0f, 6.0f, 4.0f);
  // evauate 1/a for each vector component
  auto r1 = _mm_max_ps(a,b);
  float r[4];
  storeu4f(r,r1);
  // should set all values to inf
  std::cout<<"result "<<r[0]<<' '<<r[1]<<' '<<r[2]<<' '<<r[3]<<'\n';
  ASSERT_FLOAT_EQ(r[0],2.0f);
  ASSERT_FLOAT_EQ(r[1],9.0f);
  ASSERT_FLOAT_EQ(r[2],6.0f);
  ASSERT_FLOAT_EQ(r[3],4.0f);
}

TEST(SSE4,_mm_min_ps)
{
  // create a 4 float vector
  __m128 a=set4f(1.0f, 9.0f, 3.0f, 3.0f);
  __m128 b=set4f(2.0f, 5.0f, 6.0f, 4.0f);
  // evauate 1/a for each vector component
  auto r1 = _mm_min_ps(a,b);
  float r[4];
  storeu4f(r,r1);
  // should set all values to inf
  std::cout<<"result "<<r[0]<<' '<<r[1]<<' '<<r[2]<<' '<<r[3]<<'\n';
  ASSERT_FLOAT_EQ(r[0],1.0f);
  ASSERT_FLOAT_EQ(r[1],5.0f);
  ASSERT_FLOAT_EQ(r[2],3.0f);
  ASSERT_FLOAT_EQ(r[3],3.0f);
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

int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}


