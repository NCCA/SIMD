#ifndef PARTICLE_SYSTEMAOS_H_
#define PARTICLE_SYSTEMAOS_H_
// based on code from here https://software.intel.com/en-us/articles/creating-a-particle-system-with-streaming-simd-extensions
#include <ngl/Vec3.h>
#include <memory>
#include <ngl/AbstractVAO.h>
#include "AbstractParticleSystem.h"

struct ParticleAOS
{
  float m_x,m_y,m_z; // position
  float m_vx,m_vy,m_vz; // velocity
  float m_ax,m_ay,m_az; // acceleratons
  float m_energy; // energy
  bool m_alive;
};

class ParticleSystemAOS : public AbstractParticleSystem
{

  public :
    ParticleSystemAOS(size_t numParticles,ngl::Vec3 _pos);
    ~ParticleSystemAOS();
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
    std::unique_ptr<ParticleAOS []>  m_particles;

    float s_gravity=-9.0f;     /**< Gravity value */
    std::unique_ptr<ngl::AbstractVAO> m_vao;


};


#endif
