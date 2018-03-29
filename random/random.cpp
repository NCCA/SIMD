#include "random.h"

namespace frng // fast rng code
{


static uint32_t g_state=1;

void setSeed(uint32_t s)
{
  g_state=s;
}

// simple XOR shift based on https://en.wikipedia.org/wiki/Xorshift
uint32_t xorshift32()
{
	/* Algorithm "xor" from p. 4 of Marsaglia, "Xorshift RNGs" */
	uint32_t x = g_state;
	x ^= x << 13;
	x ^= x >> 17;
	x ^= x << 5;
	g_state = x;
	return x;
}

// 0-1 random number
float randomFloat()
{
  float x;
  unsigned int a;
  a = xorshift32() >> 9; /* Take upper 23 bits */
  *((unsigned int *)&x) = a | 0x3F800000; /* Make a float from bits*/
  return x-1.0F;
}

/* Generate gaussian deviate with mean 0 and stdev 1 */
float uniformFloat()
{
 float x, y, r;
 do
 {
  x = 2.0f * randomFloat() - 1.0f;
  y = 2.0f * randomFloat() - 1.0f;
  r = x * x + y * y;
 }
 while (r == 0.0f || r >= 1.0f);
 r = sqrtf((-2.0f * log(r)) / r);
 return x * r;
}


float randomFloat(float min, float max) 
{
   return  (max - min) * ((((float) xorshift32()) / (float) UINT32_MAX)) + min ;
}

};


