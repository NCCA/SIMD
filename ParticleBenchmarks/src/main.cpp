#include "ParticleSystemSSE.h"
#include "ParticleSystemSSEFMA.h"
#include "ParticleSystemNormal.h"

#include <benchmark/benchmark.h>

class ParticleSystemSSEFixture : public ::benchmark::Fixture
{
 public:
  void SetUp(const ::benchmark::State& st)
  {

    particles= new ParticleSystemSSE(st.range(0),{0,0,0});
  }

  void TearDown(const ::benchmark::State&)
  {
    delete particles;
  }
  ParticleSystemSSE *particles;

};


BENCHMARK_DEFINE_F(ParticleSystemSSEFixture, Update)(benchmark::State& state)
{
    while (state.KeepRunning())
    {
      particles->update(0.01f);
    }
}

BENCHMARK_DEFINE_F(ParticleSystemSSEFixture, Render)(benchmark::State& state)
{
    while (state.KeepRunning())
    {
      particles->render();
    }
}


class ParticleSystemSSEFMAFixture : public ::benchmark::Fixture
{
 public:
  void SetUp(const ::benchmark::State& st)
  {

    particles= new ParticleSystemSSEFMA(st.range(0),{0,0,0});
  }

  void TearDown(const ::benchmark::State&)
  {
    delete particles;
  }
  ParticleSystemSSEFMA *particles;

};

BENCHMARK_DEFINE_F(ParticleSystemSSEFMAFixture, Update)(benchmark::State& state)
{
    while (state.KeepRunning())
    {
      particles->update(0.01f);
    }
}

BENCHMARK_DEFINE_F(ParticleSystemSSEFMAFixture, Render)(benchmark::State& state)
{
    while (state.KeepRunning())
    {
      particles->render();
    }
}


class ParticleSystemNormalFixture : public ::benchmark::Fixture
{
 public:
  void SetUp(const ::benchmark::State& st)
  {

    particles= new ParticleSystemNormal(st.range(0),{0,0,0});
  }

  void TearDown(const ::benchmark::State&)
  {
    delete particles;
  }
  ParticleSystemNormal *particles;

};


BENCHMARK_DEFINE_F(ParticleSystemNormalFixture, Update)(benchmark::State& state)
{
    while (state.KeepRunning())
    {
      particles->update(0.01f);
    }
}

BENCHMARK_DEFINE_F(ParticleSystemNormalFixture, Render)(benchmark::State& state)
{
    while (state.KeepRunning())
    {
      particles->render();
    }
}


constexpr int step=10;
constexpr int rangeStart=1024;
constexpr int rangeEnd=1<<18;
BENCHMARK_REGISTER_F(ParticleSystemSSEFixture, Update)->RangeMultiplier(step)->Range(rangeStart,rangeEnd);
BENCHMARK_REGISTER_F(ParticleSystemSSEFMAFixture, Update)->RangeMultiplier(step)->Range(rangeStart,rangeEnd);
BENCHMARK_REGISTER_F(ParticleSystemNormalFixture, Update)->RangeMultiplier(step)->Range(rangeStart,rangeEnd);
BENCHMARK_REGISTER_F(ParticleSystemSSEFixture, Render)->RangeMultiplier(step)->Range(rangeStart,rangeEnd);
BENCHMARK_REGISTER_F(ParticleSystemSSEFMAFixture, Render)->RangeMultiplier(step)->Range(rangeStart,rangeEnd);
BENCHMARK_REGISTER_F(ParticleSystemNormalFixture, Render)->RangeMultiplier(step)->Range(rangeStart,rangeEnd);

BENCHMARK_MAIN();
