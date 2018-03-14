// clang++ -std=c++11 -I../include -mfma -mavx2 -mf16c -m64 -O3 -ffast-math dot.cpp -o dot

#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <bitset>
#include "simd.h"

int main ()
{
/*
Mask layout
01234567
abcdefgh


tmp0 := (mask4 == 1) ? (a0 * b0) : +0.0 (d)
tmp1 := (mask5 == 1) ? (a1 * b1) : +0.0 (c)
tmp2 := (mask6 == 1) ? (a2 * b2) : +0.0 (b)
tmp3 := (mask7 == 1) ? (a3 * b3) : +0.0 (a)

tmp4 := tmp0 + tmp1 + tmp2 + tmp3

r0 := (mask0 == 1) ? tmp4 : +0.0  (e)
r1 := (mask1 == 1) ? tmp4 : +0.0  (f)
r2 := (mask2 == 1) ? tmp4 : +0.0  (g)
r3 := (mask3 == 1) ? tmp4 : +0.0  (h)
*/
  f128 a=set4f(1.0f,2.0f,3.0f,4.0f);
  f128 b=set4f(0.5f,1.5f,2.5f,3.5f);
  f128 c;
  float r[4];
  // result is  0.5 3.0 7.5 14.0
  // note c++ 14 binary literal may not work everywhere
  const int all =0b11111111;
  const int u1  =0b10001111;
  const int u2  =0b01001111;
  const int u3  =0b00101111;
  const int u4  =0b00011111;
  
  std::cout<<"bin "<<std::bitset<8>(all)<<' '<<all<<'\n';
  c= _mm_dp_ps(a,b,all);
  storeu4f(r,c);
  std::cout<<"result "<<r[0]<<' '<<r[1]<<' '<<r[2]<<' '<<r[3]<<'\n'; 

  std::cout<<"bin "<<std::bitset<8>(u1)<<' '<<u1<<'\n';
  c= _mm_dp_ps(a,b,u1);
  storeu4f(r,c);
  std::cout<<"result "<<r[0]<<' '<<r[1]<<' '<<r[2]<<' '<<r[3]<<'\n'; 

  std::cout<<"bin "<<std::bitset<8>(u2)<<' '<<u2<<'\n';
  c= _mm_dp_ps(a,b,u2);
  storeu4f(r,c);
  std::cout<<"result "<<r[0]<<' '<<r[1]<<' '<<r[2]<<' '<<r[3]<<'\n'; 

  std::cout<<"bin "<<std::bitset<8>(u3)<<' '<<u3<<'\n';
  c= _mm_dp_ps(a,b,u3);
  storeu4f(r,c);
  std::cout<<"result "<<r[0]<<' '<<r[1]<<' '<<r[2]<<' '<<r[3]<<'\n'; 

  std::cout<<"bin "<<std::bitset<8>(u4)<<' '<<u4<<'\n';
  c= _mm_dp_ps(a,b,u4);
  storeu4f(r,c);
  std::cout<<"result "<<r[0]<<' '<<r[1]<<' '<<r[2]<<' '<<r[3]<<'\n'; 

  // masks for lower order (result)
  const int c1  =0b11111000;
  const int c2  =0b11110100;
  const int c3  =0b11110010;
  const int c4  =0b11110001;
  
  std::cout<<"bin "<<std::bitset<8>(c1)<<' '<<c1<<'\n';
  c= _mm_dp_ps(a,b,c1);
  storeu4f(r,c);
  std::cout<<"result "<<r[0]<<' '<<r[1]<<' '<<r[2]<<' '<<r[3]<<'\n'; 

  std::cout<<"bin "<<std::bitset<8>(c2)<<' '<<c2<<'\n';
  c= _mm_dp_ps(a,b,c2);
  storeu4f(r,c);
  std::cout<<"result "<<r[0]<<' '<<r[1]<<' '<<r[2]<<' '<<r[3]<<'\n'; 

  std::cout<<"bin "<<std::bitset<8>(c3)<<' '<<c3<<'\n';
  c= _mm_dp_ps(a,b,c3);
  storeu4f(r,c);
  std::cout<<"result "<<r[0]<<' '<<r[1]<<' '<<r[2]<<' '<<r[3]<<'\n'; 

  std::cout<<"bin "<<std::bitset<8>(c4)<<' '<<c4<<'\n';
  c= _mm_dp_ps(a,b,c4);
  storeu4f(r,c);
  std::cout<<"result "<<r[0]<<' '<<r[1]<<' '<<r[2]<<' '<<r[3]<<'\n'; 




  return EXIT_SUCCESS;
}