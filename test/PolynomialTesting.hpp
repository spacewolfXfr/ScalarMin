// Copyright (c) 2025 Mael FEURGARD <mael.feurgard@enac.fr>
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#pragma once

#include "testing.hpp"
#include "polynomials.hpp"

// ==================== Configurable constants ==================== //

// -- Polynomial degree as function of the float type -- //
// Higher degrees are more numerically unstable

#if !defined(TEST_FLOAT_POLY_MAX_DEGREE) || TEST_FLOAT_POLY_MAX_DEGREE <= 2
#define TEST_FLOAT_POLY_MAX_DEGREE 5
#endif

#if !defined(TEST_DOUBLE_POLY_MAX_DEGREE) || TEST_DOUBLE_POLY_MAX_DEGREE <= 2
#define TEST_DOUBLE_POLY_MAX_DEGREE 10
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

// ==================== Helpers ==================== //

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

// ==================== GTest classes ==================== //

template<typename _T, class _Policies=Default_policies<_T>>
class TestPolynomial : public GeneralTestByFloatType<_T,_Policies>
{
public:
  TestPolynomial() {}
protected:
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

  void test_root_one()
  {
    Polynomial<_T,_Policies> testcase = root_one_test_case<_T,_Policies>(TestPolynomial<_T,_Policies>::get_precision());

    this->test_and_check(testcase);
  }

  void test_right_edge()
  {
    Polynomial<_T,_Policies> testcase = right_edge_test_case<_T,_Policies>(TestPolynomial<_T,_Policies>::get_precision());

    this->test_and_check(testcase);
  }

  void test_left_edge()
  {
    Polynomial<_T,_Policies> testcase = left_edge_test_case<_T,_Policies>(TestPolynomial<_T,_Policies>::get_precision());

    this->test_and_check(testcase);
  }

  void test_random(const std::tuple<_T,_T,_T,_T>& search_and_roots_intervals, int sample_num)
  {
    size_t max_degree = get_max_degree();
    _T precision = TestPolynomial<_T,_Policies>::get_precision();

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

      this->test_and_check(poly_test);
    }
  }
};

template<typename _T>
class BrentTestCircleCircle : public TestPolynomial<_T,Default_policies<_T>>
{
public:
  BrentTestCircleCircle() {}
protected:
  using _Policies = Default_policies<_T>;

  _T find_solution(MinimizeTesting::TestCaseWithDerivatives<_T,Default_policies<_T>> &c) override
  {
    return c.minimize_brent_method();
  }
};

template<typename _T>
class BrentDerTestPolynomial : public TestPolynomial<_T,Default_policies<_T>>
{
public:
  BrentDerTestPolynomial() {}
protected:
  using _Policies = Default_policies<_T>;

  _T find_solution(MinimizeTesting::TestCaseWithDerivatives<_T,Default_policies<_T>> &c) override
  {
    return c.minimize_optbrent_method();
  }
};


// Using paired types to test accross two kinds of types:
// https://stackoverflow.com/questions/52256953/c-multiple-parameters-with-gtest-typed-test
template<typename PairedType = std::pair<float,OptimizationRounding::standard_rounding<float>>>
class IntervalTestCircleCircle : public TestPolynomial<typename PairedType::first_type, typename PairedType::second_type>
{
public:
  IntervalTestCircleCircle() {}
protected: 
  typedef typename PairedType::first_type _T;
  typedef typename PairedType::second_type _Policies;

  _T find_solution(MinimizeTesting::TestCaseWithDerivatives<_T,_Policies> &c) override
  {
    return c.minimize_interval_method();
  }
};

template<typename PairedType = std::pair<float,OptimizationRounding::standard_rounding<float>>>
class IntervalDerTestPolynomial : public TestPolynomial<typename PairedType::first_type, typename PairedType::second_type>
{
public:
  IntervalDerTestPolynomial() {}
protected: 
  typedef typename PairedType::first_type _T;
  typedef typename PairedType::second_type _Policies;

  _T find_solution(MinimizeTesting::TestCaseWithDerivatives<_T,_Policies> &c) override
  {
    return c.minimize_interval_method_with_derivatives();
  }
};