#include <functional>
#include "Mat2Func.h"
#include "Benchmark.h"
static size_t iterations=1<<10;

template <class T>
void doNotOptimize(T& value) {
#if defined(__clang__)
  asm volatile("" : "+r,m"(value) : : "memory");
#else
  asm volatile("" : "+m,r"(value) : : "memory");
#endif
}


Benchmark<std::chrono::steady_clock,std::chrono::nanoseconds> g_bench(iterations,"Bench.csv",WriteMode::CSV);


void Bench(std::function<void(Mat2 &,const Mat2 &,const Mat2 &)> _func, const std::string &_name)
{
  std::cout<<"Starting "<<_name<<'\n';
  Mat2 a;
  Mat2 b(1.0f,2.0f,3.0f,4.0f);
  Mat2 c(2.0f,3.0f,4.0f,5.0f);
  for(size_t i=0; i<iterations; ++i)
  {
     _func(a,b,c);
     doNotOptimize(a);
     g_bench.addCSV(_name);
  }
//  std::cout<<_name<<" "<<g_bench.min()
//           <<" nS Max "<<g_bench.max()
//           <<" nS Average "<<g_bench.average()
//           <<" nS Median "<<g_bench.median()<<" nS\n";
  std::cout<<"Done "<<_name<<'\n';

}



int main()
{
  g_bench.startTimer();

  Bench(MultMat2Normal,"Normal");
  Bench(MultMat2Unroll,"Unroll");
  Bench(MultMat2Prefetch,"Prefetch");
  Bench(MultMat2Operator,"Operator");
  Bench(MultMat2SIMD,"SIMD");
  Bench(MultMat2AVX2,"AVX2");

}
