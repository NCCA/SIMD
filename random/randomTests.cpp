#include "simd.h"
#include <iostream>
#include <cmath>
#include "random.h"
#include <gtest/gtest.h>

TEST(RANDOM,SetSeed)
{
  frng::setSeed(99);
  auto rn=frng::randomFast();
  ASSERT_EQ(25193669,rn);
  frng::setSeed(250);
  rn=frng::randomFast();
  ASSERT_EQ(66543189,rn);
}


TEST(RANDOMSSE,TableSEE)
{
  frng::setSeedSSE(99);
  frng::setSeed(99);
  for(int i=0; i<10; ++i)
  {
    std::cout<<"R "<<frng::randomFast()<<'\n';
  }
  i128 rn;
  alignas(16) uint32_t res[4];
  for(int i=0; i<10; ++i)
  {
    rn=frng::randomSSE();
    store4i(&res[0],rn);
    std::cout<<"S "<<res[0]<<' '<<res[1]<<' '<<res[2]<<' '<<res[3]<<'\n';
  }
}

// When using SSE we are going to only have the same value in the first
// register as the seeds are different in each block however we
// should get the same results in res[0] -> randomFast()

TEST(RANDOMSSE,SetSeedSEE)
{
  frng::setSeedSSE(99);
  frng::setSeed(99);
  i128 rn=frng::randomSSE();
  alignas(16) uint32_t res[4];
  store4i(&res[0],rn);

  std::cout<<"randomFast() "<<frng::randomFast()<<" S "<<res[0]<<' '<<res[1]<<' '<<res[2]<<' '<<res[3]<<'\n';
  ASSERT_EQ(res[0],25193669);

  rn=frng::randomSSE();
  store4i(&res[0],rn);
  std::cout<<"randomFast() "<<frng::randomFast()<<" S "<<res[0]<<' '<<res[1]<<' '<<res[2]<<' '<<res[3]<<'\n';
  ASSERT_EQ(res[0],2399245289);

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
  frng::setSeed(99);
  float fr=frng::randomFloat();
  std::cout<<"random Float "<<fr<<'\n';

  float res[4];
  auto rn=frng::randomFloatSSE();
  store4f(&res[0],rn);
  std::cout<<"SSE ["<<res[0]<<' '<<res[1]<<' '<<res[2]<<' '<<res[3]<<"]\n";
  ASSERT_FLOAT_EQ(res[0],fr);

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
  ASSERT_NEAR(res[0],fr,0.001);
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
  for(size_t i=0; i<500000; ++i)
  {
    frng::setSeedSSE(i);
    frng::setSeed(i);
    auto rn=frng::randomFloatSSE(-2.0f,2.0f);
    store4f(&res[0],rn);
    ASSERT_NEAR(res[0],frng::randomFloat(-2.0f,2.0f),0.001f);
  }
}



int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}