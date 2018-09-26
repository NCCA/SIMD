#include "ParticleSystemAVX2.h"
#include <cassert>
#include <iostream>
#include <ngl/VAOFactory.h>
#include <ngl/SimpleVAO.h>
#include <ngl/NGLStream.h>
#include <ngl/Random.h>
#include <vector>
// based on code from here https://software.intel.com/en-us/articles/creating-a-particle-system-with-streaming-simd-extensions

ParticleSystemAVX2::ParticleSystemAVX2(size_t _numParticles,ngl::Vec3 _pos)
{
  m_numParticles=_numParticles;
  m_particles.reset(new Particle(_numParticles));
  m_pos=_pos;
  setDefaults();
  m_vao=ngl::VAOFactory::createVAO(ngl::simpleVAO,GL_POINTS);
  m_vao->bind();
  std::vector<ngl::Vec3> data(_numParticles);
  m_vao->setData( ngl::SimpleVAO::VertexData(m_numParticles*sizeof(ngl::Vec3),data[0].m_x));
  // We must do this each time as we change the data.
  m_vao->setVertexAttributePointer(0,3,GL_FLOAT,0,0);
  m_vao->setNumIndices(m_numParticles);
  m_vao->unbind();

}

ParticleSystemAVX2::~ParticleSystemAVX2()
{

}




void ParticleSystemAVX2::setDefaults()
{
  size_t i;
  size_t remaining_particles = m_numParticles % 8;
  ALIGNED(32) float rnd[8];
  const f256 startX=splat8f(m_pos.m_x);
  const f256 startY=splat8f(m_pos.m_y);
  const f256 startZ=splat8f(m_pos.m_z);
  ngl::Random *rng=ngl::Random::instance();

  for (i = 0; i < m_numParticles - remaining_particles; i+=8)
  {
    store8f(&m_particles->m_x[i], startX);
    store8f(&m_particles->m_y[i], startY);
    store8f(&m_particles->m_z[i], startZ);
    for( size_t r=0; r<8; ++r)
     rnd[r] = rng->randomNumber(1.0f);
    store8f(&m_particles->m_vx[i], load8f(&rnd[0]));
    store8f(&m_particles->m_vy[i], splat8f(4.0f));
    for( size_t r=0; r<8; ++r)
     rnd[r] = rng->randomNumber(1.0f);

    store8f(&m_particles->m_vz[i], load8f(&rnd[0]));

    store8f(&m_particles->m_ax[i],splat8f(rng->randomNumber(1.0f)));
    store8f(&m_particles->m_ay[i],splat8f( -rng->randomPositiveNumber(1.0f)));
    store8f(&m_particles->m_az[i],splat8f(rng->randomNumber(1.0f)));

    m_particles->m_energy[ i ] = rng->randomPositiveNumber(m_energyRange);
    m_particles->m_energy[i+1] = rng->randomPositiveNumber(m_energyRange);
    m_particles->m_energy[i+2] = rng->randomPositiveNumber(m_energyRange);
    m_particles->m_energy[i+3] = rng->randomPositiveNumber(m_energyRange);
    m_particles->m_energy[i+4] = rng->randomPositiveNumber(m_energyRange);
    m_particles->m_energy[i+5] = rng->randomPositiveNumber(m_energyRange);
    m_particles->m_energy[i+6] = rng->randomPositiveNumber(m_energyRange);
    m_particles->m_energy[i+7] = rng->randomPositiveNumber(m_energyRange);

    m_particles->m_alive[ i ] = true;
    m_particles->m_alive[i+1] = true;
    m_particles->m_alive[i+2] = true;
    m_particles->m_alive[i+3] = true;
    m_particles->m_alive[i+4] = true;
    m_particles->m_alive[i+5] = true;
    m_particles->m_alive[i+6] = true;
    m_particles->m_alive[i+7] = true;
  }

  if ( remaining_particles!=0)
  {
    i -= (8 - remaining_particles);
    // note use of unaligned stores here due to not being on boundary
    storeu8f(&m_particles->m_x[i], startX);
    storeu8f(&m_particles->m_y[i], startY);
    storeu8f(&m_particles->m_z[i], startZ);
    for( size_t r=0; r<8; ++r)
     rnd[r] = rng->randomNumber(1.0f);

    storeu8f(&m_particles->m_vx[i], load8f(&rnd[0]));
    storeu8f(&m_particles->m_vy[i], splat8f(4.0f));
    for( size_t r=0; r<8; ++r)
     rnd[r] = rng->randomNumber(1.0f);

    storeu8f(&m_particles->m_vz[i], load8f(&rnd[0]));

    storeu8f(&m_particles->m_ax[i],splat8f( rng->randomPositiveNumber(1.0f)));
    storeu8f(&m_particles->m_ay[i],splat8f( -rng->randomPositiveNumber(1.0f)));
    storeu8f(&m_particles->m_az[i],splat8f( rng->randomPositiveNumber(1.0f)));

    m_particles->m_energy[ i ] = rng->randomPositiveNumber(m_energyRange);
    m_particles->m_energy[i+1] = rng->randomPositiveNumber(m_energyRange);
    m_particles->m_energy[i+2] = rng->randomPositiveNumber(m_energyRange);
    m_particles->m_energy[i+3] = rng->randomPositiveNumber(m_energyRange);
    m_particles->m_energy[i+4] = rng->randomPositiveNumber(m_energyRange);
    m_particles->m_energy[i+5] = rng->randomPositiveNumber(m_energyRange);
    m_particles->m_energy[i+6] = rng->randomPositiveNumber(m_energyRange);
    m_particles->m_energy[i+7] = rng->randomPositiveNumber(m_energyRange);

    m_particles->m_alive[ i ] = true;
    m_particles->m_alive[i+1] = true;
    m_particles->m_alive[i+2] = true;
    m_particles->m_alive[i+3] = true;
    m_particles->m_alive[i+4] = true;
    m_particles->m_alive[i+5] = true;
    m_particles->m_alive[i+6] = true;
    m_particles->m_alive[i+7] = true;
  }

}

