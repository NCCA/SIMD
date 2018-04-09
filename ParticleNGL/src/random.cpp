#include "random.h"
#include <iostream>
#include <cmath>
#include <fcntl.h>
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

/* Implementation of a 32-bit KISS generator which uses no multiply instructions */
static uint32_t g_kiss32x=123456789;
static uint32_t g_kiss32y=234567891;
static uint32_t g_kiss32z=345678912;
static uint32_t g_kiss32w=456789123;
static uint32_t g_kiss32c=0;

uint32_t jkiss32()
{
 int32_t t;
 g_kiss32y ^= (g_kiss32y<<5);
 g_kiss32y ^= (g_kiss32y>>7);
 g_kiss32y ^= (g_kiss32y<<22);
 t = g_kiss32z+g_kiss32w+g_kiss32c;
 g_kiss32z = g_kiss32w;
 g_kiss32c = t < 0;
 g_kiss32w = t&2147483647;
 g_kiss32x += 1411392427;
 return g_kiss32x + g_kiss32y + g_kiss32w;
}

float jkissFloat()
{
  float x;
  uint32_t a;
  a = jkiss32() >> 9; // Take upper 23 bits
  *((unsigned int *)&x) = a | 0x3F800000; // Make a float from bits
  return x-1.0F;
}

float jkissFloat(float min, float max)
{
   return  (max - min) * ((((float) jkissFloat()))) + min ;
}



static i128 g_kiss4ix;
static i128 g_kiss4iy;
static i128 g_kiss4iz;
static i128 g_kiss4iw;
static i128 g_kiss4ic;

void seedJkiss4i(int32_t x, int32_t y,int32_t z,int32_t w, int32_t c)
{
  g_kiss4ix=set4i(x,x+1,x+2,x+3);
  g_kiss4iy=set4i(y,y+1,y+2,y+3);
  g_kiss4iz=set4i(z,z+1,z+2,z+3);
  g_kiss4iw=set4i(w,w+1,w+2,w+3);
  g_kiss4ic=set4i(c,c+1,c+2,c+3);
}


i128 jkiss4i()
{
  i128 ZERO=splat4i(0);
  i128 t;
  g_kiss4iy=xor4i( g_kiss4iy,shiftBitsLeft4i32( g_kiss4iy,5));
  g_kiss4iy=xor4i( g_kiss4iy,shiftBitsRight4i32( g_kiss4iy,7));
  g_kiss4iy=xor4i( g_kiss4iy,shiftBitsLeft4i32( g_kiss4iy,22));
  t=add4i(g_kiss4ic,add4i(g_kiss4iz,g_kiss4iw));
  g_kiss4iz=g_kiss4iw;
  g_kiss4ic=cmplt4i(t,ZERO);
  g_kiss4iw=and4i(t,splat4i(2147483647));
  g_kiss4ix=add4i(g_kiss4ix,splat4i(1411392427));
  return add4i(g_kiss4ix,add4i(g_kiss4iy,g_kiss4iw));
}

f128 jkiss4f()
{
  const i128 mask=splat4i(0x3F800000);
  const f128 ONE=splat4f(1.0f);
  f128 x;
  i128 a=shiftBitsRight4i32( jkiss4i() , 9); // Take upper 23 bits
  x=   or4f (a,mask);
  return sub4f(x,ONE);

}

