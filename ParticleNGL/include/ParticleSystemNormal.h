#ifndef PARTICLE_SYSTEMNORMAL_H_
#define PARTICLE_SYSTEMNORMAL_H_
#include "AbstractParticleSystem.h"
// based on code from here https://software.intel.com/en-us/articles/creating-a-particle-system-with-streaming-simd-extensions
#include <ngl/Vec3.h>
#include "Particle.h"
#include <memory>
#include <ngl/AbstractVAO.h>
class ParticleSystemNormal : public AbstractParticleSystem
{

  public :
    ParticleSystemNormal(size_t numParticles,ngl::Vec3 _pos);
    ~ParticleSystemNormal();
    void update(float _elapsed);
    void render();
    void setNumParticles(size_t numParticles);
    void setDefaults();
    void setParticleDefaults(size_t particleIndex);
    inline bool isAlive() const    { return (m_numAlive > 0); }
    inline size_t getNumAlive() const { return m_numAlive;       }

  private :
    size_t m_numAlive=0;
    size_t m_numParticles;
    std::unique_ptr<Particle>  m_particles;

    float s_gravity=-9.0f;     /**< Gravity value */
    float m_energyRange=1.0f;
    std::unique_ptr<ngl::AbstractVAO> m_vao;



};


#endif
