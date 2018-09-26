#include "ParticleSystemNormal.h"
#include <cassert>
#include <iostream>
#include <ngl/VAOFactory.h>
#include <ngl/SimpleVAO.h>
#include <ngl/NGLStream.h>
#include <ngl/Random.h>
#include <vector>
#ifdef GOOGLEBENCH
  #include <benchmark/benchmark.h>
#endif
// based on code from here https://software.intel.com/en-us/articles/creating-a-particle-system-with-streaming-simd-extensions

ParticleSystemNormal::ParticleSystemNormal(size_t _numParticles,ngl::Vec3 _pos)
{
  m_numParticles=_numParticles;
  m_particles.reset(new Particle(_numParticles));
  m_pos=_pos;
  setDefaults();
  m_vao=ngl::VAOFactory::createVAO(ngl::simpleVAO,GL_POINTS);
  m_vao->bind();
  std::vector<ngl::Vec3> data(_numParticles);
  m_vao->setData( ngl::SimpleVAO::VertexData(m_numParticles*sizeof(ngl::Vec3),data[0].m_x));
  // We must do this each time as we change the data.
  m_vao->setVertexAttributePointer(0,3,GL_FLOAT,0,0);
  m_vao->setNumIndices(m_numParticles);
  m_vao->unbind();



}

ParticleSystemNormal::~ParticleSystemNormal()
{

}




void ParticleSystemNormal::setDefaults()
{
  ngl::Random *rng=ngl::Random::instance();

  for (size_t i=0; i < m_numParticles ; ++i)
  {
    m_particles->m_x[i]=m_pos.m_x;
    m_particles->m_y[i]=m_pos.m_y;
    m_particles->m_z[i]=m_pos.m_z;

    m_particles->m_vx[i] = rng->randomNumber(1.0f);
    m_particles->m_vy[i] = rng->randomNumber(1.0f);
    m_particles->m_vz[i] = rng->randomNumber(1.0f);

    m_particles->m_ax[i]=rng->randomNumber(1.0f);
    m_particles->m_ay[i]=-rng->randomNumber(1.0f);
    m_particles->m_az[i]=rng->randomNumber(1.0f);

    m_particles->m_energy[ i ] = rng->randomPositiveNumber(m_energyRange);
    m_particles->m_alive[ i ] = true;
  }

}

void ParticleSystemNormal::setNumParticles(size_t _numParticles)
{


}



void ParticleSystemNormal::update(float _elapsed)
{

  for (size_t i = 0; i < m_numParticles; ++i)
  {
    m_particles->m_vx[i] +=  _elapsed * m_particles->m_ax[i];
    m_particles->m_vy[i] +=  _elapsed * m_particles->m_ay[i];
    m_particles->m_vz[i] +=  _elapsed * m_particles->m_az[i];

    // Y velocity = Y velocity + (gravity * time)
    m_particles->m_vy[i] +=  s_gravity * _elapsed;

    // p = p + v * t
    m_particles->m_x[i] += m_particles->m_vx[i] * _elapsed;
    m_particles->m_y[i] += m_particles->m_vy[i] * _elapsed;
    m_particles->m_z[i] += m_particles->m_vz[i] * _elapsed;

    // energy = energy - time
    m_particles->m_energy[i] -= _elapsed;

    if (0 >= m_particles->m_energy[i])
       setParticleDefaults(i);
  }
    // do collision
    // stretch particles
    // paritcle has light

}

void ParticleSystemNormal::setParticleDefaults(size_t particleIndex)
{
  ngl::Random *rng=ngl::Random::instance();
  m_particles->m_x[particleIndex]       = m_pos.m_x;
  m_particles->m_y[particleIndex]       = m_pos.m_y;
  m_particles->m_z[particleIndex]       = m_pos.m_z;
  ngl::Vec3 dir=rng->getRandomNormalizedVec3();
  m_particles->m_vx[particleIndex]      = dir.m_x;
  m_particles->m_vy[particleIndex]      = rng->randomPositiveNumber(1.0f) * 6.0f + 4.0f;
  m_particles->m_vz[particleIndex]      = dir.m_y;

  m_particles->m_ax[particleIndex]      = rng->randomPositiveNumber(1.0f);
  m_particles->m_ay[particleIndex]      = -rng->randomPositiveNumber(1.0f);
  m_particles->m_az[particleIndex]      = rng->randomPositiveNumber(1.0f);

  m_particles->m_energy[particleIndex]  = rng->randomPositiveNumber(m_energyRange);

  m_particles->m_alive[particleIndex]   = true;
}

void ParticleSystemNormal::render()
{
  size_t v = 0;

  ngl::Vec3 *verts=reinterpret_cast<ngl::Vec3 *> (m_vao->mapBuffer());


for (size_t  i = 0; i < m_numParticles; i++)
 {
   if (m_particles->m_alive[i])
   {
     verts[v].m_x=m_particles->m_x[i];
     verts[v].m_y=m_particles->m_y[i];
     verts[v].m_z=m_particles->m_z[i];
     ++v;
   }
 }
m_vao->unmapBuffer();
// now render
m_vao->bind();
m_vao->draw();
m_vao->unbind();



}