f128 jkiss4f(float min, float max)
{
  const f128 Min=splat4f(min);
  const f128 Max=splat4f(max);
  f128 dist=sub4f(Max,Min);
  return fmadd4f(dist,jkiss4f(),Min);
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
// see http://www0.cs.ucl.ac.uk/staff/d.jones/GoodPracticeRNG.pdf
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





/*
based on https://github.com/RJVB/sse_mathfun
*/
f128 frexp4f(const f128 x, i128 &e)
{

  const i128 ONE=splat4i(1);
  const f128 INVMANTISSAMASK = cast4f(splat4i(~0x7f800000));
  const f128 MINNORMALPOS = cast4f(splat4i(0x00800000));
  const f128 POINTFIVE=splat4f(0.5f);
  const i128 SEVEN = splat4i(0x7f);

  f128 xx = max4f(x, MINNORMALPOS);
  i128 emm0 = shiftBitsRight4i32(cast4i(xx), 23);
  xx = and4f(xx, INVMANTISSAMASK);
  xx = or4f(xx, POINTFIVE);
  emm0 = sub4i(emm0, SEVEN);
  e = cast4f(emm0);
  e = add4i(e, ONE);
  return xx;
}

float log1f(float _l)
{
  const float A1 = 0.788132f;
  const float B1 = 3.4156f;
  const float C1 = -2.34331f;
  const float D1 = -1.86043f;
  const float A2 = 0.135516f;
  const float B2 = 1.74755f;
  const float C2 = 2.47952f;
  const float D2 = 0.387069f;
  int32_t exp;
  float mant=frexpf(_l,&exp);
  auto fmadd=[](float a,float b,float c)
  {
    return (a*b)+c;
  };

  float a2 = fmadd(mant, A2, B2);
  float a1 = fmadd(mant, A1, B1);
  float b2 = fmadd(mant, a2, C2);
  float b1 = fmadd(mant, a1, C1);
  float c2 = fmadd(mant, b2, D2);
  float c1 = fmadd(mant, b1, D1);
  float d2 = 1.0f/c2;
  return fmadd(c1, d2, exp);
}


f128 log4f(const f128 _l)
{
  const f128 A1 = splat4f(0.788132f);
  const f128 B1 = splat4f(3.4156f);
  const f128 C1 = splat4f(-2.34331f);
  const f128 D1 = splat4f(-1.86043f);
  const f128 A2 = splat4f(0.135516f);
  const f128 B2 = splat4f(1.74755f);
  const f128 C2 = splat4f(2.47952f);
  const f128 D2 = splat4f(0.387069f);
  const f128 ONE= splat4f(1.0f);
  i128 exp;
  f128 mant=frexp4f(_l,exp);

  const f128 a2 = fmadd4f(mant, A2, B2);
  const f128 a1 = fmadd4f(mant, A1, B1);
  const f128 b2 = fmadd4f(mant, a2, C2);
  const f128 b1 = fmadd4f(mant, a1, C1);
  const f128 c2 = fmadd4f(mant, b2, D2);
  const f128 c1 = fmadd4f(mant, b1, D1);
  const f128 d2 = div4f(ONE,c2);//  reciprocal4f(c2);
  return fmadd4f(c1, d2, exp);
}

f128 fastlog4f(const f128 _l)
{
  const f128 A = splat4f(0.154967f);
  const f128 B = splat4f(-1.03583f);
  const f128 C = splat4f(3.02252f);
  const f128 D = splat4f(-2.14071f);

  // mantissa in zero to 1 range
  //const f128 mant = getmant16f(a);
  //const f128 exp = getexp16f(a);
  i128 exp;
  f128 mant=frexp4f(_l,exp);
  const f128 logm = fmadd4f(mant, fmadd4f(mant, fmadd4f(mant, A, B), C), D);
  return add4f(exp, logm);
}

/* Generate gaussian deviate with mean 0 and stdev 1 */
f128 uniformFloatSSE()
{
 f128 x, y, r;
 const f128 TWO=splat4f(2.0f);
 const f128 MINUSTWO=splat4f(-2.0f);
 const f128 ONE=splat4f(1.0f);
 const f128 ZERO=splat4f(0.0f);
 do
 {
   x=fnmsub4f(TWO,randomFloatSSE(),ONE);
   y=fnmsub4f(TWO,randomFloatSSE(),ONE);
   r=add4f(mul4f(x,x),mul4f(y,y));
 }while ( !movemask4f( or4f( cmpeq4f(r,ZERO) ,cmpgteq4f(r,ONE) )) );

// r = sqrtf((-2.0f * log(r)) / r);
// return x * r;


 r=sqrt4f(mul4f(MINUSTWO,div4f(log4f(r),r)));

 return sub4f(ZERO,mul4f(x ,r));

}


float randomFloat(float min, float max) 
{
   return  (max - min) * ((((float) randomFast()) / (float) UINT32_MAX)) + min ;
}

uint32_t devrand(void)
{
 int fn;
 unsigned int r;
 fn = open("/dev/urandom", O_RDONLY);
 if (fn == -1)
 {
   exit(EXIT_FAILURE); /* Failed! */
 }
 if (read(fn, &r, 4) != 4)
 {
   exit(EXIT_FAILURE); /* Failed! */
 }
 close(fn);
 return r;
}

/* Initialise KISS generator using /dev/urandom */
void initKiss32Urand()
{
 g_kiss32x = devrand();
 while (!(g_kiss32y = devrand())); /* y must not be zero! */
 g_kiss32z = devrand();
 /* We don’t really need to set c as well but let's anyway… */
 /* NOTE: offset c by 1 to avoid z=c=0 */
 g_kiss32c = devrand() % 698769068 + 1; /* Should be less than 698769069 */
}

void initKissSSEUrand()
{
  g_kiss4ix=set4i(devrand(),devrand(),devrand(),devrand());
  uint32_t a,b,c,d;
  while (!(a = devrand()) && !(b = devrand()) && !(c = devrand()) && !(d = devrand())); /* y must not be zero! */

  g_kiss4iy=set4i(a,b,c,d);
  g_kiss4iz=set4i(devrand(),devrand(),devrand(),devrand());
  g_kiss4iw=set4i(devrand(),devrand(),devrand(),devrand());
  g_kiss4ic=set4i(devrand() % 698769068 + 1,devrand() % 698769068 + 1,
                  devrand() % 698769068 + 1,devrand() % 698769068 + 1);

}

// These are for unit tests only so we can control the seed values
// when we run an SSE random we do 4 at a time and update a vec4 seed
// here we replicate it by passing in our own seed rather than using the
// global one.
#ifdef UNITTESTS
uint32_t randomFast(uint32_t _state)
{
  /* Algorithm "xor" from p. 4 of Marsaglia, "Xorshift RNGs" */
  uint32_t x = _state;
  x ^= x << 13;
  x ^= x >> 17;
  x ^= x << 5;
  return x;
}

// 0-1 random number
float randomFloat(uint32_t _state)
{
  float x;
  unsigned int a;
  auto r=randomFast(_state);
  a = r >> 9; // Take upper 23 bits
  *((unsigned int *)&x) = a | 0x3F800000; // Make a float from bits
  return x-1.0F;
}

#endif

};