void ParticleSystemAVX2::setNumParticles(size_t _numParticles)
{


}



void ParticleSystemAVX2::update(float _elapsed)
{
  f256 vel_x, vel_y, vel_z;
  f256 frame_time = splat8f(_elapsed);
  f256 gravity    = splat8f(s_gravity * _elapsed);
  const f128 ZERO = zero4f();


  for (size_t i = 0; i < m_numParticles; i+=8)
  {
    // velocity = velocity + (time * acceleration)

    vel_x=fmadd8f(frame_time,load8f(&m_particles->m_ax[i]),load8f(&m_particles->m_vx[i]));
    vel_y=fmadd8f(frame_time,load8f(&m_particles->m_ay[i]),load8f(&m_particles->m_vy[i]));
    vel_z=fmadd8f(frame_time,load8f(&m_particles->m_az[i]),load8f(&m_particles->m_vz[i]));

    // Y velocity = Y velocity + (gravity * time)
    vel_y = add8f(vel_y, gravity);

    // p = p + v * t
    store8f(&m_particles->m_x[i],fmadd8f(vel_x, frame_time,load8f(&m_particles->m_x[i])));
    store8f(&m_particles->m_y[i],fmadd8f(vel_y, frame_time,load8f(&m_particles->m_y[i])));
    store8f(&m_particles->m_z[i],fmadd8f(vel_z, frame_time,load8f(&m_particles->m_z[i])));

    store8f(&m_particles->m_vx[i], vel_x);
    store8f(&m_particles->m_vy[i], vel_y);
    store8f(&m_particles->m_vz[i], vel_z);

    // energy = energy - time
    store8f(&m_particles->m_energy[i], sub8f(load8f(&m_particles->m_energy[i]), frame_time));
    // just to make code simpler I'm going to split the 8 floats into two
    auto energy=load8f(&m_particles->m_energy[i]);
    f128 hi=cast8fHi(energy);
    f128 lo=cast8fLow(energy);
    //size_t e=0;
    for(size_t e=0; e<2; ++e)
    {
      int32_t sw;
      if(e==0)
        sw=movemask4f(cmplteq4f(hi, ZERO));
      else
        sw=movemask4f(cmplteq4f(lo, ZERO));
      //std::cout<<"sw "<<sw<<'\n';
      switch (sw)
      {
      case 0: // f f f f
        // do nothing; all 4 particles are alive
        break;
      case 1: // f f f t
        // particle [i] is dead
        setParticleDefaults((e*4)+i);
        break;
      case 2: // f f t f
        // particle [i+1] is dead
        setParticleDefaults((e*4)+i+1);
        break;
      case 3: // f f t t
        // particles [i] and [i+1] are dead
        setParticleDefaults((e*4)+i);
        setParticleDefaults((e*4)+i+1);
        break;
      case 4: // f t f f
        // particle [i+2] is dead
        setParticleDefaults((e*4)+i+2);
        break;
      case 5: // f t f t
        // particles [i] and [i+2] are dead
        setParticleDefaults((e*4)+i);
        setParticleDefaults((e*4)+i+2);
        break;
      case 6: // f t t f
        // particles [i+1] and [i+2] are dead
        setParticleDefaults((e*4)+i+1);
        setParticleDefaults((e*4)+i+2);
        break;
      case 7: // f t t t
        // particles [i] and [i+1] and [i+2] are dead
        setParticleDefaults((e*4)+i);
        setParticleDefaults((e*4)+i+1);
        setParticleDefaults((e*4)+i+2);
        break;
      case 8: // t f f f
        // particle [i+3] is dead
        setParticleDefaults((e*4)+i+3);
        break;
      case 9: // t f f t
        // particles [i] and [i+3] are dead
        setParticleDefaults((e*4)+i);
        setParticleDefaults((e*4)+i+3);
        break;
      case 10: // t f t f
        // particles [i+1] and [i+3] are dead
        setParticleDefaults((e*4)+i+1);
        setParticleDefaults((e*4)+i+3);
        break;
      case 11: // t f t t
        // particles [i] and [i+1] and [i+3] are dead
        setParticleDefaults((e*4)+i);
        setParticleDefaults((e*4)+i+1);
        setParticleDefaults((e*4)+i+3);
        break;
      case 12: // t t f f
        // particles [i+2] and [i+3] are dead
        setParticleDefaults((e*4)+i+2);
        setParticleDefaults((e*4)+i+3);
        break;
      case 13: // t t f t
        // particles [i] and [i+2] and [i+3] are dead
        setParticleDefaults((e*4)+i);
        setParticleDefaults((e*4)+i+2);
        setParticleDefaults((e*4)+i+3);
        break;
      case 14: // t t t f
        // particles [i+1] and [i+2] and [i+3] are dead
        setParticleDefaults((e*4)+i+1);
        setParticleDefaults((e*4)+i+2);
        setParticleDefaults((e*4)+i+3);
        break;
      case 15: // t t t t
        // all 4 particles are dead
        setParticleDefaults((e*4)+i);
        setParticleDefaults((e*4)+i+1);
        setParticleDefaults((e*4)+i+2);
        setParticleDefaults((e*4)+i+3);
        break;

      default:
        std::cout<<"Beyond bounds! \n" ;
        break;
  }
 }

    // do collision
    // stretch particles
    // paritcle has light
  }
}

