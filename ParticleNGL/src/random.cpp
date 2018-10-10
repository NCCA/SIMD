#include "random.h"
#include <iostream>
#include <cmath>
#include <fcntl.h>
namespace frng // fast rng code
{



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



/* Implementation of a 32-bit KISS generator which uses no multiply instructions */
static uint32_t s_kiss32x=123456789;
static uint32_t s_kiss32y=234567891;
static uint32_t s_kiss32z=345678912;
static uint32_t s_kiss32w=456789123;
static uint32_t s_kiss32c=0;

static i128 s_kiss4ix;
static i128 s_kiss4iy;
static i128 s_kiss4iz;
static i128 s_kiss4iw;
static i128 s_kiss4ic;

static bool s_kiss4iInit=false;

uint32_t rand1i()
{
 int32_t t;
 s_kiss32y ^= (s_kiss32y<<5);
 s_kiss32y ^= (s_kiss32y>>7);
 s_kiss32y ^= (s_kiss32y<<22);
 t = s_kiss32z+s_kiss32w+s_kiss32c;
 s_kiss32z = s_kiss32w;
 s_kiss32c = t < 0;
 s_kiss32w = t&2147483647;
 s_kiss32x += 1411392427;
 return s_kiss32x + s_kiss32y + s_kiss32w;
}

float rand1f()
{
  float x;
  unsigned int a;
  a = rand1i() >> 9; // Take upper 23 bits
  *((unsigned int *)&x) = a | 0x3F800000; // Make a float from bits
  return x-1.0F;
}


void seedJkiss4i(int32_t x, int32_t y,int32_t z,int32_t w, int32_t c)
{
  s_kiss4ix=set4i(x,x+1,x+2,x+3);
  s_kiss4iy=set4i(y,y+1,y+2,y+3);
  s_kiss4iz=set4i(z,z+1,z+2,z+3);
  s_kiss4iw=set4i(w,w+1,w+2,w+3);
  s_kiss4ic=set4i(c,c+1,c+2,c+3);
}


i128 rand4i()
{
  i128 ZERO=splat4i(0);
  i128 t;
  s_kiss4iy=xor4i( s_kiss4iy,shiftBitsLeft4i32( s_kiss4iy,5));
  s_kiss4iy=xor4i( s_kiss4iy,shiftBitsRight4i32( s_kiss4iy,7));
  s_kiss4iy=xor4i( s_kiss4iy,shiftBitsLeft4i32( s_kiss4iy,22));
  t=add4i(s_kiss4ic,add4i(s_kiss4iz,s_kiss4iw));
  s_kiss4iz=s_kiss4iw;
  s_kiss4ic=cmplt4i(t,ZERO);
  s_kiss4iw=and4i(t,splat4i(2147483647));
  s_kiss4ix=add4i(s_kiss4ix,splat4i(1411392427));
  return add4i(s_kiss4ix,add4i(s_kiss4iy,s_kiss4iw));
}

f128 rand4f()
{
  const i128 mask=splat4i(0x3F800000);
  const f128 ONE=splat4f(1.0f);
  f128 x;
  i128 a=shiftBitsRight4i32( rand4i() , 9); // Take upper 23 bits
  x=or4f (cast4f(a),cast4f(mask));
  return sub4f(x,ONE);

}

f128 rand4f(float min, float max)
{
  const f128 Min=splat4f(min);
  const f128 Max=splat4f(max);
  f128 dist=sub4f(Max,Min);
  return fmadd4f(dist,rand4f(),Min);
}


float rand1f(float min, float max)
{
   return  (max - min) * ((((float) rand1f()) )) + min;
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
  e = (emm0);
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
  return fmadd4f(c1, d2, cast4f(exp));
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
  return add4f(cast4f(exp), logm);
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
 s_kiss32x = devrand();
 while (!(s_kiss32y = devrand())); /* y must not be zero! */
 s_kiss32z = devrand();
 /* We don’t really need to set c as well but let's anyway… */
 /* NOTE: offset c by 1 to avoid z=c=0 */
 s_kiss32c = devrand() % 698769068 + 1; /* Should be less than 698769069 */
}

void initKissSSEUrand()
{
  s_kiss4ix=set4i(devrand(),devrand(),devrand(),devrand());
  uint32_t a,b,c,d;
  while (!(a = devrand()) && !(b = devrand()) && !(c = devrand()) && !(d = devrand())); /* y must not be zero! */

  s_kiss4iy=set4i(a,b,c,d);
  s_kiss4iz=set4i(devrand(),devrand(),devrand(),devrand());
  s_kiss4iw=set4i(devrand(),devrand(),devrand(),devrand());
  s_kiss4ic=set4i(devrand() % 698769068 + 1,devrand() % 698769068 + 1,
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


