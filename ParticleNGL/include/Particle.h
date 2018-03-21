#ifndef PARTICLE_H_
#define PARTICLE_H_
// based on code from here https://software.intel.com/en-us/articles/creating-a-particle-system-with-streaming-simd-extensions

#include "AlignedArray.h"
class Particle
{
  public :
    Particle(size_t _numParticles);
    Particle(const Particle &_p);
    AlignedArrayRAII<float,16> m_x,m_y,m_z; // position
    AlignedArrayRAII<float,16> m_vx,m_vy,m_vz; // velocity
    AlignedArrayRAII<float,16> m_ax,m_ay,m_az; // acceleratons
    AlignedArrayRAII<float,16> m_energy; // energy
    AlignedArrayRAII<bool,16> m_alive;

};


#endif
