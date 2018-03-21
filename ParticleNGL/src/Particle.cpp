#include "Particle.h"
// based on code from here https://software.intel.com/en-us/articles/creating-a-particle-system-with-streaming-simd-extensions

Particle::Particle(size_t _numparticles)
{
  m_x.reset(_numparticles);
  m_y.reset(_numparticles);
  m_z.reset(_numparticles);

  m_vx.reset(_numparticles);
  m_vy.reset(_numparticles);
  m_vz.reset(_numparticles);

  m_ax.reset(_numparticles);
  m_ay.reset(_numparticles);
  m_az.reset(_numparticles);

  m_energy.reset(_numparticles);
  m_alive.reset(_numparticles);
}

Particle::Particle(const Particle &_p)
{
  m_x=_p.m_x;
  m_y=_p.m_y;
  m_z=_p.m_z;
  m_ax=_p.m_ax;
  m_ay=_p.m_ay;
  m_az=_p.m_az;
  m_vx=_p.m_vx;
  m_vy=_p.m_vy;
  m_vz=_p.m_vz;
  m_energy=_p.m_energy;
  m_alive=_p.m_alive;

}
