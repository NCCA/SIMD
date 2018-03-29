#ifndef PARTICLE_SYSTEMSSERAND_H_
#define PARTICLE_SYSTEMSSERAND_H_
// based on code from here https://software.intel.com/en-us/articles/creating-a-particle-system-with-streaming-simd-extensions
#include "Particle.h"
#include "AbstractParticleSystem.h"
#include <ngl/Vec3.h>

#include <ngl/AbstractVAO.h>

class ParticleSystemSSERAND : public AbstractParticleSystem
{

  public :
    ParticleSystemSSERAND(size_t numParticles,ngl::Vec3 _pos);
    ~ParticleSystemSSERAND();
    void update(float _elapsed) override;
    void updateFMA(float _elapsed);
    void render() override;
    void setNumParticles(size_t numParticles) override;
    void setDefaults() override;
    void setParticleDefaults(size_t particleIndex) override;
    inline bool isAlive() const    { return (m_numAlive > 0); }
    inline size_t getNumAlive() const { return m_numAlive;       }
  private :
    size_t m_numAlive=0;
    size_t m_numParticles;
    std::unique_ptr<Particle>  m_particles;

    float s_gravity=-9.0f;     /**< Gravity value */
    std::unique_ptr<ngl::AbstractVAO> m_vao;


};


#endif
