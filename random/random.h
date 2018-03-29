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

};


#endif
