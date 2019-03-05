#include "Mat2Func.h"
#include <gtest/gtest.h>
using namespace ::testing;

TEST(Mat2,userCtor)
{
  Mat2 test(1,2,3,4);
  Mat2 result(1,2,3,4);
  EXPECT_TRUE(test == result);
}

TEST(Mat2,defautCtor)
{
  Mat2 test;
  Mat2 result(1,0,0,1);
  EXPECT_TRUE(test == result);
}


TEST(Mat2,MultMat2Normal)
{
  Mat2 a;
  Mat2 b(1.0f,2.0f,3.0f,4.0f);
  Mat2 c(5.0f,6.0f,7.0f,8.0f);
  MultMat2Normal(a,b,c);
  EXPECT_TRUE(a==Mat2(19.0f,22.0f,43.0f,50.0f));
}


TEST(Mat2,MultMat2Unroll)
{
  Mat2 a;
  Mat2 b(1.0f,2.0f,3.0f,4.0f);
  Mat2 c(5.0f,6.0f,7.0f,8.0f);
  MultMat2Unroll(a,b,c);
  EXPECT_TRUE(a==Mat2(19.0f,22.0f,43.0f,50.0f));
}

TEST(Mat2,MultMat2Prefetch)
{
  Mat2 a;
  Mat2 b(1.0f,2.0f,3.0f,4.0f);
  Mat2 c(5.0f,6.0f,7.0f,8.0f);
  MultMat2Prefetch(a,b,c);
  EXPECT_TRUE(a==Mat2(19.0f,22.0f,43.0f,50.0f));
}

TEST(Mat2,operatorMult)
{
  Mat2 b(1.0f,2.0f,3.0f,4.0f);
  Mat2 c(5.0f,6.0f,7.0f,8.0f);
  auto a=b*c;
  EXPECT_TRUE(a==Mat2(19.0f,22.0f,43.0f,50.0f));
}

TEST(Mat2,MultMat2Operator)
{
  Mat2 a;
  Mat2 b(1.0f,2.0f,3.0f,4.0f);
  Mat2 c(5.0f,6.0f,7.0f,8.0f);
  MultMat2Operator(a,b,c);
  EXPECT_TRUE(a==Mat2(19.0f,22.0f,43.0f,50.0f));
}

TEST(Mat2,MultMat2SIMD)
{
  Mat2 a;
  Mat2 b(1.0f,2.0f,3.0f,4.0f);
  Mat2 c(5.0f,6.0f,7.0f,8.0f);
  MultMat2SIMD(a,b,c);
  EXPECT_TRUE(a==Mat2(19.0f,22.0f,43.0f,50.0f));
}

TEST(Mat2,MultMat2AVX2)
{
  Mat2 a;
  Mat2 b(1.0f,2.0f,3.0f,4.0f);
  Mat2 c(5.0f,6.0f,7.0f,8.0f);
  MultMat2AVX2(a,b,c);
  EXPECT_TRUE(a==Mat2(19.0f,22.0f,43.0f,50.0f));
}
