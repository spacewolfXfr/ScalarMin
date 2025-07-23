// Copyright (c) 2025 Mael FEURGARD <mael.feurgard@enac.fr>
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <gtest/gtest.h>
#include <array>
#include <tuple>
#include <vector>
#include <concepts>


#include "RoundingPolicies.hpp"
#include "testInterface.hpp"
#include "circleCircle.hpp"
#include "polynomials.hpp"

// -------------------- Configurable constants -------------------- //


// -- Number of random test to generate -- //
#if !defined(TEST_SEED_COUNT) || TEST_SEED_COUNT <= 0
#define TEST_SEED_COUNT 10
#endif

// -- Precision and degree for polynomial tests in function of float precision type -- //
// Precision and degree for float polynomial tests
#if !defined(TEST_FLOAT_PRECISION)
#define TEST_FLOAT_PRECISION 1e-3f
#endif

#if !defined(TEST_FLOAT_POLY_MAX_DEGREE) || TEST_FLOAT_POLY_MAX_DEGREE <= 2
#define TEST_FLOAT_POLY_MAX_DEGREE 4
#endif

// Precision and degree for double float polynomial tests
#if !defined(TEST_DOUBLE_PRECISION)
#define TEST_DOUBLE_PRECISION 1e-6
#endif

#if !defined(TEST_DOUBLE_POLY_MAX_DEGREE) || TEST_DOUBLE_POLY_MAX_DEGREE <= 2
#define TEST_DOUBLE_POLY_MAX_DEGREE 10
#endif

// Precision and degree for long double float polynomial tests
#if !defined(TEST_LONG_DOUBLE_PRECISION)
#define TEST_LONG_DOUBLE_PRECISION 1e-8
#endif

#if !defined(TEST_LONG_DOUBLE_POLY_MAX_DEGREE) || TEST_LONG_DOUBLE_POLY_MAX_DEGREE <= 2
#define TEST_LONG_DOUBLE_POLY_MAX_DEGREE 10
#endif

// -- Search intervals I and root generation intervals R -- //
// Subcase R included in I
#if !defined(TEST_INNER_I_MIN)
#define TEST_INNER_I_MIN -10
#endif

#if !defined(TEST_INNER_I_MAX)
#define TEST_INNER_I_MAX  10
#endif

#if !defined(TEST_INNER_R_MIN)
#define TEST_INNER_R_MIN -5
#endif

#if !defined(TEST_INNER_R_MAX)
#define TEST_INNER_R_MAX  5
#endif

// Subcase I included in R
#if !defined(TEST_OUTER_I_MIN)
#define TEST_OUTER_I_MIN -5
#endif

#if !defined(TEST_OUTER_I_MAX)
#define TEST_OUTER_I_MAX  5
#endif

#if !defined(TEST_OUTER_R_MIN)
#define TEST_OUTER_R_MIN -10
#endif

#if !defined(TEST_OUTER_R_MAX)
#define TEST_OUTER_R_MAX  10
#endif


// -------------------- Helper functions -------------------- //

template<typename T>
constexpr bool check_interval_nonempty(T l, T r)
{
  return (l < r);
}

template<typename T>
constexpr bool check_interval_inclusion(T outer_l, T outer_r, T inner_l, T inner_r)
{
  check_interval_nonempty(outer_l,outer_r);
  check_interval_nonempty(inner_l,inner_r);

  return (
    (outer_l < inner_l) &&
    (outer_l < inner_r) &&
    (inner_l < outer_r) &&
    (inner_r < outer_r)
  );
}

template<typename T, class Policies>
void EXPECT_MINIMIZE_RESULTS(IntervalTesting::TestCase<T,Policies> &c)
{
  T s = c.minimize_interval_method();

  EXPECT_NEAR(s     ,c.testVals.location,c.testVals.precision)  << "(Interval method, derivative free) Minimum locations do not match";
  EXPECT_NEAR(c.f(s),c.testVals.value   ,c.testVals.precision)  << "(Interval method, derivative free) Minimum values do not match";
}

template<typename T, class Policies>
void EXPECT_MINIMIZE_DER_RESULTS(IntervalTesting::TestCaseWithDerivatives<T,Policies> &c)
{
  T s = c.minimize_interval_method_with_derivatives();

  EXPECT_NEAR(s     ,c.testVals.location,c.testVals.precision)  << "(Interval method, with derivatives) Minimum locations do not match";
  EXPECT_NEAR(c.f(s),c.testVals.value   ,c.testVals.precision)  << "(Interval method, with derivatives) Minimum values do not match";
}

// -------------------- GTest classes -------------------- //

// Using paired types to test accross two kinds of types:
// https://stackoverflow.com/questions/52256953/c-multiple-parameters-with-gtest-typed-test
template<typename PairedType = std::pair<float,OptimizationRounding::standard_rounding<float>>>
class TestPolynomialByFloatType : public testing::Test
{
protected:

