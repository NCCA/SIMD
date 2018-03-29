#include <benchmark/benchmark.h>
#include "random.h"

float stdrandomFloat(float min, float max)
{
    return  (max - min) * ((((float) rand()) / (float) UINT32_MAX)) + min ;
}


static void XORRand(benchmark::State& state)
{
  for (auto _ : state)
  {
    auto a=frng::randomFast();
  }
}


static void Rand(benchmark::State& state)
{
  for (auto _ : state)
  {
    auto a=rand();
  }
}


static void RandFloat(benchmark::State& state)
{
  using namespace frng;

  for (auto _ : state)
  {
    auto a=randomFloat();
  }
}

static void RandFloatRange(benchmark::State& state)
{
  using namespace frng;

  for (auto _ : state)
  {
    auto a=randomFloat(-10.0f,10.0f);
  }
}

static void StdRandFloatRange(benchmark::State& state)
{
  using namespace frng;

  for (auto _ : state)
  {
    auto a=stdrandomFloat(-10.0f,10.0f);
  }
}

static void UniformFloat(benchmark::State& state)
{
  using namespace frng;

  for (auto _ : state)
  {
    auto a=uniformFloat();
  }
}

static void RandomSSE(benchmark::State& state)
{
  using namespace frng;

  for (auto _ : state)
  {
    auto a=randomSSE();
  }
}


// Register the function as a benchmark
BENCHMARK(XORRand);
BENCHMARK(Rand);
BENCHMARK(RandFloat);
BENCHMARK(RandFloatRange);
BENCHMARK(StdRandFloatRange);
BENCHMARK(UniformFloat);
BENCHMARK(RandomSSE);


BENCHMARK_MAIN();