void ParticleSystemAVX2::setParticleDefaults(size_t particleIndex)
{
  ngl::Random *rng=ngl::Random::instance();
  m_particles->m_x[particleIndex]       = m_pos.m_x;
  m_particles->m_y[particleIndex]       = m_pos.m_y;
  m_particles->m_z[particleIndex]       = m_pos.m_z;
  ngl::Vec3 dir=rng->getRandomNormalizedVec3();
  m_particles->m_vx[particleIndex]      = dir.m_x;
  m_particles->m_vy[particleIndex]      = rng->randomPositiveNumber(1.0f) * 6.0f + 4.0f;
  m_particles->m_vz[particleIndex]      = dir.m_y;

  m_particles->m_ax[particleIndex]      = rng->randomPositiveNumber(1.0f);
  m_particles->m_ay[particleIndex]      = -rng->randomPositiveNumber(1.0f);
  m_particles->m_az[particleIndex]      = rng->randomPositiveNumber(1.0f);

  m_particles->m_energy[particleIndex]  = rng->randomPositiveNumber(m_energyRange);

  m_particles->m_alive[particleIndex]   = true;
}

void ParticleSystemAVX2::render()
{

 f256 xmm0, xmm1, xmm2, xmm3, xmm4, xmm5, xmm6, xmm7;
 size_t remaining_particles = m_numParticles % 8;

 size_t i = 0;
 ngl::Vec3 *verts=reinterpret_cast<ngl::Vec3 *> (m_vao->mapBuffer());

 // lambda to set the vertex from a register
 auto setVert=[&verts](f256 reg)
 {
   ALIGNED(32)float v[8];
   store8f(v, reg);
   verts->m_x = v[0];
   verts->m_y = v[1];
   verts->m_z = v[2];
   ++verts;

 };

 const f256 ONE = splat8f(1.0f);


 for (i = 0; i < m_numParticles - remaining_particles; i+=4)
 {
   xmm0 = load8f(&m_particles->m_x[i]);                     // xmm0: x[i+3]  x[i+2]  x[i+1]  x[ i ]
   xmm1 = load8f(&m_particles->m_y[i]);                     // xmm1: y[i+3]  y[i+2]  y[i+1]  y[ i ]
   xmm2 = load8f(&m_particles->m_z[i]);                     // xmm2: z[i+3]  z[i+2]  z[i+1]  z[ i ]
   xmm3 = ONE;                                              // xmm3:   1.0     1.0     1.0     1.0

   xmm4 = unpacklo8f(xmm0, xmm1);                         // xmm4: y[i+1]  x[i+1]  y[ i ]  x[ i ]
   xmm6 = unpackhi8f(xmm0, xmm1);                         // xmm6: y[i+3]  x[i+3]  y[i+2]  x[i+2]
   xmm5 = unpacklo8f(xmm2, xmm3);                         // xmm5:   1.0   z[i+1]    1.0   z[ i ]
   xmm7 = unpackhi8f(xmm2, xmm3);                         // xmm7:   1.0   z[i+3]    1.0   z[i+2]

   xmm0 = shuffle8f(xmm4, xmm5, 1, 0, 1, 0); // xmm0:   1.0   z[ i ]  y[ i ]  x[ i ]
   xmm1 = shuffle8f(xmm4, xmm5, 3, 2, 3, 2); // xmm1:   1.0   z[i+1]  y[i+1]  x[i+1]
   xmm2 = shuffle8f(xmm6, xmm7, 1, 0, 1, 0); // xmm2:   1.0   z[i+2]  y[i+2]  x[i+2]
   xmm3 = shuffle8f(xmm6, xmm7, 3, 2, 3, 2); // xmm3:   1.0   z[i+3]  y[i+3]  x[i+3]

   if (m_particles->m_alive[i])
   {
     setVert(xmm0);
   }

   if (m_particles->m_alive[i+1])
   {
     setVert(xmm1);
   }

   if (m_particles->m_alive[i+2])
   {
     setVert(xmm2);
   }

   if (m_particles->m_alive[i+3])
   {
     setVert(xmm3);
   }
 }


 // Complete filling the vertex buffer with the remaining particles
 if (0 != remaining_particles)
 {
   i -= (4 - remaining_particles);
   // Note use of unaligned loads here as not on boundary!
   //verts -= (4 - remaining_particles);                       //         r3      r2      r1      r0                                                               //       ------------------------------
   xmm0 = loadu8f(&m_particles->m_x[i]);                     // xmm0: x[i+3]  x[i+2]  x[i+1]  x[ i ]
   xmm1 = loadu8f(&m_particles->m_y[i]);                     // xmm1: y[i+3]  y[i+2]  y[i+1]  y[ i ]
   xmm2 = loadu8f(&m_particles->m_z[i]);                     // xmm2: z[i+3]  z[i+2]  z[i+1]  z[ i ]
   xmm3 = ONE;                                              // xmm3:   1.0     1.0     1.0     1.0

   xmm4 = unpacklo8f(xmm0, xmm1);                         // xmm4: y[i+1]  x[i+1]  y[ i ]  x[ i ]
   xmm6 = unpackhi8f(xmm0, xmm1);                         // xmm6: y[i+3]  x[i+3]  y[i+2]  x[i+2]
   xmm5 = unpacklo8f(xmm2, xmm3);                         // xmm5:   1.0   z[i+1]    1.0   z[ i ]
   xmm7 = unpackhi8f(xmm2, xmm3);                         // xmm7:   1.0   z[i+3]    1.0   z[i+2]

   xmm0 = shuffle8f(xmm4, xmm5, 1, 0, 1, 0); // xmm0:   1.0   z[ i ]  y[ i ]  x[ i ]
   xmm1 = shuffle8f(xmm4, xmm5, 3, 2, 3, 2); // xmm1:   1.0   z[i+1]  y[i+1]  x[i+1]
   xmm2 = shuffle8f(xmm6, xmm7, 1, 0, 1, 0); // xmm2:   1.0   z[i+2]  y[i+2]  x[i+2]
   xmm3 = shuffle8f(xmm6, xmm7, 3, 2, 3, 2); // xmm3:   1.0   z[i+3]  y[i+3]  x[i+3]


   if (m_particles->m_alive[i])
   {
     setVert(xmm0);
   }

   if (m_particles->m_alive[i+1])
   {
     setVert(xmm1);
   }

   if (m_particles->m_alive[i+2])
   {
     setVert(xmm2);
   }

   if (m_particles->m_alive[i+3])
   {
     setVert(xmm3);
   }
 }

  m_vao->unmapBuffer();
  // now render
  m_vao->bind();
  m_vao->draw();
  m_vao->unbind();
}




