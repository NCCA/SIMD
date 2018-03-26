// clang++ -std=c++11 -I../include -mavx2 -mfma -mf16c -m64 -O3 -ffast-math cast.cpp -o cast

#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include "simd.h"


int main()
{
  f256 data=set8f(1.0f,2.0f,3.0f,4.0f,5.0f,6.0f,7.0f,8.0f);
  auto r= _mm256_castps256_ps128(data);
  float res[4];
  store4f(res,r);
  std::cout<<res[0]<<' '<<res[1]<<' '<<res[2]<<' '<<res[3]<<'\n';
  r= _mm256_castps256_ps128(_mm256_permute2f128_ps(data,data,1));
  store4f(res,r);
  std::cout<<res[0]<<' '<<res[1]<<' '<<res[2]<<' '<<res[3]<<'\n';
  
  r=cast8fHi(data);
  store4f(res,r);
  std::cout<<res[0]<<' '<<res[1]<<' '<<res[2]<<' '<<res[3]<<'\n';

  r=cast8fLow(data);
  store4f(res,r);
  std::cout<<res[0]<<' '<<res[1]<<' '<<res[2]<<' '<<res[3]<<'\n';



}