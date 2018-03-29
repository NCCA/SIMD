#include "random.h"
#include <iostream>
namespace frng // fast rng code
{


static uint32_t g_state=1;
static i128 g_stateSSE;

void setSeed(uint32_t s)
{
  g_state=s;
}

void setSeedSSE(uint32_t s)
{
  uint32_t state[4];
  state[0]=s+0;
  state[1]=s+1;
  state[2]=s+2;
  state[3]=s+3;

  g_stateSSE=load4i(&state[0]);
}


void debug(i128 _r)
{
  alignas(16) uint32_t a[4];
  store4i(&a[0],_r);
  std::cout<<"u128 "<<a[0]<<' '<<a[1]<<' '<<a[2]<<' '<<a[3]<<'\n';
}

void debug(f128 _r)
{
  alignas(16) float a[4];
  store4f(&a[0],_r);
  std::cout<<"f128 "<<a[0]<<' '<<a[1]<<' '<<a[2]<<' '<<a[3]<<'\n';
}

i128 randomSSE()
{
  i128 x=g_stateSSE;
  x=xor4i(x,shiftBitsLeft4i32(x,13));
  x=xor4i(x,shiftBitsRight4i32(x,17));
  x=xor4i(x,shiftBitsLeft4i32(x,5));
  g_stateSSE=x;
  return x;
}

// simple XOR shift based on https://en.wikipedia.org/wiki/Xorshift
uint32_t randomFast()
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
  auto r=randomFast();
  a = r >> 9; // Take upper 23 bits
  *((unsigned int *)&x) = a | 0x3F800000; // Make a float from bits
  return x-1.0F;
}


f128 randomFloatSSE()
{
  f128 x;
  const i128 mask=splat4i(0x3F800000);
  const f128 ONE=splat4f(1.0f);
  i128 a=shiftBitsRight4i32(randomSSE(),9);
  x=   or4f (a,mask);
  return sub4f(x,ONE);
}


f128 randomFloatSSE(float min, float max)
{
  const f128 Min=splat4f(min);
  const f128 Max=splat4f(max);
  f128 dist=sub4f(Max,Min);

  return fmadd4f(dist,randomFloatSSE(),Min);



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
   return  (max - min) * ((((float) randomFast()) / (float) UINT32_MAX)) + min ;
}

};


