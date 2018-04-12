#include "ParticleSystemAOS.h"
#include <cassert>
#include <iostream>
#include <ngl/VAOFactory.h>
#include <ngl/SimpleVAO.h>
#include <ngl/NGLStream.h>
#include <ngl/Random.h>
#include <vector>
#include <ngl/Util.h>
// based on code from here https://software.intel.com/en-us/articles/creating-a-particle-system-with-streaming-simd-extensions

ParticleSystemAOS::ParticleSystemAOS(size_t _numParticles,ngl::Vec3 _pos)
{
  m_numParticles=_numParticles;
  m_particles.reset(new ParticleAOS[_numParticles]);
  m_pos=_pos;
  setDefaults();
  m_vao.reset( ngl::VAOFactory::createVAO(ngl::simpleVAO,GL_POINTS));
  m_vao->bind();

  std::vector<ngl::Vec3> data(_numParticles);
  m_vao->setData( ngl::SimpleVAO::VertexData(m_numParticles*sizeof(ngl::Vec3),data[0].m_x));
  // We must do this each time as we change the data.
  m_vao->setVertexAttributePointer(0,3,GL_FLOAT,0,0);
  m_vao->setNumIndices(m_numParticles);
  m_vao->unbind();
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

  size_t v = 0;
  ngl::Vec3 *verts=reinterpret_cast<ngl::Vec3 *> (m_vao->mapBuffer());

  for (size_t  i = 0; i < m_numParticles; ++i)
   {
     if (m_particles[i].m_alive)
     {
       verts[v].m_x=m_particles[i].m_x;
       verts[v].m_y=m_particles[i].m_y;
       verts[v].m_z=m_particles[i].m_z;
       ++v;
     }
   }
  m_vao->unmapBuffer();
  // now render
  m_vao->bind();
  m_vao->draw();
  m_vao->unbind();

}



