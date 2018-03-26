#ifndef ABSTRACTPARTICLESYSTEM_H_
#define ABSTRACTPARTICLESYSTEM_H_
#include <ngl/Vec3.h>

class AbstractParticleSystem
{
 public :
  AbstractParticleSystem()=default;
  virtual ~AbstractParticleSystem()=default;
  virtual void update(float _elapsed)=0;
  virtual void render()=0;
  virtual void setNumParticles(size_t numParticles)=0;
  virtual void setDefaults()=0;
  virtual void setParticleDefaults(size_t particleIndex)=0;
  void updatePosition(float _dx, float _dy, float _dz)
  {
    m_pos.m_x+=_dx;
    m_pos.m_y+=_dy;
    m_pos.m_z+=_dz;
  }
  void updateEnergy(float _dt) {m_energyRange+=_dt;}
  ngl::Vec3 m_pos;
  float m_energyRange=1.0f;

};


#endif
