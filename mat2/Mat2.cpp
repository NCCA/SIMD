#include <hayai/hayai.hpp>
#include <hayai/hayai_main.hpp>
#include "Mat2Func.h"


Mat2 g_a;
Mat2 g_b(1.0f,2.0f,3.0f,4.0f);
Mat2 g_c(2.0f,3.0f,4.0f,5.0f);

BENCHMARK(Mat2Tests, MultMat2Normal, 10, 100)
{
  MultMat2Normal(g_a,g_b,g_c);
}

BENCHMARK(Mat2Tests, MultMat2Unroll, 10, 100)
{
  MultMat2Unroll(g_a,g_b,g_c);
}

BENCHMARK(Mat2Tests, MultMat2Prefetch, 10, 100)
{
  MultMat2Prefetch(g_a,g_b,g_c);
}

BENCHMARK(Mat2Tests, MultMat2operator, 10, 100)
{
  g_a=g_b*g_c;
}

BENCHMARK(Mat2Tests, MultMat2operatorFunc, 10, 100)
{
  MultMat2Operator(g_a,g_b,g_c);
}


BENCHMARK(Mat2Tests, MultMat2SIMD, 10, 100)
{
  MultMat2SIMD(g_a,g_b,g_c);
}

BENCHMARK(Mat2Tests, MultMat2AVX2, 10, 100)
{
  MultMat2AVX2(g_a,g_b,g_c);
}

int main(int argc, char **argv)
{
    // Set up the main runner.
    ::hayai::MainRunner runner;
    // Parse the arguments.
    int result = runner.ParseArgs(argc, argv);
    if (result)
        return result;

    // Execute based on the selected mode.
    return runner.Run();
}
