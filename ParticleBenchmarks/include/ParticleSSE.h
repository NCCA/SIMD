#ifndef PARTICLESSE_H_
#define PARTICLESSE_H_
// based on code from here https://software.intel.com/en-us/articles/creating-a-particle-system-with-streaming-simd-extensions

#include "AlignedArray.h"
class ParticleSSE
{
  public :
    ParticleSSE(size_t _numParticles);
    ParticleSSE(const ParticleSSE &_p);
    AlignedArrayRAII<float,16> m_x,m_y,m_z; // position
    AlignedArrayRAII<float,16> m_vx,m_vy,m_vz; // velocity
    AlignedArrayRAII<float,16> m_ax,m_ay,m_az; // acceleratons
    AlignedArrayRAII<float,16> m_energy; // energy
    AlignedArrayRAII<bool,16> m_alive;

};


#endif
