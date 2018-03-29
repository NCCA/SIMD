#include "ParticleSystemSSE.h"
#include "ParticleSystemSSEFMA.h"
#include "ParticleSystemNormal.h"
#include "ParticleSystemAOS.h"
#include "ParticleSystemAVX2.h"
#include "ParticleSystemSSERand.h"

#include <benchmark/benchmark.h>

template<typename T>
class ParticleSystemFixture : public ::benchmark::Fixture
{
 public:
  void SetUp(const ::benchmark::State& st)
  {

    particles= new T(st.range(0),{0,0,0});
  }

  void TearDown(const ::benchmark::State&)
  {
    delete particles;
  }
  T *particles;

};


BENCHMARK_TEMPLATE_DEFINE_F(ParticleSystemFixture, AOSUpdate, ParticleSystemAOS)(benchmark::State& st)
{
 for (auto _ : st)
 {
   particles->update(0.01f);
 }
}

BENCHMARK_TEMPLATE_DEFINE_F(ParticleSystemFixture, AOSRender, ParticleSystemAOS)(benchmark::State& st)
{
 for (auto _ : st)
 {
   particles->render();
 }
}


BENCHMARK_TEMPLATE_DEFINE_F(ParticleSystemFixture, NormalUpdate, ParticleSystemNormal)(benchmark::State& st)
{
 for (auto _ : st)
 {
   particles->update(0.01f);
 }
}

BENCHMARK_TEMPLATE_DEFINE_F(ParticleSystemFixture, NormalRender, ParticleSystemNormal)(benchmark::State& st)
{
 for (auto _ : st)
 {
   particles->render();
 }
}


BENCHMARK_TEMPLATE_DEFINE_F(ParticleSystemFixture, SSEUpdate, ParticleSystemSSE)(benchmark::State& st)
{
 for (auto _ : st)
 {
   particles->update(0.01f);
 }
}

BENCHMARK_TEMPLATE_DEFINE_F(ParticleSystemFixture, SSERender, ParticleSystemSSE)(benchmark::State& st)
{
 for (auto _ : st)
 {
   particles->render();
 }
}

BENCHMARK_TEMPLATE_DEFINE_F(ParticleSystemFixture, SSEFMAUpdate, ParticleSystemSSEFMA)(benchmark::State& st)
{
 for (auto _ : st)
 {
   particles->update(0.01f);
 }
}

BENCHMARK_TEMPLATE_DEFINE_F(ParticleSystemFixture, SSEFMARender, ParticleSystemSSEFMA)(benchmark::State& st)
{
 for (auto _ : st)
 {
   particles->render();
 }
}


BENCHMARK_TEMPLATE_DEFINE_F(ParticleSystemFixture, AVX2Update, ParticleSystemAVX2)(benchmark::State& st)
{
 for (auto _ : st)
 {
   particles->update(0.01f);
 }
}

BENCHMARK_TEMPLATE_DEFINE_F(ParticleSystemFixture, AVX2Render, ParticleSystemAVX2)(benchmark::State& st)
{
 for (auto _ : st)
 {
   particles->render();
 }
}


BENCHMARK_TEMPLATE_DEFINE_F(ParticleSystemFixture, SSERandomUpdate, ParticleSystemSSERAND)(benchmark::State& st)
{
 for (auto _ : st)
 {
   particles->update(0.01f);
 }
}

BENCHMARK_TEMPLATE_DEFINE_F(ParticleSystemFixture, SSERandomRender, ParticleSystemSSERAND)(benchmark::State& st)
{
 for (auto _ : st)
 {
   particles->render();
 }
}

constexpr int step=10;
constexpr int rangeStart=1024;
constexpr int rangeEnd=1000000; //24;

//BENCHMARK_REGISTER_F(ParticleSystemFixture, AOSUpdate)->RangeMultiplier(step)->Range(rangeStart,rangeEnd);
//BENCHMARK_REGISTER_F(ParticleSystemFixture, NormalUpdate)->RangeMultiplier(step)->Range(rangeStart,rangeEnd);
//BENCHMARK_REGISTER_F(ParticleSystemFixture, SSEUpdate)->RangeMultiplier(step)->Range(rangeStart,rangeEnd);
//BENCHMARK_REGISTER_F(ParticleSystemFixture, SSEFMAUpdate)->RangeMultiplier(step)->Range(rangeStart,rangeEnd);
BENCHMARK_REGISTER_F(ParticleSystemFixture, AVX2Update)->RangeMultiplier(step)->Range(rangeStart,rangeEnd);
BENCHMARK_REGISTER_F(ParticleSystemFixture, SSERandomUpdate)->RangeMultiplier(step)->Range(rangeStart,rangeEnd);

//BENCHMARK_REGISTER_F(ParticleSystemFixture, AOSRender)->RangeMultiplier(step)->Range(rangeStart,rangeEnd);
//BENCHMARK_REGISTER_F(ParticleSystemFixture, NormalRender)->RangeMultiplier(step)->Range(rangeStart,rangeEnd);
//BENCHMARK_REGISTER_F(ParticleSystemFixture, SSERender)->RangeMultiplier(step)->Range(rangeStart,rangeEnd);
//BENCHMARK_REGISTER_F(ParticleSystemFixture, SSEFMARender)->RangeMultiplier(step)->Range(rangeStart,rangeEnd);
BENCHMARK_REGISTER_F(ParticleSystemFixture, AVX2Render)->RangeMultiplier(step)->Range(rangeStart,rangeEnd);
BENCHMARK_REGISTER_F(ParticleSystemFixture, SSERandomRender)->RangeMultiplier(step)->Range(rangeStart,rangeEnd);

BENCHMARK_MAIN();
