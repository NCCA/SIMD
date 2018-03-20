#include <iostream>
#include "simd.h"
#include "ParticleSystem.h"

int main()
{
  ParticleSystem p(16000,{0,0,0});
  for(int i=0; i<100; ++i)
  {
    p.render();
    p.update(0.01f);
  }
}
