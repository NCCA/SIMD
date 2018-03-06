// build
// clang++ -std=c++11 -mfma -msse3 -m64 -O3 -ffast-math main.cpp -o test

#include <pmmintrin.h>
#include <iostream>
#include <gtest/gtest.h>


TEST(SSE3,addTest)
{
  __m128 a=_mm_setr_ps(5, 6, 7, 8);
  __m128 b=_mm_setr_ps(1, 2, 3, 4);
  __m128 res=_mm_add_ps(a, b);
  float r[4];  
  _mm_store_ps(r,res);

  std::cout<<"add result "<<r[0]<<' '<<r[1]<<' '<<r[2]<<' '<<r[3]<<'\n';
  ASSERT_FLOAT_EQ(r[0],6);
  ASSERT_FLOAT_EQ(r[1],8);
  ASSERT_FLOAT_EQ(r[2],10);
  ASSERT_FLOAT_EQ(r[3],12);
}

TEST(SSE3,subTest)
{
  __m128 a=_mm_setr_ps(5, 6, 7, 8);
  __m128 b=_mm_setr_ps(1, 2, 3, 4);
  __m128 res=_mm_sub_ps(a, b);
  float r[4];  
  _mm_store_ps(r,res);

  std::cout<<"sub result "<<r[0]<<' '<<r[1]<<' '<<r[2]<<' '<<r[3]<<'\n';
  ASSERT_FLOAT_EQ(r[0],4);
  ASSERT_FLOAT_EQ(r[1],4);
  ASSERT_FLOAT_EQ(r[2],4);
  ASSERT_FLOAT_EQ(r[3],4);

}

int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}


