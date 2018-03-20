#include "ParticleSystem.h"
#include <cassert>
#include <iostream>
#include <ngl/VAOFactory.h>
#include <ngl/SimpleVAO.h>
#include <ngl/NGLStream.h>
#include <ngl/Random.h>
#include <vector>

ParticleSystem::ParticleSystem(size_t _numParticles,ngl::Vec3 _pos)
{
  m_numParticles=_numParticles;
  m_particles.reset(new Particle(_numParticles));
  m_pos=_pos;
  setDefaults();
  m_vao.reset( ngl::VAOFactory::createVAO(ngl::simpleVAO,GL_POINTS));
  m_vao->bind();
  std::vector<ngl::Vec3> data(_numParticles);
  m_vao->setData( ngl::SimpleVAO::VertexData(m_numParticles*sizeof(ngl::Vec3),data[0].m_x));
  // We must do this each time as we change the data.
  m_vao->setVertexAttributePointer(0,3,GL_FLOAT,0,0);
  m_vao->setNumIndices(m_numParticles);
  m_vao->unbind();


}

ParticleSystem::~ParticleSystem()
{

}

int random_dir()
{
  if (rand() > RAND_MAX / 2)
    return 1;
  else
    return -1;
}

void ParticleSystem::setDefaults()
{
  size_t i;
  size_t remaining_particles = m_numParticles % 4;
  ALIGNED(16) float rnd[4];
//  const f128 ZERO = set4f(m_pos.m_x,m_pos.m_y,m_pos.z);///zero4f();
  const f128 startX=splat4f(m_pos.m_x);
  const f128 startY=splat4f(m_pos.m_y);
  const f128 startZ=splat4f(m_pos.m_z);

  for (i = 0; i < m_numParticles - remaining_particles; i+=4)
  {
    store4f(&m_particles->m_x[i], startX);
    store4f(&m_particles->m_y[i], startY);
    store4f(&m_particles->m_z[i], startZ);

    rnd[0] = (float)rand() / (float)RAND_MAX * random_dir();
    rnd[1] = (float)rand() / (float)RAND_MAX * random_dir();
    rnd[2] = (float)rand() / (float)RAND_MAX * random_dir();
    rnd[3] = (float)rand() / (float)RAND_MAX * random_dir();
    store4f(&m_particles->m_vx[i], load4f(&rnd[0]));
    store4f(&m_particles->m_vy[i], splat4f(4.0f));
    rnd[0] = (float)rand() / (float)RAND_MAX * random_dir();
    rnd[1] = (float)rand() / (float)RAND_MAX * random_dir();
    rnd[2] = (float)rand() / (float)RAND_MAX * random_dir();
    rnd[3] = (float)rand() / (float)RAND_MAX * random_dir();
    store4f(&m_particles->m_vz[i], load4f(&rnd[0]));

    store4f(&m_particles->m_ax[i],splat4f((float)rand() / (float)RAND_MAX));
    store4f(&m_particles->m_ay[i],splat4f(-1.0f * (float)rand() / (float)RAND_MAX));
    store4f(&m_particles->m_az[i],splat4f((float)rand() / (float)RAND_MAX));

    m_particles->m_energy[ i ] = (float)rand() / (float)RAND_MAX * 3.0f;
    m_particles->m_energy[i+1] = (float)rand() / (float)RAND_MAX * 3.0f;
    m_particles->m_energy[i+2] = (float)rand() / (float)RAND_MAX * 3.0f;
    m_particles->m_energy[i+3] = (float)rand() / (float)RAND_MAX * 3.0f;

    m_particles->m_alive[ i ] = true;
    m_particles->m_alive[i+1] = true;
    m_particles->m_alive[i+2] = true;
    m_particles->m_alive[i+3] = true;
  }

  if (0 != remaining_particles)
  {
    i -= (4 - remaining_particles);

    store4f(&m_particles->m_x[i], startX);
    store4f(&m_particles->m_y[i], startY);
    store4f(&m_particles->m_z[i], startZ);

    rnd[0] = (float)rand() / (float)RAND_MAX * random_dir();
    rnd[1] = (float)rand() / (float)RAND_MAX * random_dir();
    rnd[2] = (float)rand() / (float)RAND_MAX * random_dir();
    rnd[3] = (float)rand() / (float)RAND_MAX * random_dir();
    store4f(&m_particles->m_vx[i], load4f(&rnd[0]));
    store4f(&m_particles->m_vy[i], splat4f(4.0f));
    rnd[0] = (float)rand() / (float)RAND_MAX * random_dir();
    rnd[1] = (float)rand() / (float)RAND_MAX * random_dir();
    rnd[2] = (float)rand() / (float)RAND_MAX * random_dir();
    rnd[3] = (float)rand() / (float)RAND_MAX * random_dir();
    store4f(&m_particles->m_vz[i], load4f(&rnd[0]));

    store4f(&m_particles->m_ax[i],splat4f(0));//(float)rand() / (float)RAND_MAX));
    store4f(&m_particles->m_ay[i],splat4f(-1.0f));// * (float)rand() / (float)RAND_MAX));
    store4f(&m_particles->m_az[i],splat4f(0));//(float)rand() / (float)RAND_MAX));

    m_particles->m_energy[ i ] = (float)rand() / (float)RAND_MAX * 3.0f;
    m_particles->m_energy[i+1] = (float)rand() / (float)RAND_MAX * 3.0f;
    m_particles->m_energy[i+2] = (float)rand() / (float)RAND_MAX * 3.0f;
    m_particles->m_energy[i+3] = (float)rand() / (float)RAND_MAX * 3.0f;

    m_particles->m_alive[ i ] = true;
    m_particles->m_alive[i+1] = true;
    m_particles->m_alive[i+2] = true;
    m_particles->m_alive[i+3] = true;
  }

}

