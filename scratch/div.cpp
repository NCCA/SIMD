#include <iostream>
#include <cmath>

void classify(float a)
{
  auto r=std::fpclassify(a);
  switch (r)
  {
    case FP_NAN : std::cout<<"fp NAN\n"; break;
    case FP_NORMAL : std::cout<<"fp Normal\n"; break;
    case FP_SUBNORMAL : std::cout<<"fp SubNormal\n"; break;
    case FP_ZERO : std::cout<<"fp Zero\n"; break;
    case FP_INFINITE : std::cout<<"fp NAN\n"; break;
    
  }
}


int main()
{
  float a=10.0f;
  float zero=0.0f;
  std::cout<<a/zero<<'\n';

  classify(a/zero);
}
