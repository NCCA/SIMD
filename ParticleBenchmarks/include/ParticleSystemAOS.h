#ifndef PARTICLE_SYSTEMAOS_H_
#define PARTICLE_SYSTEMAOS_H_
// based on code from here https://software.intel.com/en-us/articles/creating-a-particle-system-with-streaming-simd-extensions
#include <ngl/Vec3.h>
#include <memory>

struct Particle
{
  float m_x,m_y,m_z; // position
  float m_vx,m_vy,m_vz; // velocity
  float m_ax,m_ay,m_az; // acceleratons
  float m_energy; // energy
  bool m_alive;
};

class ParticleSystemAOS
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
    void updatePosition(float _dx, float _dy, float _dz);

    void updateEnergy(float _dt){m_energyRange+=_dt;}

  private :
    size_t m_numAlive=0;
    size_t m_numParticles;
    std::unique_ptr<Particle []>  m_particles;

    ngl::Vec3 m_pos;
    float s_gravity=-9.0f;     /**< Gravity value */
    float m_energyRange=1.0f;


};


#endif
