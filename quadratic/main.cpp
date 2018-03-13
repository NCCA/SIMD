#include "simd.h"
#include <cmath>
#include <ngl/Vec3.h>
#include <iostream>
#include <benchmark/benchmark.h>
/*
f256 solve_quadratic(f256 a, f256 b, f256 c, f256 r[2])
{
  const f256 half = splat8f(0.5f);
  const f256 four = splat8f(4.0f);
  const f256 a2 = div8f(half, a);
  const f256 ac = mul8f(a, c);
  const f256 b2 = mul8f(b, b);
  const f256 minusb = negate8f(b);
  const f256 b2_4ac = fnmadd8f(ac, four, b2);
  const f256 cmpaA = cmpeq8f(zero8f(), a);
  const f256 isinvalid = isnegative(b2_4ac);
  const f256 sqrt = sqrt8f(b2_4ac);
  const f256 f0 = sub8f(minusb, sqrt);
  const f256 f1 = add8f(minusb, sqrt);
  r[0] = mul8f(f0, a2);
  r[1] = mul8f(f1, a2);
  return and8f(cmpaA, isinvalid);
}
*/

bool solve_quadratic(float a, float b, float c, float r[2])
{
  if(a == 0) return false;
  float term = b*b-4.0f*a*c;
  if(term < 0) return false;
  float a2 = 0.5f / a; // == 1.0f / (2.0f*a)
  term = sqrtf(term);
  r[0] = (-b + term) * a2;
  r[1] = (-b - term) * a2;
  return true;
}


static void normalQuadratic(benchmark::State& state)
{
 for (auto _ : state)
  {
  ngl::Vec3 ray(1,-1,0);
  ray.normalize();
  // cal the A value as the dotproduct a.a (see lecture notes)
  float a = ray.dot(ray);
  ngl::Vec3 spherePos(1,0,0);
  ngl::Vec3 p=ray-spherePos;
  float b= ray.dot(p)*2.0f;
  // C = (Po-Pc).(Po-Pc)-r^2
  float radius=0.8f;
  float c=p.dot(p)-radius*radius;
  float res[2];
  bool hit=solve_quadratic(a,b,c,&res[0]);
  //std::cout<<hit<<' '<<res[0]<<' '<<res[1]<<'\n';
 }
}

BENCHMARK(normalQuadratic);

BENCHMARK_MAIN();
