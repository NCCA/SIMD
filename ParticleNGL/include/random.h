#ifndef RANDOM_H_
#define RANDOM_H_
#include <cstdint>
#include <cstdlib>
#include <cmath>
#include <unistd.h>
#include "simd.h"

namespace frng // fast rng code
{
  extern uint32_t rand1i();
  extern i128 rand4i();
  extern void seedJkiss4i(int32_t x=123456789, int32_t y=234567891,int32_t z=345678912,
                   int32_t w=456789123, int32_t c=0);

  extern float rand1f();
  extern float rand1f(float min, float max);
  extern f128 rand4f();
  extern f128 rand4f(float min, float max);
  extern void initKiss32Urand();
  extern void initKissSSEUrand();



  #ifdef UNITTESTS
    extern f128 frexp4f(const f128 x, i128 &e);
    extern f128 log4f(const f128 _l);
    extern float log1f(float _l);
    extern f128 fastlog4f(const f128 _l);


  #endif
};


#endif
