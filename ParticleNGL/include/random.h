#ifndef RANDOM_H_
#define RANDOM_H_
#include <cstdint>
#include <cstdlib>
#include <cmath>
#include <unistd.h>
#include "simd.h"

namespace frng // fast rng code
{
  extern uint32_t randomFast();
  extern float randomFloat();
  extern float uniformFloat();
  extern float randomFloat(float min, float max);
  extern void setSeed(uint32_t s);
  extern i128 randomSSE();
  extern f128 randomFloatSSE();
  extern void setSeedSSE(uint32_t s);
  extern f128 randomFloatSSE(float min, float max);
  extern f128 uniformFloatSSE();
  extern uint32_t jkiss32();
  extern i128 jkiss4i();
  extern void seedJkiss4i(int32_t x=123456789, int32_t y=234567891,int32_t z=345678912,
                   int32_t w=456789123, int32_t c=0);

  extern float jkissFloat();
  extern float jkissFloat(float min, float max);

  extern f128 jkiss4f();
  extern f128 jkiss4f(float min, float max);


  #ifdef UNITTESTS
    uint32_t randomFast(uint32_t _state);
    extern float randomFloat(uint32_t _state);
    extern f128 frexp4f(const f128 x, i128 &e);
    extern f128 log4f(const f128 _l);
    extern float log1f(float _l);
    extern f128 fastlog4f(const f128 _l);


  #endif
};


#endif
