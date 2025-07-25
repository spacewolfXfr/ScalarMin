// Copyright (c) 2025 Mael FEURGARD <mael.feurgard@enac.fr>
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "testing.hpp"

// -------------------- GTest classes -------------------- //

// Using paired types to test accross two kinds of types:
// https://stackoverflow.com/questions/52256953/c-multiple-parameters-with-gtest-typed-test
template<typename PairedType = std::pair<float,OptimizationRounding::standard_rounding<float>>>
class TestPolynomialByFloatType : public testing::Test
{
protected:
  typedef typename PairedType::first_type _T;
  typedef typename PairedType::second_type _Policies;

  static constexpr _T get_precision()
  {
    _T precision;

    if (std::is_same<_T, double>::value)
    {
      precision   = TEST_DOUBLE_PRECISION;
    }
    else if (std::is_same<_T, long double>::value)
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
    size_t max_degree;

    if (std::is_same<_T, double>::value)
    {
      max_degree  = TEST_DOUBLE_POLY_MAX_DEGREE;
    }
    else if (std::is_same<_T, long double>::value)
    {
      max_degree  = TEST_LONG_DOUBLE_POLY_MAX_DEGREE;
    }
    else
    {
      max_degree  = TEST_FLOAT_POLY_MAX_DEGREE;
    }

    return max_degree;
  }

  static constexpr Polynomial<_T,_Policies> root_one_test_case()
  {
    const MinimizeTesting::TestValues<_T> root_one_setup{
        .location   = 1.f,
        .value      = 0.f,
        .init_left  = -10.f,
        .init_right =  10.f,
        .precision  = get_precision(),
      };

    return Polynomial<_T,_Policies>(root_one_setup,{1,-2,1});
  }

  static constexpr std::tuple<_T,_T,_T,_T> roots_in_interval()
  {
    constexpr _T I_min = static_cast<_T>(TEST_INNER_I_MIN);
    constexpr _T I_max = static_cast<_T>(TEST_INNER_I_MAX);
    constexpr _T R_min = static_cast<_T>(TEST_INNER_R_MIN);
    constexpr _T R_max = static_cast<_T>(TEST_INNER_R_MAX);

    static_assert(check_interval_inclusion(I_min,I_max,R_min,R_max));

    return std::make_tuple(I_min,I_max,R_min,R_max);
  }

  static constexpr std::tuple<_T,_T,_T,_T> roots_out_interval()
  {
    constexpr _T I_min = static_cast<_T>(TEST_OUTER_I_MIN);
    constexpr _T I_max = static_cast<_T>(TEST_OUTER_I_MAX);
    constexpr _T R_min = static_cast<_T>(TEST_OUTER_R_MIN);
    constexpr _T R_max = static_cast<_T>(TEST_OUTER_R_MAX);

    static_assert(check_interval_inclusion(R_min,R_max,I_min,I_max));

    return std::make_tuple(I_min,I_max,R_min,R_max);
  }

  static void random_polys_interval_minimize(const std::tuple<_T,_T,_T,_T>& search_and_roots_intervals, int sample_num)
  {
    size_t max_degree = get_max_degree();
    _T precision = get_precision();

    _T I_min = std::get<0>(search_and_roots_intervals);
    _T I_max = std::get<1>(search_and_roots_intervals);
    _T R_min = std::get<2>(search_and_roots_intervals);
    _T R_max = std::get<3>(search_and_roots_intervals);

    for(auto i = 0; i < sample_num; i++)
    {
      auto poly_test = generate_polynomial_test_case<_T,_Policies>(i,max_degree,
        R_min,R_max,
        I_min,I_max,
        precision);

      EXPECT_MINIMIZE_INTERVAL_RESULTS(poly_test);
    }
  }

  static void random_polys_interval_der_minimize(const std::tuple<_T,_T,_T,_T>& search_and_roots_intervals, int sample_num)
  {
    size_t max_degree = get_max_degree();
    _T precision = get_precision();

    _T I_min = std::get<0>(search_and_roots_intervals);
    _T I_max = std::get<1>(search_and_roots_intervals);
    _T R_min = std::get<2>(search_and_roots_intervals);
    _T R_max = std::get<3>(search_and_roots_intervals);

    for(auto i = 0; i < sample_num; i++)
    {
      auto poly_test = generate_polynomial_test_case<_T,_Policies>(i,max_degree,
        R_min,R_max,
        I_min,I_max,
        precision);

      EXPECT_MINIMIZE_INTERVAL_DER_RESULTS(poly_test);
    }
  }

  static void random_polys_brent_minimize(const std::tuple<_T,_T,_T,_T>& search_and_roots_intervals, int sample_num)
  {
    size_t max_degree = get_max_degree();
    _T precision = get_precision();

    _T I_min = std::get<0>(search_and_roots_intervals);
    _T I_max = std::get<1>(search_and_roots_intervals);
    _T R_min = std::get<2>(search_and_roots_intervals);
    _T R_max = std::get<3>(search_and_roots_intervals);

    for(auto i = 0; i < sample_num; i++)
    {
      auto poly_test = generate_polynomial_test_case<_T,_Policies>(i,max_degree,
        R_min,R_max,
        I_min,I_max,
        precision);

      EXPECT_MINIMIZE_BRENT_RESULTS(poly_test);
    }
  }

