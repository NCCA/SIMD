#ifndef PARTICLE_SYSTEMSAVX2_H_
#define PARTICLE_SYSTEMSAVX2_H_
// based on code from here https://software.intel.com/en-us/articles/creating-a-particle-system-with-streaming-simd-extensions
#include "Particle.h"
#include <ngl/AbstractVAO.h>
#include <ngl/Vec3.h>
#include <memory>
#include "AbstractParticleSystem.h"
class ParticleSystemAVX2 : public AbstractParticleSystem
{

  public :
    ParticleSystemAVX2(size_t numParticles,ngl::Vec3 _pos);
    ~ParticleSystemAVX2();
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
