#include "ParticleSystemAOS.h"
#include <cassert>
#include <iostream>
#include <ngl/VAOFactory.h>
#include <ngl/SimpleVAO.h>
#include <ngl/NGLStream.h>
#include <ngl/Random.h>
#include <vector>
#include <benchmark/benchmark.h>
// based on code from here https://software.intel.com/en-us/articles/creating-a-particle-system-with-streaming-simd-extensions

ParticleSystemAOS::ParticleSystemAOS(size_t _numParticles,ngl::Vec3 _pos)
{
  m_numParticles=_numParticles;
  m_particles.reset(new Particle[_numParticles]);
  m_pos=_pos;
  setDefaults();


}

ParticleSystemAOS::~ParticleSystemAOS()
{

}




void ParticleSystemAOS::setDefaults()
{
  ngl::Random *rng=ngl::Random::instance();

  for (size_t i=0; i < m_numParticles ; ++i)
  {
    m_particles[i].m_x=m_pos.m_x;
    m_particles[i].m_y=m_pos.m_y;
    m_particles[i].m_z=m_pos.m_z;

    m_particles[i].m_vx = rng->randomNumber(1.0f);
    m_particles[i].m_vy = rng->randomNumber(1.0f);
    m_particles[i].m_vz = rng->randomNumber(1.0f);

    m_particles[i].m_ax=rng->randomNumber(1.0f);
    m_particles[i].m_ay=-rng->randomNumber(1.0f);
    m_particles[i].m_az=rng->randomNumber(1.0f);

    m_particles[i].m_energy = rng->randomPositiveNumber(m_energyRange);
    m_particles[i].m_alive = true;
  }

}

void ParticleSystemAOS::setNumParticles(size_t _numParticles)
{


}



void ParticleSystemAOS::update(float _elapsed)
{

  for (size_t i = 0; i < m_numParticles; ++i)
  {
    m_particles[i].m_vx +=  _elapsed * m_particles[i].m_ax;
    m_particles[i].m_vy +=  _elapsed * m_particles[i].m_ay;
    m_particles[i].m_vz +=  _elapsed * m_particles[i].m_az;

    // Y velocity = Y velocity + (gravity * time)
    m_particles[i].m_vy +=  s_gravity * _elapsed;

    // p = p + v * t
    m_particles[i].m_x += m_particles[i].m_vx * _elapsed;
    m_particles[i].m_y += m_particles[i].m_vy * _elapsed;
    m_particles[i].m_z += m_particles[i].m_vz * _elapsed;

    // energy = energy - time
    m_particles[i].m_energy -= _elapsed;

    if (0 >= m_particles[i].m_energy)
       setParticleDefaults(i);
  }
    // do collision
    // stretch particles
    // paritcle has light

}

void ParticleSystemAOS::setParticleDefaults(size_t particleIndex)
{
  ngl::Random *rng=ngl::Random::instance();
  m_particles[particleIndex].m_x       = m_pos.m_x;
  m_particles[particleIndex].m_y       = m_pos.m_y;
  m_particles[particleIndex].m_z       = m_pos.m_z;
  ngl::Vec3 dir=rng->getRandomNormalizedVec3();
  m_particles[particleIndex].m_vx      = dir.m_x;
  m_particles[particleIndex].m_vy      = rng->randomPositiveNumber(1.0f) * 6.0f + 4.0f;
  m_particles[particleIndex].m_vz      = dir.m_y;

  m_particles[particleIndex].m_ax= rng->randomPositiveNumber(1.0f);
  m_particles[particleIndex].m_ay= -rng->randomPositiveNumber(1.0f);
  m_particles[particleIndex].m_az= rng->randomPositiveNumber(1.0f);

  m_particles[particleIndex].m_energy = rng->randomPositiveNumber(m_energyRange);

  m_particles[particleIndex].m_alive = true;
}

void ParticleSystemAOS::render()
{


for (size_t  i = 0; i < m_numParticles; i++)
 {
   if (m_particles[i].m_alive)
   {
     benchmark::DoNotOptimize(m_particles[i].m_x);
     benchmark::DoNotOptimize(m_particles[i].m_y);
     benchmark::DoNotOptimize(m_particles[i].m_z);
   }
 }
}


 void ParticleSystemAOS::updatePosition(float _dx, float _dy, float _dz)
 {
    m_pos.m_x+=_dx;
    m_pos.m_y+=_dy;
    m_pos.m_z+=_dz;
 }


