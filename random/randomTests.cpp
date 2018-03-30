#include "simd.h"
#include <iostream>
#include <cmath>
#include "random.h"
#include <gtest/gtest.h>

/*
TEST(RANDOM,SetSeed)
{
  frng::setSeed(99);
  auto rn=frng::randomFast();
  ASSERT_EQ(25193669,rn);
  frng::setSeed(250);
  rn=frng::randomFast();
  ASSERT_EQ(66543189,rn);
}

// When using SSE we are going to only have the same value in the first
// register as the seeds are different in each block however we
// should get the same results in res[0] -> randomFast()

TEST(RANDOMSSE,SetSeedSEE)
{
  frng::setSeedSSE(99);
  i128 rn=frng::randomSSE();
  alignas(16) uint32_t res[4];
  store4i(&res[0],rn);
  // Note randomFast with param is a unit test only function
  ASSERT_EQ(res[0],frng::randomFast(99));
  ASSERT_EQ(res[1],frng::randomFast(100));
  ASSERT_EQ(res[2],frng::randomFast(101));
  ASSERT_EQ(res[3],frng::randomFast(102));


}


TEST(RANDOMSSE,loopRand)
{
  frng::setSeedSSE(2345);
  frng::setSeed(2345);
  alignas(16) uint32_t a[4];

  for(size_t i=0; i<250000; ++i)
  {
    i128 rn=frng::randomSSE();
    store4i(&a[0],rn);

    ASSERT_EQ(a[0],frng::randomFast());
  }

}

TEST(RANDOMSSE,loopRandSeed)
{
  alignas(16) uint32_t a[4];

  for(uint32_t i=0; i<250000; ++i)
  {
    frng::setSeedSSE(i);
    frng::setSeed(i);
    i128 rn=frng::randomSSE();
    store4i(&a[0],rn);

    ASSERT_EQ(a[0],frng::randomFast());
  }

}


TEST(RANDOMSSE,randFloat)
{
  frng::setSeedSSE(99);

  float res[4];
  auto rn=frng::randomFloatSSE();
  store4f(&res[0],rn);
  std::cout<<"SSE ["<<res[0]<<' '<<res[1]<<' '<<res[2]<<' '<<res[3]<<"]\n";
  ASSERT_FLOAT_EQ(res[0],frng::randomFloat(99));
  ASSERT_FLOAT_EQ(res[1],frng::randomFloat(100));
  ASSERT_FLOAT_EQ(res[2],frng::randomFloat(101));
  ASSERT_FLOAT_EQ(res[3],frng::randomFloat(102));

}

TEST(RANDOMSSE,randFloatLimits)
{
  frng::setSeed(uint32_t(time(NULL)));
  for(size_t i=0; i<1000000; ++i)
  {
    auto t=frng::randomFloat();
    ASSERT_TRUE(t>=0.0f && t<=1.0f);
  }
}

TEST(RANDOMSSE,randFloatRange)
{
  frng::setSeedSSE(99);
  frng::setSeed(99);
  float fr=frng::randomFloat(-2.0f,2.0f);
  std::cout<<"random Float "<<fr<<'\n';

  float res[4];
  auto rn=frng::randomFloatSSE(-2.0f,2.0f);
  store4f(&res[0],rn);
  std::cout<<"SSE ["<<res[0]<<' '<<res[1]<<' '<<res[2]<<' '<<res[3]<<"]\n";
  ASSERT_NEAR(res[0],fr,0.001f);
}

TEST(RANDOMSSE,randFloatLoop)
{
  frng::setSeedSSE(99);
  frng::setSeed(99);
  float res[4];
  for(size_t i=0; i<500000; ++i)
  {
    auto rn=frng::randomFloatSSE();
    store4f(&res[0],rn);
    ASSERT_FLOAT_EQ(res[0],frng::randomFloat());
  }
}

TEST(RANDOMSSE,randFloatLoopSeed)
{
  float res[4];
  for(size_t i=0; i<500000; ++i)
  {
    frng::setSeedSSE(i);
    frng::setSeed(i);
    auto rn=frng::randomFloatSSE();
    store4f(&res[0],rn);
    ASSERT_FLOAT_EQ(res[0],frng::randomFloat());
  }
}


TEST(RANDOMSSE,randFloatLoopRange)
{
  frng::setSeedSSE(99);
  frng::setSeed(99);
  float res[4];
  for(size_t i=0; i<500000; ++i)
  {
    auto rn=frng::randomFloatSSE(-2.0f,2.0f);
    store4f(&res[0],rn);
    ASSERT_NEAR(res[0],frng::randomFloat(-2.0f,2.0f),0.001f);
  }
}

TEST(RANDOMSSE,randFloatLoopSeedRange)
{
  float res[4];
  for(size_t i=0; i<900000; ++i)
  {
    frng::setSeedSSE(i);
    frng::setSeed(i);
    auto rn=frng::randomFloatSSE(-2.0f,2.0f);
    store4f(&res[0],rn);
    ASSERT_NEAR(res[0],frng::randomFloat(-2.0f,2.0f),0.001f);
  }
}

TEST(RANDOMSSE,uniformFloat)
{
  frng::setSeed(99);
  for(int i=0; i<10; ++i)
  {
    auto t=frng::uniformFloat();
    std::cout<<t<<'\n';
    ASSERT_TRUE(t>=0.0f && t<=1.0f);

  }
}

TEST(RANDOMSSE,uniformFloatSSE)
{
  float res[4];
  frng::setSeedSSE(99);
  for(int i=0; i<10; ++i)
  {
    auto rn=frng::uniformFloatSSE();
    store4f(&res[0],rn);
    std::cout<<"UF "<<res[0]<<' '<<res[1]<<' '<<res[2]<<' '<<res[3]<<'\n';
  }

}


TEST(RANDOMSSE,frexp4f)
{
  float input[4]={0.002f,9.99f,0.21f,0.654321f};
  f128 test=load4f(input);
  i128 ires;
  f128 r=frng::frexp4f(test,ires);
  float res[4];
  int32_t resi[4];
  store4f(&res[0],r);
  store4i(&resi[0],ires);

  std::cout<<"frexp4f float "<<res[0]<<' '<<res[1]<<' '<<res[2]<<' '<<res[3]<<'\n';
  std::cout<<"frexp4f int "<<resi[0]<<' '<<resi[1]<<' '<<resi[2]<<' '<<resi[3]<<'\n';

 float dres[4];
 int idres[4];

  dres[0]=frexpf(input[0],&idres[0]);
  dres[1]=frexpf(input[1],&idres[1]);
  dres[2]=frexpf(input[2],&idres[2]);
  dres[3]=frexpf(input[3],&idres[3]);
  std::cout<<"frexpf float "<<dres[0]<<' '<<dres[1]<<' '<<dres[2]<<' '<<dres[3]<<'\n';
  std::cout<<"frexpf int "<<idres[0]<<' '<<idres[1]<<' '<<idres[2]<<' '<<idres[3]<<'\n';

  EXPECT_EQ(resi[0],idres[0]);
  EXPECT_EQ(resi[1],idres[1]);
  EXPECT_EQ(resi[2],idres[2]);
  EXPECT_EQ(resi[3],idres[3]);

  EXPECT_FLOAT_EQ(res[0],dres[0]);
  EXPECT_FLOAT_EQ(res[1],dres[1]);
  EXPECT_FLOAT_EQ(res[2],dres[2]);
  EXPECT_FLOAT_EQ(res[3],dres[3]);
}


TEST(RANDOMSEE,log4f)
{
  float data[4]={2.0f,5.0f,80.0f,100.0f};
  auto sslog=frng::log4f(load4f(data));
  float res[4];
  store4f(&res[0],sslog);
  auto fsslog=frng::fastlog4f(load4f(data));
  float fres[4];
  store4f(&fres[0],fsslog);

  float nlog[4];
//  nlog[0]=logf(data[0]);
//  nlog[1]=logf(data[1]);
//  nlog[2]=logf(data[2]);
//  nlog[3]=logf(data[3]);
  nlog[0]=frng::log1f(data[0]);
  nlog[1]=frng::log1f(data[1]);
  nlog[2]=frng::log1f(data[2]);
  nlog[3]=frng::log1f(data[3]);

  std::cout<<"log4f "<<res[0]<<' '<<res[1]<<' '<<res[2]<<' '<<res[3]<<'\n';
  std::cout<<"fastlog4f "<<fres[0]<<' '<<fres[1]<<' '<<fres[2]<<' '<<fres[3]<<'\n';
  std::cout<<"log1f "<<nlog[0]<<' '<<nlog[1]<<' '<<nlog[2]<<' '<<nlog[3]<<'\n';

  nlog[0]=logf(data[0]);
  nlog[1]=logf(data[1]);
  nlog[2]=logf(data[2]);
  nlog[3]=logf(data[3]);
  std::cout<<"logf "<<nlog[0]<<' '<<nlog[1]<<' '<<nlog[2]<<' '<<nlog[3]<<'\n';


  ASSERT_FLOAT_EQ(res[0],nlog[0]);
  ASSERT_FLOAT_EQ(res[1],nlog[1]);
  ASSERT_FLOAT_EQ(res[2],nlog[2]);
  ASSERT_FLOAT_EQ(res[3],nlog[3]);

}


TEST(RANDOMSSE,exponentf)
{
  f128 test=set4f(1.002f,9.99f,234.001f,324.654321f);
  auto r=frng::exponent_f(test);
  float res[4];
  store4f(&res[0],r);
  std::cout<<"exponent_f "<<res[0]<<' '<<res[1]<<' '<<res[2]<<' '<<res[3]<<'\n';
}
*/

TEST(RANDOM,jkiss32)
{
  frng::seedJkiss4i();
  uint32_t res[4];
  for(size_t i=0; i<10; ++i)
  {
    auto r=frng::jkiss4i();
    auto r32=frng::jkiss32();
    store4i(&res[0],r);
    std::cout<<"jkist32 "<<r32<<" jkiss4i "<<res[0]<<' '<<res[1]<<' '<<res[2]<<' '<<res[3]<<'\n';
    ASSERT_FLOAT_EQ(r32,res[0]);
  }
}


int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
