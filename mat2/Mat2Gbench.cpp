#include <benchmark/benchmark.h>
#include <gtest/gtest.h>
#include "Mat2Func.h"


static void Normal(benchmark::State& state)
{
  Mat2 a;
  Mat2 b(1.0f,2.0f,3.0f,4.0f);
  Mat2 c(2.0f,3.0f,4.0f,5.0f);

  for (auto _ : state)
  {
     MultMat2Normal(a,b,c);
     benchmark::DoNotOptimize(a);
  }
}


BENCHMARK(Normal);


static void Unroll(benchmark::State& state)
{
  Mat2 a;
  Mat2 b(1.0f,2.0f,3.0f,4.0f);
  Mat2 c(2.0f,3.0f,4.0f,5.0f);
  for (auto _ : state)
  {
    MultMat2Unroll(a,b,c);
    benchmark::DoNotOptimize(a);
  }
}

BENCHMARK( Unroll);

static void Prefetch(benchmark::State& state)
{
  Mat2 a;
  Mat2 b(1.0f,2.0f,3.0f,4.0f);
  Mat2 c(2.0f,3.0f,4.0f,5.0f);
  for (auto _ : state)
  {
    MultMat2Prefetch(a,b,c);
    benchmark::DoNotOptimize(a);
  }
}


BENCHMARK( Prefetch);


static void OperatorMultFunc(benchmark::State& state)
{
  Mat2 a;
  Mat2 b(1.0f,2.0f,3.0f,4.0f);
  Mat2 c(2.0f,3.0f,4.0f,5.0f);
  for (auto _ : state)
  {
    MultMat2Operator(a,b,c);
    benchmark::DoNotOptimize(a);
  }
}

BENCHMARK(OperatorMultFunc);

static void OperatorMult(benchmark::State& state)
{
  Mat2 a;
  Mat2 b(1.0f,2.0f,3.0f,4.0f);
  Mat2 c(2.0f,3.0f,4.0f,5.0f);
  for (auto _ : state)
  {
    a=b*c;
    benchmark::DoNotOptimize(a);
  }
}

BENCHMARK(OperatorMult);


static void SIMD(benchmark::State& state)
{
  Mat2 a;
  Mat2 b(1.0f,2.0f,3.0f,4.0f);
  Mat2 c(2.0f,3.0f,4.0f,5.0f);
  for (auto _ : state)
  {
    MultMat2SIMD(a,b,c);
    benchmark::DoNotOptimize(a);
  }
}

BENCHMARK(SIMD);


static void AVX2(benchmark::State& state)
{
  Mat2 a;
  Mat2 b(1.0f,2.0f,3.0f,4.0f);
  Mat2 c(2.0f,3.0f,4.0f,5.0f);
  for (auto _ : state)
  {
    MultMat2AVX2(a,b,c);
    benchmark::DoNotOptimize(a);
  }
}

BENCHMARK(AVX2);



BENCHMARK_MAIN();

