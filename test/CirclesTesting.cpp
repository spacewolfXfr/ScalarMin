// Copyright (c) 2025 Mael FEURGARD <mael.feurgard@enac.fr>
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "CirclesTesting.hpp"

// ==================== Actual testing ==================== //

// ========== Test on circle-circle distance functions ========== //

// ---- Brent Methods ---- //

using FloatTestTypes = ::testing::Types<float,double,long double>;

TYPED_TEST_CASE(BrentTestCircleCircle,FloatTestTypes);

TYPED_TEST(BrentTestCircleCircle, EasySpecial)
{
    this->test_special_easy();
}

TYPED_TEST(BrentTestCircleCircle, HardSpecial)
{
    this->test_special_hard();
}

TYPED_TEST(BrentTestCircleCircle, UnitSpeed)
{
  this->test_random(1., TEST_I_MIN,TEST_I_MAX, TEST_SEED_COUNT);
}

TYPED_TEST(BrentTestCircleCircle, LowSpeed)
{
  this->test_random(TEST_LOW_SPEED, TEST_I_MIN,TEST_I_MAX, TEST_SEED_COUNT);
}

TYPED_TEST(BrentTestCircleCircle, HighSpeed)
{
  this->test_random(TEST_HIGH_SPEED, TEST_I_MIN,TEST_I_MAX, TEST_SEED_COUNT);
}

TYPED_TEST(BrentTestCircleCircle, RandomSpeed)
{
  this->test_random(-1., TEST_I_MIN,TEST_I_MAX, TEST_SEED_COUNT);
}

// Tests specifically for Brent with derivatives
TYPED_TEST_SUITE(BrentDerTestCircleCircle,FloatTestTypes);

TYPED_TEST(BrentDerTestCircleCircle, EasySpecial)
{
    this->test_special_easy();
}

TYPED_TEST(BrentDerTestCircleCircle, HardSpecial)
{
    this->test_special_hard();
}


// ---- Interval Methods ---- //

// Inspect the effects of float precision and rounding types
using PairedTestTypes = ::testing::Types<
  std::pair<float,OptimizationRounding::standard_rounding<float>>,
  std::pair<float,OptimizationRounding::fast_rounding<float>>,
  std::pair<double,OptimizationRounding::standard_rounding<double>>,
  std::pair<double,OptimizationRounding::fast_rounding<double>>,
  std::pair<long double,OptimizationRounding::standard_rounding<long double>>,
  std::pair<long double,OptimizationRounding::fast_rounding<long double>>
  >;


TYPED_TEST_CASE(IntervalTestCircleCircle,PairedTestTypes);

TYPED_TEST(IntervalTestCircleCircle, EasySpecial)
{
    this->test_special_easy();
}

TYPED_TEST(IntervalTestCircleCircle, HardSpecial)
{
    this->test_special_hard();
}

TYPED_TEST(IntervalTestCircleCircle, UnitSpeed)
{
  this->test_random(1., TEST_I_MIN,TEST_I_MAX, TEST_SEED_COUNT);
}

TYPED_TEST(IntervalTestCircleCircle, LowSpeed)
{
  this->test_random(TEST_LOW_SPEED, TEST_I_MIN,TEST_I_MAX, TEST_SEED_COUNT);
}

TYPED_TEST(IntervalTestCircleCircle, HighSpeed)
{
  this->test_random(TEST_HIGH_SPEED, TEST_I_MIN,TEST_I_MAX, TEST_SEED_COUNT);
}

TYPED_TEST(IntervalTestCircleCircle, RandomSpeed)
{
  this->test_random(-1., TEST_I_MIN,TEST_I_MAX, TEST_SEED_COUNT);
}

// Tests specifically for Interval with derivatives
TYPED_TEST_SUITE(IntervalDerTestCircleCircle,PairedTestTypes);

TYPED_TEST(IntervalDerTestCircleCircle, EasySpecial)
{
    this->test_special_easy();
}

TYPED_TEST(IntervalDerTestCircleCircle, HardSpecial)
{
    this->test_special_hard();
}

// ---- Methods with derivatives ---- //

TYPED_TEST_CASE(WithDerivativesTestCircleCircle,PairedTestTypes);


TYPED_TEST(WithDerivativesTestCircleCircle, UnitSpeed)
{
  this->test_random(1., TEST_I_MIN,TEST_I_MAX, TEST_SEED_COUNT);
}

TYPED_TEST(WithDerivativesTestCircleCircle, LowSpeed)
{
  this->test_random(TEST_LOW_SPEED, TEST_I_MIN,TEST_I_MAX, TEST_SEED_COUNT);
}

TYPED_TEST(WithDerivativesTestCircleCircle, HighSpeed)
{
  this->test_random(TEST_HIGH_SPEED, TEST_I_MIN,TEST_I_MAX, TEST_SEED_COUNT);
}

TYPED_TEST(WithDerivativesTestCircleCircle, RandomSpeed)
{
  this->test_random(-1., TEST_I_MIN,TEST_I_MAX, TEST_SEED_COUNT);
}

// ---- Methods without derivatives ---- //

TYPED_TEST_CASE(NoDerivativesTestCircleCircle,PairedTestTypes);


TYPED_TEST(NoDerivativesTestCircleCircle, UnitSpeed)
{
  this->test_random(1., TEST_I_MIN,TEST_I_MAX, TEST_SEED_COUNT);
}

TYPED_TEST(NoDerivativesTestCircleCircle, LowSpeed)
{
  this->test_random(TEST_LOW_SPEED, TEST_I_MIN,TEST_I_MAX, TEST_SEED_COUNT);
}

TYPED_TEST(NoDerivativesTestCircleCircle, HighSpeed)
{
  this->test_random(TEST_HIGH_SPEED, TEST_I_MIN,TEST_I_MAX, TEST_SEED_COUNT);
}

TYPED_TEST(NoDerivativesTestCircleCircle, RandomSpeed)
{
  this->test_random(-1., TEST_I_MIN,TEST_I_MAX, TEST_SEED_COUNT);
}