#include "simd.h"
#include <iostream>
#include <cmath>
#include <gtest/gtest.h>


TEST(AVX,set8f)
{
  f256 a=set8f(1.0f, 2.0f, 3.0f, 4.0f,5.0f,6.0f,7.0f,8.0f);

  float res[8];
  store8f(res,a);
  std::cout<<"result "<<res[0]<<' '<<res[1]<<' '<<res[2]<<' '<<res[3]<<
             ' '<<res[4]<<' '<<res[5]<<res[6]<<' '<<res[7]<<'\n';
  ASSERT_FLOAT_EQ(a[0],1.0f);
  ASSERT_FLOAT_EQ(a[1],2.0f);
  ASSERT_FLOAT_EQ(a[2],3.0f);
  ASSERT_FLOAT_EQ(a[3],4.0f);
  ASSERT_FLOAT_EQ(a[4],5.0f);
  ASSERT_FLOAT_EQ(a[5],6.0f);
  ASSERT_FLOAT_EQ(a[6],7.0f);
  ASSERT_FLOAT_EQ(a[7],8.0f);
}


TEST(AVX,add8f)
{
  f256 a=set8f(1.0f, 2.0f, 3.0f, 4.0f,5.0f,6.0f,7.0f,8.0f);
  f256 b=set8f(1.0f, 2.0f, 3.0f, 4.0f,5.0f,6.0f,7.0f,8.0f);
  f256 r=add8f(a,b);
  float res[8];
  store8f(res,r);
  std::cout<<"result "<<res[0]<<' '<<res[1]<<' '<<res[2]<<' '<<res[3]<<
             ' '<<res[4]<<' '<<res[5]<<res[6]<<' '<<res[7]<<'\n';
  ASSERT_FLOAT_EQ(r[0],2.0f);
  ASSERT_FLOAT_EQ(r[1],4.0f);
  ASSERT_FLOAT_EQ(r[2],6.0f);
  ASSERT_FLOAT_EQ(r[3],8.0f);
  ASSERT_FLOAT_EQ(r[4],10.0f);
  ASSERT_FLOAT_EQ(r[5],12.0f);
  ASSERT_FLOAT_EQ(r[6],14.0f);
  ASSERT_FLOAT_EQ(r[7],16.0f);
}

TEST(AVX,sub8f)
{
  f256 a=set8f(1.0f, 2.0f, 3.0f, 4.0f,5.0f,6.0f,7.0f,8.0f);
  f256 b=set8f(1.0f, 2.0f, 3.0f, 4.0f,5.0f,6.0f,7.0f,8.0f);
  f256 r=sub8f(a,b);
  float res[8];
  store8f(res,r);
  std::cout<<"result "<<res[0]<<' '<<res[1]<<' '<<res[2]<<' '<<res[3]<<
             ' '<<res[4]<<' '<<res[5]<<res[6]<<' '<<res[7]<<'\n';
  ASSERT_FLOAT_EQ(r[0],0.0f);
  ASSERT_FLOAT_EQ(r[1],0.0f);
  ASSERT_FLOAT_EQ(r[2],0.0f);
  ASSERT_FLOAT_EQ(r[3],0.0f);
  ASSERT_FLOAT_EQ(r[4],0.0f);
  ASSERT_FLOAT_EQ(r[5],0.0f);
  ASSERT_FLOAT_EQ(r[6],0.0f);
  ASSERT_FLOAT_EQ(r[7],0.0f);

}

TEST(AVX,mul8f)
{
  f256 a=set8f(1.0f, 2.0f, 3.0f, 4.0f,5.0f,6.0f,7.0f,8.0f);
  f256 b=splat8f(0.5f);
  f256 r=mul8f(a,b);
  float res[8];
  store8f(res,r);
  std::cout<<"result "<<res[0]<<' '<<res[1]<<' '<<res[2]<<' '<<res[3]<<
             ' '<<res[4]<<' '<<res[5]<<res[6]<<' '<<res[7]<<'\n';
  ASSERT_FLOAT_EQ(r[0],0.5f);
  ASSERT_FLOAT_EQ(r[1],1.0f);
  ASSERT_FLOAT_EQ(r[2],1.5f);
  ASSERT_FLOAT_EQ(r[3],2.0f);
  ASSERT_FLOAT_EQ(r[4],2.5f);
  ASSERT_FLOAT_EQ(r[5],3.0f);
  ASSERT_FLOAT_EQ(r[6],3.5f);
  ASSERT_FLOAT_EQ(r[7],4.0f);

}





int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
