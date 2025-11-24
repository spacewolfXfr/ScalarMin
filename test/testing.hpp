// Copyright (c) 2025 Mael FEURGARD <mael.feurgard@enac.fr>
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#pragma once

#include <gtest/gtest.h>
#include <tuple>

#include "RoundingPolicies.hpp"
#include "testInterface.hpp"

// -------------------- Configurable constants -------------------- //


// -- Number of random test to generate -- //
#if !defined(TEST_SEED_COUNT) || TEST_SEED_COUNT <= 0
#define TEST_SEED_COUNT 24
#endif

// -- Precision for tests as function of float subtype -- //
#if !defined(TEST_FLOAT_PRECISION)
#define TEST_FLOAT_PRECISION 1e-2f
#endif

#if !defined(TEST_DOUBLE_PRECISION)
#define TEST_DOUBLE_PRECISION 1e-3
#endif

#if !defined(TEST_LONG_DOUBLE_PRECISION)
#define TEST_LONG_DOUBLE_PRECISION 1e-5
#endif


// ==================== Helper functions ==================== //

template<typename T>
using Default_policies = OptimizationRounding::fast_rounding<T>;

// ==================== GTest classes ==================== //

template<typename _T, class _Policies=Default_policies<_T>>
class GeneralTestByFloatType : public testing::Test
{
public:
  GeneralTestByFloatType() {}
protected:
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

  virtual _T find_solution(MinimizeTesting::TestCaseWithDerivatives<_T,_Policies> &c) = 0;
  virtual _T find_another_solution(MinimizeTesting::TestCaseWithDerivatives<_T,_Policies> &c)
  {
    std::cerr << "WARNING: No alternative solving method provided" << std::endl;
    return find_solution(c);
  }

  void test_and_check(MinimizeTesting::TestCaseWithDerivatives<_T,_Policies> &c)
  {
    _T s = find_solution(c);

    c.print_test_values();
    c.print_params();

    _T precision = std::max(c.testVals.precision,
      c.testVals.precision*std::abs(c.testVals.value));

    EXPECT_NEAR(c.f(s),c.testVals.value   ,precision)             << "Minimum values do not match";
    EXPECT_NEAR(s     ,c.testVals.location,c.testVals.precision)  << "Minimum locations do not match";
  }

  void test_and_check_bis(MinimizeTesting::TestCaseWithDerivatives<_T,_Policies> &c)
  {
    _T s = find_another_solution(c);

    c.print_test_values();
    c.print_params();

    _T precision = std::max(c.testVals.precision,
      c.testVals.precision*std::abs(c.testVals.value));

    EXPECT_NEAR(c.f(s),c.testVals.value   ,precision)             << "Minimum values do not match";
    EXPECT_NEAR(s     ,c.testVals.location,c.testVals.precision)  << "Minimum locations do not match";
  }

  void agreement_test(MinimizeTesting::TestCaseWithDerivatives<_T,_Policies> &c)
  {
    _T s1 = find_solution(c);
    _T s2 = find_another_solution(c);

    _T v1 = c.f(s1);
    _T v2 = c.f(s2);

    _T v_precision = std::max(c.testVals.precision,
      std::min(c.testVals.precision*std::abs(v1), c.testVals.precision*std::abs(v2)));

    EXPECT_NEAR(v1,v2, v_precision)           << "Minimum values do not match";
    EXPECT_NEAR(s1,s2, c.testVals.precision)  << "Minimum locations do not match";

  }
};