  static constexpr PairedType::first_type get_precision()
  {
    typedef typename PairedType::first_type  T;

    T precision;

    if (std::is_same<T, double>::value)
    {
      precision   = TEST_DOUBLE_PRECISION;
    }
    else if (std::is_same<T, long double>::value)
    {
      precision   = TEST_LONG_DOUBLE_PRECISION;
    }
    else
    {
      precision   = TEST_FLOAT_PRECISION;
    }

    return precision;
  }

  static constexpr size_t get_max_degree()
  {
    typedef typename PairedType::first_type  T;

    size_t max_degree;

    if (std::is_same<T, double>::value)
    {
      max_degree  = TEST_DOUBLE_POLY_MAX_DEGREE;
    }
    else if (std::is_same<T, long double>::value)
    {
      max_degree  = TEST_LONG_DOUBLE_POLY_MAX_DEGREE;
    }
    else
    {
      max_degree  = TEST_FLOAT_POLY_MAX_DEGREE;
    }

    return max_degree;
  }

  TestPolynomialByFloatType() {}

};

// -------------------- Actual testing -------------------- //

// ---------- Test on polynomials ---------- //

// Simple test to find the minimum of (X-1)²
TEST(StdPolynomials, RootOne) {

  // ----- Using float ----- //
  IntervalTesting::TestValues<float> t1{
    .location   = 1.f,
    .value      = 0.f,
    .init_left  = -10.f,
    .init_right =  10.f,
    .precision  = TEST_FLOAT_PRECISION,
  };

  std::vector<float> c1{1,-2,1};

  auto p1 = Polynomial<float,OptimizationRounding::standard_rounding<float>>(t1,c1);

  EXPECT_MINIMIZE_RESULTS(p1);
  EXPECT_MINIMIZE_DER_RESULTS(p1);

  // ----- Using double ----- //
  IntervalTesting::TestValues<double> t2{
    .location   = 1.,
    .value      = 0.,
    .init_left  = -10.,
    .init_right =  10.,
    .precision  = TEST_DOUBLE_PRECISION,
  };

  std::vector<double> c2{1,-2,1};

  auto p2 = Polynomial<double,OptimizationRounding::standard_rounding<double>>(t2,c2);

  EXPECT_MINIMIZE_RESULTS(p2);
  EXPECT_MINIMIZE_DER_RESULTS(p2);

}


// Tests on random polynomials ; inspect the effects of float precision and rounding types
using TestTypes = ::testing::Types<
  std::pair<float,OptimizationRounding::standard_rounding<float>>,
  std::pair<float,OptimizationRounding::fast_rounding<float>>,
  std::pair<double,OptimizationRounding::standard_rounding<double>>,
  std::pair<double,OptimizationRounding::fast_rounding<double>>,
  std::pair<long double,OptimizationRounding::standard_rounding<long double>>,
  std::pair<long double,OptimizationRounding::fast_rounding<long double>>
  >;

TYPED_TEST_CASE(TestPolynomialByFloatType,TestTypes);


TYPED_TEST(TestPolynomialByFloatType, RootsInRange)
{
  typedef typename TypeParam::first_type T;
  typedef typename TypeParam::second_type Policies;

  size_t max_degree = TestFixture::get_max_degree();
  T precision = TestFixture::get_precision();

  constexpr T I_min = static_cast<T>(TEST_INNER_I_MIN);
  constexpr T I_max = static_cast<T>(TEST_INNER_I_MAX);
  constexpr T R_min = static_cast<T>(TEST_INNER_R_MIN);
  constexpr T R_max = static_cast<T>(TEST_INNER_R_MAX);

  static_assert(check_interval_inclusion(I_min,I_max,R_min,R_max));

  for(auto i = 0; i < TEST_SEED_COUNT; i++)
  {
    auto poly_test = generate_polynomial_test_case<T,Policies>(i,max_degree,
      R_min,R_max,
      I_min,I_max,
      precision);

    EXPECT_MINIMIZE_RESULTS(poly_test);
    EXPECT_MINIMIZE_DER_RESULTS(poly_test);
  }
}

TYPED_TEST(TestPolynomialByFloatType, RootsOutRange)
{
  typedef typename TypeParam::first_type T;
  typedef typename TypeParam::second_type Policies;

  size_t max_degree = TestFixture::get_max_degree();
  T precision = TestFixture::get_precision();

  constexpr T I_min = static_cast<T>(TEST_OUTER_I_MIN);
  constexpr T I_max = static_cast<T>(TEST_OUTER_I_MAX);
  constexpr T R_min = static_cast<T>(TEST_OUTER_R_MIN);
  constexpr T R_max = static_cast<T>(TEST_OUTER_R_MAX);

  static_assert(check_interval_inclusion(R_min,R_max,I_min,I_max));


  for(auto i = 0; i < TEST_SEED_COUNT; i++)
  {
    auto poly_test = generate_polynomial_test_case<T,Policies>(i,max_degree,
      R_min,R_max,
      I_min,I_max,
      precision);

    EXPECT_MINIMIZE_RESULTS(poly_test);
    EXPECT_MINIMIZE_DER_RESULTS(poly_test);
  }
}




