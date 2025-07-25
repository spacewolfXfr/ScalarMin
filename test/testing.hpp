// Copyright (c) 2025 Mael FEURGARD <mael.feurgard@enac.fr>
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#pragma once

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
#define TEST_SEED_COUNT 24
#endif

// -- Precision and degree for polynomial tests in function of float precision type -- //
// Precision and degree for float polynomial tests
#if !defined(TEST_FLOAT_PRECISION)
#define TEST_FLOAT_PRECISION 1e-2f
#endif

#if !defined(TEST_FLOAT_POLY_MAX_DEGREE) || TEST_FLOAT_POLY_MAX_DEGREE <= 2
#define TEST_FLOAT_POLY_MAX_DEGREE 5
#endif

// Precision and degree for double float polynomial tests
#if !defined(TEST_DOUBLE_PRECISION)
#define TEST_DOUBLE_PRECISION 1e-3
#endif

#if !defined(TEST_DOUBLE_POLY_MAX_DEGREE) || TEST_DOUBLE_POLY_MAX_DEGREE <= 2
#define TEST_DOUBLE_POLY_MAX_DEGREE 10
#endif

// Precision and degree for long double float polynomial tests
#if !defined(TEST_LONG_DOUBLE_PRECISION)
#define TEST_LONG_DOUBLE_PRECISION 1e-3
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
void EXPECT_MINIMIZE_INTERVAL_RESULTS(MinimizeTesting::TestCase<T,Policies> &c)
{
  T s = c.minimize_interval_method();

  c.print_test_values();
  c.print_params();

  T precision = std::max(c.testVals.precision,
    c.testVals.precision*std::abs(c.testVals.value));

    EXPECT_NEAR(c.f(s),c.testVals.value   ,precision)             << "(Interval method, derivative free) Minimum values do not match";
    ASSERT_NEAR(s     ,c.testVals.location,c.testVals.precision)  << "(Interval method, derivative free) Minimum locations do not match";
}

template<typename T, class Policies>
void EXPECT_MINIMIZE_INTERVAL_DER_RESULTS(MinimizeTesting::TestCaseWithDerivatives<T,Policies> &c)
{
  T s = c.minimize_interval_method_with_derivatives();

  c.print_test_values();
  c.print_params();

  T precision = std::max(c.testVals.precision,
    c.testVals.precision*std::abs(c.testVals.value));

    EXPECT_NEAR(c.f(s),c.testVals.value   ,precision)             << "(Interval method, with derivatives) Minimum values do not match";
    ASSERT_NEAR(s     ,c.testVals.location,c.testVals.precision)  << "(Interval method, with derivatives) Minimum locations do not match";
}

template<typename T, class Policies>
void EXPECT_MINIMIZE_BRENT_RESULTS(MinimizeTesting::TestCaseWithDerivatives<T,Policies> &c)
{
  T s = c.minimize_brent_method();

  c.print_test_values();
  c.print_params();

  T precision = std::max(c.testVals.precision,
    c.testVals.precision*std::abs(c.testVals.value));

    EXPECT_NEAR(c.f(s),c.testVals.value   ,precision)             << "(Brent method, derivatives-free) Minimum values do not match";
    ASSERT_NEAR(s     ,c.testVals.location,c.testVals.precision)  << "(Brent method, derivatives-free) Minimum locations do not match";
}

template<typename T, class Policies>
void EXPECT_MINIMIZE_BRENT_DER_RESULTS(MinimizeTesting::TestCaseWithDerivatives<T,Policies> &c)
{
  T s = c.minimize_optbrent_method();

  c.print_test_values();
  c.print_params();

  T precision = std::max(c.testVals.precision,
    c.testVals.precision*std::abs(c.testVals.value));

    EXPECT_NEAR(c.f(s),c.testVals.value   ,precision)             << "(Brent method, with derivatives) Minimum values do not match";
    ASSERT_NEAR(s     ,c.testVals.location,c.testVals.precision)  << "(Brent method, with derivatives) Minimum locations do not match";
}