  static void random_polys_brent_der_minimize(const std::tuple<_T,_T,_T,_T>& search_and_roots_intervals, int sample_num)
  {
    size_t max_degree = get_max_degree();
    _T precision = get_precision();

    _T I_min = std::get<0>(search_and_roots_intervals);
    _T I_max = std::get<1>(search_and_roots_intervals);
    _T R_min = std::get<2>(search_and_roots_intervals);
    _T R_max = std::get<3>(search_and_roots_intervals);

    for(auto i = 0; i < sample_num; i++)
    {
      auto poly_test = generate_polynomial_test_case<_T,_Policies>(i,max_degree,
        R_min,R_max,
        I_min,I_max,
        precision);

      EXPECT_MINIMIZE_BRENT_DER_RESULTS(poly_test);
    }
  }

  TestPolynomialByFloatType() {}

};

// -------------------- Actual testing -------------------- //

// ---------- Test on polynomials ---------- //



// Inspect the effects of float precision and rounding types
using TestTypes = ::testing::Types<
  std::pair<float,OptimizationRounding::standard_rounding<float>>,
  std::pair<float,OptimizationRounding::fast_rounding<float>>,
  std::pair<double,OptimizationRounding::standard_rounding<double>>,
  std::pair<double,OptimizationRounding::fast_rounding<double>>,
  std::pair<long double,OptimizationRounding::standard_rounding<long double>>,
  std::pair<long double,OptimizationRounding::fast_rounding<long double>>
  >;

TYPED_TEST_CASE(TestPolynomialByFloatType,TestTypes);

// Simple test to find the minimum of (X-1)²
TYPED_TEST(TestPolynomialByFloatType, RootOneInterval)
{
  auto root_one_poly = TestFixture::root_one_test_case();

  EXPECT_MINIMIZE_INTERVAL_RESULTS(root_one_poly);

}
TYPED_TEST(TestPolynomialByFloatType, RootOneIntervalDer)
{
  auto root_one_poly = TestFixture::root_one_test_case();

  EXPECT_MINIMIZE_INTERVAL_DER_RESULTS(root_one_poly);

}
TYPED_TEST(TestPolynomialByFloatType, RootOneBrent)
{
  auto root_one_poly = TestFixture::root_one_test_case();

  EXPECT_MINIMIZE_BRENT_RESULTS(root_one_poly);

}
TYPED_TEST(TestPolynomialByFloatType, RootOneBrentDer)
{
  auto root_one_poly = TestFixture::root_one_test_case();

  EXPECT_MINIMIZE_BRENT_DER_RESULTS(root_one_poly);

}


// Test on random polynomials, with some first derivative roots INSIDE the search interval
TYPED_TEST(TestPolynomialByFloatType, RootsInRangeInterval)
{
  auto intervals = TestFixture::roots_in_interval();
  TestFixture::random_polys_interval_minimize(intervals,TEST_SEED_COUNT);
}

TYPED_TEST(TestPolynomialByFloatType, RootsInRangeIntervalDer)
{
  auto intervals = TestFixture::roots_in_interval();
  TestFixture::random_polys_interval_der_minimize(intervals,TEST_SEED_COUNT);
}

TYPED_TEST(TestPolynomialByFloatType, RootsInRangeBrent)
{
  auto intervals = TestFixture::roots_in_interval();
  TestFixture::random_polys_brent_minimize(intervals,TEST_SEED_COUNT);
}

TYPED_TEST(TestPolynomialByFloatType, RootsInRangeBrentDer)
{
  auto intervals = TestFixture::roots_in_interval();
  TestFixture::random_polys_brent_der_minimize(intervals,TEST_SEED_COUNT);
}

// Test on random polynomials, with some first derivative roots OUTSIDE the search interval
TYPED_TEST(TestPolynomialByFloatType, RootsOutRangeInterval)
{
  auto intervals = TestFixture::roots_in_interval();
  TestFixture::random_polys_interval_minimize(intervals,TEST_SEED_COUNT);
}

TYPED_TEST(TestPolynomialByFloatType, RootsOutRangeIntervalDer)
{
  auto intervals = TestFixture::roots_in_interval();
  TestFixture::random_polys_interval_der_minimize(intervals,TEST_SEED_COUNT);
}

TYPED_TEST(TestPolynomialByFloatType, RootsOutRangeBrent)
{
  auto intervals = TestFixture::roots_in_interval();
  TestFixture::random_polys_brent_minimize(intervals,TEST_SEED_COUNT);
}

TYPED_TEST(TestPolynomialByFloatType, RootsOutRangeBrentDer)
{
  auto intervals = TestFixture::roots_in_interval();
  TestFixture::random_polys_brent_der_minimize(intervals,TEST_SEED_COUNT);
}

