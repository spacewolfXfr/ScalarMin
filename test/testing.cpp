// Copyright (c) 2025 Mael FEURGARD <mael.feurgard@enac.fr>
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <gtest/gtest.h>
#include <array>
#include <vector>

#include "RoundingPolicies.hpp"
#include "testInterface.hpp"
#include "circleCircle.hpp"
#include "polynomials.hpp"

#ifndef TEST_SEED_COUNT
#define TEST_SEED_COUNT 4
#endif

// -------------------- Helper functions -------------------- //

template<typename T, class Policies>
void EXPECT_MINIMIZE_RESULTS(IntervalTesting::TestCase<T,Policies> &c)
{
  T s = c.minimize_interval_method();

  EXPECT_NEAR(s     ,c.testVals.location,c.testVals.precision);
  EXPECT_NEAR(c.f(s),c.testVals.value   ,c.testVals.precision);
}

template<typename T, class Policies>
void EXPECT_MINIMIZE_DER_RESULTS(IntervalTesting::TestCaseWithDerivatives<T,Policies> &c)
{
  T s = c.minimize_interval_method_with_derivatives();

  EXPECT_NEAR(s     ,c.testVals.location,c.testVals.precision);
  EXPECT_NEAR(c.f(s),c.testVals.value   ,c.testVals.precision);
}

// -------------------- GTest classes -------------------- //

class TestPolynomialByDegree : public testing::TestWithParam<unsigned short>
{
protected:
  TestPolynomialByDegree() 
  {
    Polynomial<float,OptimizationRounding::standard_rounding<float>,GetParam()> p_std_float;



  }


};

// TODO: Meta-meta testing ??
// https://stackoverflow.com/questions/8507385/google-test-is-there-a-way-to-combine-a-test-which-is-both-type-parameterized-a

template<class T>
struct foo_test : public ::testing::Test {
    static std::vector<T> _range_;
};

TYPED_TEST_CASE_P(foo_test);

TYPED_TEST_P(foo_test, IsGreaterThanZero) {
    for (TypeParam value : foo_test<TypeParam>::_range_) {
        EXPECT_GT(value,0);
    }
}

REGISTER_TYPED_TEST_CASE_P(foo_test,IsGreaterThanZero);

typedef ::testing::Types<char, int, float> MyTypes;
INSTANTIATE_TYPED_TEST_CASE_P(My, foo_test, MyTypes);

template<> std::vector<char> foo_test<char>::_range_{'1','2','3'};
template<> std::vector<int> foo_test<int>::_range_{1,2,3};
template<> std::vector<float> foo_test<float>::_range_{1.1,2.2,0.0};

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}



// INSTANTIATE_TEST_SUITE_P()

// -------------------- Actual testing -------------------- //


TEST(StdPolynomials, RootOne) {

  // ----- Using float ----- //
  IntervalTesting::TestValues<float> t1{
    .location   = 1.f,
    .value      = 0.f,
    .init_left  = -10.f,
    .init_right =  10.f,
    .precision  = 1e-4f,
  };

  std::array<float,3> c1{1,-2,1};

  auto p1 = Polynomial<float,OptimizationRounding::standard_rounding<float>,2>(t1,c1);

  EXPECT_MINIMIZE_RESULTS(p1);
  EXPECT_MINIMIZE_DER_RESULTS(p1);

  // ----- Using double ----- //
  IntervalTesting::TestValues<double> t2{
    .location   = 1.,
    .value      = 0.,
    .init_left  = -10.,
    .init_right =  10.,
    .precision  = 1e-8,
  };

  std::array<double,3> c2{1,-2,1};

  auto p2 = Polynomial<double,OptimizationRounding::standard_rounding<double>,2>(t2,c2);

  EXPECT_MINIMIZE_RESULTS(p2);
  EXPECT_MINIMIZE_DER_RESULTS(p2);

}

TEST(StdPolynomials, FloatRandomRootsInRange) {
  // ----- Float, roots in interval ----- //
  float precision_f = 1e-4f;
  float r_min = -1.f;
  float r_max =  1.f;
  float I_min = -10.f;
  float I_max =  10.f;

  for(unsigned short i = 0 ; i < TEST_SEED_COUNT ; i++)
  {
    auto poly_test = generate_polynomial_test_case<float,OptimizationRounding::standard_rounding<float>,3>(
      i, r_min, r_max, I_min, I_max, precision_f
    );

    EXPECT_MINIMIZE_RESULTS(poly_test);
    EXPECT_MINIMIZE_DER_RESULTS(poly_test);
  }
}

TEST(StdPolynomials, FloatRandomRootsOutRange) {
  // ----- Float, roots out of interval ----- //
  float precision_f = 1e-4f;
  float r_min = -10.f;
  float r_max =  10.f;
  float I_min = -3.f;
  float I_max =  3.f;

  for(unsigned short i = 0 ; i < TEST_SEED_COUNT ; i++)
  {
    auto poly_test = generate_polynomial_test_case<float,OptimizationRounding::standard_rounding<float>,3>(
      i, r_min, r_max, I_min, I_max, precision_f
    );

    EXPECT_MINIMIZE_RESULTS(poly_test);
    EXPECT_MINIMIZE_DER_RESULTS(poly_test);
  }

}

TEST(StdPolynomials, DoubleRandomRootsInRange) {
  // ----- Double, roots in interval ----- //

  double precision_d = 1e-4;
  double r_min_d = -1.;
  double r_max_d =  1.;
  double I_min_d = -10.;
  double I_max_d =  10.;

  for(unsigned short i = 0 ; i < TEST_SEED_COUNT ; i++)
  {
    auto poly_test = generate_polynomial_test_case<double,OptimizationRounding::standard_rounding<double>,3>(
      i, r_min_d, r_max_d, I_min_d, I_max_d, precision_d
    );

    EXPECT_MINIMIZE_RESULTS(poly_test);    
    EXPECT_MINIMIZE_DER_RESULTS(poly_test);
  }
}

TEST(StdPolynomials, DoubleRandomRootsOutRange) {
  // ----- Double, roots out of interval ----- //
  double precision_d = 1e-4;
  double r_min_d = -10.;
  double r_max_d =  10.;
  double I_min_d = -3.;
  double I_max_d =  3.;

  for(unsigned short i = 0 ; i < TEST_SEED_COUNT ; i++)
  {
    auto poly_test = generate_polynomial_test_case<double,OptimizationRounding::standard_rounding<double>,3>(
      i, r_min_d, r_max_d, I_min_d, I_max_d, precision_d
    );

    EXPECT_MINIMIZE_RESULTS(poly_test);    
    EXPECT_MINIMIZE_DER_RESULTS(poly_test);
  }
}