void ParticleSystem::setNumParticles(size_t _numParticles)
{


}



void ParticleSystem::update(float _elapsed)
{
  f128 vel_x, vel_y, vel_z;
  f128 frame_time = splat4f(_elapsed);
  f128 gravity    = splat4f(s_gravity * _elapsed);
  const f128 ZERO = zero4f();


  for (size_t i = 0; i < m_numParticles; i+=4)
  {
    // velocity = velocity + (time * acceleration)
    //vel_x = add4f(load4f(&m_particles->m_vx[i]), mul4f(frame_time, load4f(&m_particles->m_ax[i])));
    vel_x=fmadd4f(frame_time,load4f(&m_particles->m_ax[i]),load4f(&m_particles->m_vx[i]));

    vel_y = add4f(load4f(&m_particles->m_vy[i]), mul4f(frame_time, load4f(&m_particles->m_ay[i])));
    vel_z = add4f(load4f(&m_particles->m_vz[i]), mul4f(frame_time, load4f(&m_particles->m_az[i])));

    // Y velocity = Y velocity + (gravity * time)
    vel_y = add4f(vel_y, gravity);

    // p = p + v * t
    store4f(&m_particles->m_x[i], add4f(load4f(&m_particles->m_x[i]), mul4f(vel_x, frame_time)));
    store4f(&m_particles->m_y[i], add4f(load4f(&m_particles->m_y[i]), mul4f(vel_y, frame_time)));
    store4f(&m_particles->m_z[i], add4f(load4f(&m_particles->m_z[i]), mul4f(vel_z, frame_time)));

    store4f(&m_particles->m_vx[i], vel_x);
    store4f(&m_particles->m_vy[i], vel_y);
    store4f(&m_particles->m_vz[i], vel_z);

    // energy = energy - time
    store4f(&m_particles->m_energy[i], sub4f(load4f(&m_particles->m_energy[i]), frame_time));

    if (s_alwaysAlive)
    {
      switch (movemask4f(cmplteq4f(load4f(&m_particles->m_energy[i]), ZERO)))
      {
      case 0: // f f f f
        // do nothing; all 4 particles are alive
        break;
      case 1: // f f f t
        // particle [i] is dead
        setParticleDefaults(i);
        break;
      case 2: // f f t f
        // particle [i+1] is dead
        setParticleDefaults(i+1);
        break;
      case 3: // f f t t
        // particles [i] and [i+1] are dead
        setParticleDefaults(i);
        setParticleDefaults(i+1);
        break;
      case 4: // f t f f
        // particle [i+2] is dead
        setParticleDefaults(i+2);
        break;
      case 5: // f t f t
        // particles [i] and [i+2] are dead
        setParticleDefaults(i);
        setParticleDefaults(i+2);
        break;
      case 6: // f t t f
        // particles [i+1] and [i+2] are dead
        setParticleDefaults(i+1);
        setParticleDefaults(i+2);
        break;
      case 7: // f t t t
        // particles [i] and [i+1] and [i+2] are dead
        setParticleDefaults(i);
        setParticleDefaults(i+1);
        setParticleDefaults(i+2);
        break;
      case 8: // t f f f
        // particle [i+3] is dead
        setParticleDefaults(i+3);
        break;
      case 9: // t f f t
        // particles [i] and [i+3] are dead
        setParticleDefaults(i);
        setParticleDefaults(i+3);
        break;
      case 10: // t f t f
        // particles [i+1] and [i+3] are dead
        setParticleDefaults(i+1);
        setParticleDefaults(i+3);
        break;
      case 11: // t f t t
        // particles [i] and [i+1] and [i+3] are dead
        setParticleDefaults(i);
        setParticleDefaults(i+1);
        setParticleDefaults(i+3);
        break;
      case 12: // t t f f
        // particles [i+2] and [i+3] are dead
        setParticleDefaults(i+2);
        setParticleDefaults(i+3);
        break;
      case 13: // t t f t
        // particles [i] and [i+2] and [i+3] are dead
        setParticleDefaults(i);
        setParticleDefaults(i+2);
        setParticleDefaults(i+3);
        break;
      case 14: // t t t f
        // particles [i+1] and [i+2] and [i+3] are dead
        setParticleDefaults(i+1);
        setParticleDefaults(i+2);
        setParticleDefaults(i+3);
        break;
      case 15: // t t t t
        // all 4 particles are dead
        setParticleDefaults(i);
        setParticleDefaults(i+1);
        setParticleDefaults(i+2);
        setParticleDefaults(i+3);
        break;
      default:
#ifdef _DEBUG
        OutputDebugString("No matches!\n");
#endif // _DEBUG
        // void
        break;
      }
    }
    else // !mInfo.alwaysAlive == true
    {
      switch (movemask4f(cmplteq4f(load4f(&m_particles->m_energy[i]), ZERO)))
      {
      case 0: // f f f f
        // do nothing; all 4 particles are alive
        break;
      case 1: // f f f t
        // particle [i] is dead
        m_particles->m_alive[i] = false;
        m_numAlive -= 1;
        break;
      case 2: // f f t f
        // particle [i+1] is dead
        m_particles->m_alive[i+1] = false;
        m_numAlive -= 1;
        break;
      case 3: // f f t t
        // particles [i] and [i+1] are dead
        m_particles->m_alive[ i ] = false;
        m_particles->m_alive[i+1] = false;
        m_numAlive -= 2;
        break;
      case 4: // f t f f
        // particle [i+2] is dead
        m_particles->m_alive[i+2] = false;
        m_numAlive -= 1;
        break;
      case 5: // f t f t
        // particles [i] and [i+2] are dead
        m_particles->m_alive[ i ] = false;
        m_particles->m_alive[i+2] = false;
        m_numAlive -= 2;
        break;
      case 6: // f t t f
        // particles [i+1] and [i+2] are dead
        m_particles->m_alive[i+1] = false;
        m_particles->m_alive[i+2] = false;
        m_numAlive -= 2;
        break;
      case 7: // f t t t
        // particles [i] and [i+1] and [i+2] are dead
        m_particles->m_alive[ i ] = false;
        m_particles->m_alive[i+1] = false;
        m_particles->m_alive[i+2] = false;
        m_numAlive -= 3;
        break;
      case 8: // t f f f
        // particle [i+3] is dead
        m_particles->m_alive[i+3] = false;
        m_numAlive -= 1;
        break;
      case 9: // t f f t
        // particles [i] and [i+3] are dead
        m_particles->m_alive[ i ] = false;
        m_particles->m_alive[i+3] = false;
        m_numAlive -= 2;
        break;
      case 10: // t f t f
        // particles [i+1] and [i+3] are dead
        m_particles->m_alive[i+1] = false;
        m_particles->m_alive[i+3] = false;
        m_numAlive -= 2;
        break;
      case 11: // t f t t
        // particles [i] and [i+1] and [i+3] are dead
        m_particles->m_alive[ i ] = false;
        m_particles->m_alive[i+1] = false;
        m_particles->m_alive[i+3] = false;
        m_numAlive -= 3;
        break;
      case 12: // t t f f
        // particles [i+2] and [i+3] are dead
        m_particles->m_alive[i+2] = false;
        m_particles->m_alive[i+3] = false;
        m_numAlive -= 2;
        break;
      case 13: // t t f t
        // particles [i] and [i+2] and [i+3] are dead
        m_particles->m_alive[ i ] = false;
        m_particles->m_alive[i+2] = false;
        m_particles->m_alive[i+3] = false;
        m_numAlive -= 3;
        break;
      case 14: // t t t f
        // particles [i+1] and [i+2] and [i+3] are dead
        m_particles->m_alive[i+1] = false;
        m_particles->m_alive[i+2] = false;
        m_particles->m_alive[i+3] = false;
        m_numAlive -= 3;
        break;
      case 15: // t t t t
        // all 4 particles are dead
        m_particles->m_alive[ i ] = false;
        m_particles->m_alive[i+1] = false;
        m_particles->m_alive[i+2] = false;
        m_particles->m_alive[i+3] = false;
        m_numAlive -= 4;
        break;
      default:
#ifdef _DEBUG
        OutputDebugString("No matches!\n");
#endif // _DEBUG
        // void
        break;
      }
    }

    // do collision
    // stretch particles
    // paritcle has light
  }
}

