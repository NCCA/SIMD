// clang++ -std=c++11 -I../include -mavx2 -mfma -mf16c -m64 -O3 -ffast-math movemask.cpp -o movemask

#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <bitset>
#include "simd.h"


void mask4f( float a, float b, float c, float d)
{
  const f128 ZERO = zero4f();

  auto mask=movemask4f(cmplteq4f(set4f(a,b,c,d),ZERO));
  std::cout<<a<<b<<c<<d<<' '<<mask<<'\n';
}

void mask8f( float a, float b, float c, float d,float e, float f, float g, float h)
{
  const f256 ZERO = zero8f();

  auto mask=movemask8f(cmplteq8f(set8f(a,b,c,d,e,f,g,h),ZERO));
  std::cout<<a<<b<<c<<d<<e<<f<<g<<h<<' '<<mask<<'\n';
}

int main()
{
  std::cout<<"Mask 4f\n";
  mask4f(0,0,0,0);
  mask4f(0,0,0,1);
  mask4f(0,0,1,0);
  mask4f(0,0,1,1);
  mask4f(0,1,0,0);
  mask4f(0,1,0,1);
  mask4f(0,1,1,1);
  mask4f(1,0,0,0);
  mask4f(1,0,0,1);
  mask4f(1,0,1,0);
  mask4f(1,0,1,1);
  mask4f(1,1,0,0);
  mask4f(1,1,0,1);
  mask4f(1,1,1,1);
  std::cout<<"Mask 8f\n";
  mask8f(0,0,0,0,0,0,0,0);
  mask8f(0,0,0,0,0,0,0,1);
  mask8f(0,0,0,0,0,0,1,0);
  mask8f(0,0,0,0,0,0,1,1);
  mask8f(0,0,0,0,0,1,0,0);
  mask8f(0,0,0,0,0,1,0,1);
  mask8f(0,0,0,0,0,1,1,0);
  mask8f(0,0,0,0,0,1,1,1);
  mask8f(0,0,0,0,1,0,0,0);
  mask8f(0,0,0,0,1,0,0,1);
  mask8f(0,0,0,0,1,0,1,0);
  mask8f(0,0,0,0,1,0,1,1);
  mask8f(0,0,0,0,1,1,0,0);
  mask8f(0,0,0,0,1,1,0,1);
  mask8f(0,0,0,0,1,1,1,0);
  mask8f(0,0,0,0,1,1,1,1);
  mask8f(0,0,0,1,0,0,0,0);
  

}
