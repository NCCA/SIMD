#include <functional>
#include "Mat2Func.h"
#include "Benchmark.h"
constexpr size_t iterations=1<<20;

template <class T>
void doNotOptimize(T& value) {
#if defined(__clang__)
  asm volatile("" : "+r,m"(value) : : "memory");
#else
  asm volatile("" : "+m,r"(value) : : "memory");
#endif
}


void Bench(std::function<void(Mat2 &,const Mat2 &,const Mat2 &)> _func, const std::string &_name)
{
  Mat2 a;
  Mat2 b(1.0f,2.0f,3.0f,4.0f);
  Mat2 c(2.0f,3.0f,4.0f,5.0f);
  Benchmark<std::chrono::high_resolution_clock,std::chrono::nanoseconds> bench(iterations,_name+".txt");
  bench.startTimer();
  for(size_t i=0; i<iterations; ++i)
  {
     _func(a,b,c);
     doNotOptimize(a);
     bench.addDuration();
  }
  std::cout<<_name<<" "<<bench.min()
           <<" nS Max "<<bench.max()
           <<" nS Average "<<bench.average()
           <<" nS Median "<<bench.median()<<" nS\n";

}



int main()
{
  Bench(MultMat2Normal,"Normal");
  Bench(MultMat2Unroll,"Unroll");
  Bench(MultMat2Prefetch,"Prefetch");
  Bench(MultMat2Operator,"Operator");
  Bench(MultMat2SIMD,"SIMD");
  Bench(MultMat2AVX2,"AVX2");

}
