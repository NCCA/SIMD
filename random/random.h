#ifndef RANDOM_H_
#define RANDOM_H_
#include <cstdint>
#include <cstdlib>
#include <cmath>
#include <unistd.h>

namespace frng // fast rng code
{
  extern uint32_t xorshift32();
  extern float randomFloat();
  extern float uniformFloat();
  extern float randomFloat(float min, float max);
  extern void setSeed(uint32_t s);
};


#endif
