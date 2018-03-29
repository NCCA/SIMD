#ifndef PARTICLE_SYSTEMSSERAND_H_
#define PARTICLE_SYSTEMSSERAND_H_
// based on code from here https://software.intel.com/en-us/articles/creating-a-particle-system-with-streaming-simd-extensions
#include "ParticleSSE.h"
#include <ngl/Vec3.h>

#include <ngl/AbstractVAO.h>

class ParticleSystemSSERAND
{

  public :
    ParticleSystemSSERAND(size_t numParticles,ngl::Vec3 _pos);
    ~ParticleSystemSSERAND();
    void update(float _elapsed) ;
    void updateFMA(float _elapsed);
    void render() ;
    void setNumParticles(size_t numParticles) ;
    void setDefaults() ;
    void setParticleDefaults(size_t particleIndex) ;
    inline bool isAlive() const    { return (m_numAlive > 0); }
    inline size_t getNumAlive() const { return m_numAlive;       }
  private :
    size_t m_numAlive=0;
    size_t m_numParticles;
    std::unique_ptr<ParticleSSE>  m_particles;
    ngl::Vec3 m_pos;
    float s_gravity=-9.0f;     /**< Gravity value */


};


#endif