void ParticleSystem::setParticleDefaults(size_t particleIndex)
{
  ngl::Random *rng=ngl::Random::instance();
  m_particles->m_x[particleIndex]       = m_pos.m_x;
  m_particles->m_y[particleIndex]       = m_pos.m_y;
  m_particles->m_z[particleIndex]       = m_pos.m_z;
  ngl::Vec3 dir=rng->getRandomNormalizedVec3();
  m_particles->m_vx[particleIndex]      = dir.m_x;
  m_particles->m_vy[particleIndex]      = rng->randomPositiveNumber(1.0f) * 6.0f + 4.0f;
  m_particles->m_vz[particleIndex]      = dir.m_y;

  m_particles->m_ax[particleIndex]      = 0;//(float)rand() / (float)RAND_MAX;
  m_particles->m_ay[particleIndex]      = -1.0f;// * (float)rand() / (float)RAND_MAX;
  m_particles->m_az[particleIndex]      = 0;//(float)rand() / (float)RAND_MAX;

  m_particles->m_energy[particleIndex]  = rng->randomPositiveNumber(1.0f) * 3.0f;

  m_particles->m_alive[particleIndex]   = true;
}

 void ParticleSystem::render()
 {

   f128 xmm0, xmm1, xmm2, xmm3, xmm4, xmm5, xmm6, xmm7;
   ALIGNED(16)float v[4];
   size_t remaining_particles = m_numParticles % 4;
   std::unique_ptr<ngl::Vec3[]> pVertices(new ngl::Vec3[m_numParticles]);
   // Fill vertex buffer with current data
   size_t i = 0;
   ngl::Vec3 *verts=reinterpret_cast<ngl::Vec3 *> (m_vao->mapBuffer());
  // std::cout<<"frame\n";

   for (i = 0; i < m_numParticles - remaining_particles; i+=4)
   {
    // std::cout<<"v "<<*verts<<'\n';                                                          //         r3      r2      r1      r0                                                                 //       ------------------------------
     xmm0 = load4f(&m_particles->m_x[i]);                     // xmm0: x[i+3]  x[i+2]  x[i+1]  x[ i ]
     xmm1 = load4f(&m_particles->m_y[i]);                     // xmm1: y[i+3]  y[i+2]  y[i+1]  y[ i ]
     xmm2 = load4f(&m_particles->m_z[i]);                     // xmm2: z[i+3]  z[i+2]  z[i+1]  z[ i ]
     xmm3 = splat4f(1.0f);                                   // xmm3:   1.0     1.0     1.0     1.0

     xmm4 = unpacklo4f(xmm0, xmm1);                         // xmm4: y[i+1]  x[i+1]  y[ i ]  x[ i ]
     xmm6 = unpackhi4f(xmm0, xmm1);                         // xmm6: y[i+3]  x[i+3]  y[i+2]  x[i+2]
     xmm5 = unpacklo4f(xmm2, xmm3);                         // xmm5:   1.0   z[i+1]    1.0   z[ i ]
     xmm7 = unpackhi4f(xmm2, xmm3);                         // xmm7:   1.0   z[i+3]    1.0   z[i+2]

     xmm0 = shuffle4f(xmm4, xmm5, 1, 0, 1, 0); // xmm0:   1.0   z[ i ]  y[ i ]  x[ i ]
     xmm1 = shuffle4f(xmm4, xmm5, 3, 2, 3, 2); // xmm1:   1.0   z[i+1]  y[i+1]  x[i+1]
     xmm2 = shuffle4f(xmm6, xmm7, 1, 0, 1, 0); // xmm2:   1.0   z[i+2]  y[i+2]  x[i+2]
     xmm3 = shuffle4f(xmm6, xmm7, 3, 2, 3, 2); // xmm3:   1.0   z[i+3]  y[i+3]  x[i+3]

     if (m_particles->m_alive[i])
     {
       store4f(v, xmm0);
       verts->m_x = v[0];
       verts->m_y = v[1];
       verts->m_z = v[2];
       ++verts;
       //std::cout<<"P "<<v[0]<<' '<<v[1]<<' '<<v[2]<<'\n';
     }

     if (m_particles->m_alive[i+1])
     {
       store4f(v, xmm1);
       verts->m_x = v[0];
       verts->m_y = v[1];
       verts->m_z = v[2];
       ++verts;
//       pVertices->v.x = v[0];
//       pVertices->v.y = v[1];
//       pVertices->v.z = v[2];
//       pVertices++;
//       std::cout<<"P "<<v[0]<<' '<<v[1]<<' '<<v[2]<<'\n';

     }

     if (m_particles->m_alive[i+2])
     {
       store4f(v, xmm2);
       verts->m_x = v[0];
       verts->m_y = v[1];
       verts->m_z = v[2];
       ++verts;
//       pVertices->v.x = v[0];
//       pVertices->v.y = v[1];
//       pVertices->v.z = v[2];
//       pVertices++;
//       std::cout<<"P "<<v[0]<<' '<<v[1]<<' '<<v[2]<<'\n';

     }

     if (m_particles->m_alive[i+3])
     {
       store4f(v, xmm3);
       verts->m_x = v[0];
       verts->m_y = v[1];
       verts->m_z = v[2];
       ++verts;
//       pVertices->v.x = v[0];
//       pVertices->v.y = v[1];
//       pVertices->v.z = v[2];
//       pVertices++;
//       std::cout<<"P "<<v[0]<<' '<<v[1]<<' '<<v[2]<<'\n';

     }
   }


   // Complete filling the vertex buffer with the remaining particles
   if (0 != remaining_particles)
   {
     i -= (4 - remaining_particles);
     //pVertices -= (4 - remaining_particles);
                                                                 //         r3      r2      r1      r0
                                                                 //       ------------------------------
     xmm0 = load4f(&m_particles->m_x[i]);                     // xmm0: x[i+3]  x[i+2]  x[i+1]  x[ i ]
     xmm1 = load4f(&m_particles->m_y[i]);                     // xmm1: y[i+3]  y[i+2]  y[i+1]  y[ i ]
     xmm2 = load4f(&m_particles->m_z[i]);                     // xmm2: z[i+3]  z[i+2]  z[i+1]  z[ i ]
     xmm3 = splat4f(1.0f);                                   // xmm3:   1.0     1.0     1.0     1.0

     xmm4 = unpacklo4f(xmm0, xmm1);                         // xmm4: y[i+1]  x[i+1]  y[ i ]  x[ i ]
     xmm6 = unpackhi4f(xmm0, xmm1);                         // xmm6: y[i+3]  x[i+3]  y[i+2]  x[i+2]
     xmm5 = unpacklo4f(xmm2, xmm3);                         // xmm5:   1.0   z[i+1]    1.0   z[ i ]
     xmm7 = unpackhi4f(xmm2, xmm3);                         // xmm7:   1.0   z[i+3]    1.0   z[i+2]

     xmm0 = shuffle4f(xmm4, xmm5, 1, 0, 1, 0); // xmm0:   1.0   z[ i ]  y[ i ]  x[ i ]
     xmm1 = shuffle4f(xmm4, xmm5, 3, 2, 3, 2); // xmm1:   1.0   z[i+1]  y[i+1]  x[i+1]
     xmm2 = shuffle4f(xmm6, xmm7, 1, 0, 1, 0); // xmm2:   1.0   z[i+2]  y[i+2]  x[i+2]
     xmm3 = shuffle4f(xmm6, xmm7, 3, 2, 3, 2); // xmm3:   1.0   z[i+3]  y[i+3]  x[i+3]


     if (m_particles->m_alive[i])
     {
       store4f(v, xmm0);
       verts->m_x = v[0];
       verts->m_y = v[1];
       verts->m_z = v[2];
       ++verts;

       //       pVertices->v.x = v[0];
//       pVertices->v.y = v[1];
//       pVertices->v.z = v[2];
//       pVertices++;
//       std::cout<<"P "<<v[0]<<' '<<v[1]<<' '<<v[2]<<'\n';

     }

     if (m_particles->m_alive[i+1])
     {
       store4f(v, xmm1);
       verts->m_x = v[0];
       verts->m_y = v[1];
       verts->m_z = v[2];
       ++verts;

       //       pVertices->v.x = v[0];
//       pVertices->v.y = v[1];
//       pVertices->v.z = v[2];
//       pVertices++;
//       std::cout<<"P "<<v[0]<<' '<<v[1]<<' '<<v[2]<<'\n';

     }

     if (m_particles->m_alive[i+2])
     {
       store4f(v, xmm2);
       verts->m_x = v[0];
       verts->m_y = v[1];
       verts->m_z = v[2];
       ++verts;

       //       pVertices->v.x = v[0];
//       pVertices->v.y = v[1];
//       pVertices->v.z = v[2];
//       pVertices++;
//       std::cout<<"P "<<v[0]<<' '<<v[1]<<' '<<v[2]<<'\n';

     }

     if (m_particles->m_alive[i+3])
     {
       store4f(v, xmm3);
       verts->m_x = v[0];
       verts->m_y = v[1];
       verts->m_z = v[2];
       ++verts;

       //       pVertices->v.x = v[0];
//       pVertices->v.y = v[1];
//       pVertices->v.z = v[2];
//       pVertices++;
//       std::cout<<"P "<<v[0]<<' '<<v[1]<<' '<<v[2]<<'\n';

     }
   }
   m_vao->unmapBuffer();
   m_vao->bind();
   m_vao->draw();
   m_vao->unbind();
 }


 void ParticleSystem::updatePosition(float _dx, float _dy, float _dz)
 {
    m_pos.m_x+=_dx;
    m_pos.m_y+=_dy;
    m_pos.m_z+=_dz;
 }


