// Copyright (c) 2025 Mael FEURGARD <mael.feurgard@enac.fr>
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "LinAccLineCircleTesting.hpp"

// ==================== Actual testing ==================== //

// ========== Test on line-circle distance functions with linear acceleration ========== //

// ---- Brent Methods ---- //
// DISABLED SINCE METHOD IS NOT ACCURATE //
using FloatTestTypes = ::testing::Types<float,double,long double>;

/*
TYPED_TEST_CASE(BrentTestLinAccLineCircle,FloatTestTypes);

TYPED_TEST(BrentTestLinAccLineCircle, EasySpecial)
{
    this->test_special_easy();
}

TYPED_TEST(BrentTestLinAccLineCircle, HardSpecial)
{
    this->test_special_hard();
}

TYPED_TEST(BrentTestLinAccLineCircle, UnitSpeed)
{
  this->test_random(1., TEST_MAX_ACCEL_CIRCLE, TEST_MAX_ACCEL_LINE, TEST_I_MIN,TEST_I_MAX, TEST_SEED_COUNT);
}

TYPED_TEST(BrentTestLinAccLineCircle, LowSpeed)
{
  this->test_random(TEST_LOW_SPEED, TEST_MAX_ACCEL_CIRCLE, TEST_MAX_ACCEL_LINE, TEST_I_MIN,TEST_I_MAX, TEST_SEED_COUNT);
}

TYPED_TEST(BrentTestLinAccLineCircle, HighSpeed)
{
  this->test_random(TEST_HIGH_SPEED, TEST_MAX_ACCEL_CIRCLE, TEST_MAX_ACCEL_LINE, TEST_I_MIN,TEST_I_MAX, TEST_SEED_COUNT);
}

TYPED_TEST(BrentTestLinAccLineCircle, RandomSpeed)
{
  this->test_random(-1., TEST_MAX_ACCEL_CIRCLE, TEST_MAX_ACCEL_LINE, TEST_I_MIN,TEST_I_MAX, TEST_SEED_COUNT);
}

// Tests specifically for Brent with derivatives
TYPED_TEST_SUITE(BrentDerTestLinAccLineCircle,FloatTestTypes);

TYPED_TEST(BrentDerTestLinAccLineCircle, EasySpecial)
{
    this->test_special_easy();
}

TYPED_TEST(BrentDerTestLinAccLineCircle, HardSpecial)
{
    this->test_special_hard();
}
*/

// ---- Interval Methods ---- //

// Inspect the effects of float precision and rounding types
using PairedTestTypes = ::testing::Types<
//   std::pair<float,OptimizationRounding::standard_rounding<float>>,
//   std::pair<float,OptimizationRounding::fast_rounding<float>>,
  std::pair<double,OptimizationRounding::standard_rounding<double>>,
  std::pair<double,OptimizationRounding::fast_rounding<double>>,
  std::pair<long double,OptimizationRounding::standard_rounding<long double>>,
  std::pair<long double,OptimizationRounding::fast_rounding<long double>>
  >;


TYPED_TEST_CASE(IntervalTestLinAccLineCircle,PairedTestTypes);

TYPED_TEST(IntervalTestLinAccLineCircle, EasySpecial)
{
  this->test_special_easy();
}

TYPED_TEST(IntervalTestLinAccLineCircle, HardSpecial)
{
  this->test_special_hard();
}

TYPED_TEST(IntervalTestLinAccLineCircle, UnitSpeed)
{
  this->test_random(1., TEST_MAX_ACCEL_CIRCLE, TEST_MAX_ACCEL_LINE, TEST_I_MIN,TEST_I_MAX, TEST_SEED_COUNT);
}

TYPED_TEST(IntervalTestLinAccLineCircle, LowSpeed)
{
  this->test_random(TEST_LOW_SPEED, TEST_MAX_ACCEL_CIRCLE, TEST_MAX_ACCEL_LINE, TEST_I_MIN,TEST_I_MAX, TEST_SEED_COUNT);
}

TYPED_TEST(IntervalTestLinAccLineCircle, HighSpeed)
{
  this->test_random(TEST_HIGH_SPEED, TEST_MAX_ACCEL_CIRCLE, TEST_MAX_ACCEL_LINE, TEST_I_MIN,TEST_I_MAX, TEST_SEED_COUNT);
}

TYPED_TEST(IntervalTestLinAccLineCircle, RandomSpeed)
{
  this->test_random(-1., TEST_MAX_ACCEL_CIRCLE, TEST_MAX_ACCEL_LINE, TEST_I_MIN,TEST_I_MAX, TEST_SEED_COUNT);
}

// Tests specifically for Interval with derivatives
TYPED_TEST_SUITE(IntervalDerTestLinAccLineCircle,PairedTestTypes);

TYPED_TEST(IntervalDerTestLinAccLineCircle, EasySpecial)
{
    this->test_special_easy();
}

TYPED_TEST(IntervalDerTestLinAccLineCircle, HardSpecial)
{
    this->test_special_hard();
}

// ---- Methods with derivatives ---- //

TYPED_TEST_CASE(WithDerivativesTestLinAccLineCircle,PairedTestTypes);


TYPED_TEST(WithDerivativesTestLinAccLineCircle, UnitSpeed)
{
  this->test_random(1., TEST_MAX_ACCEL_CIRCLE, TEST_MAX_ACCEL_LINE, TEST_I_MIN,TEST_I_MAX, TEST_SEED_COUNT);
}

TYPED_TEST(WithDerivativesTestLinAccLineCircle, LowSpeed)
{
  this->test_random(TEST_LOW_SPEED, TEST_MAX_ACCEL_CIRCLE, TEST_MAX_ACCEL_LINE, TEST_I_MIN,TEST_I_MAX, TEST_SEED_COUNT);
}

TYPED_TEST(WithDerivativesTestLinAccLineCircle, HighSpeed)
{
  this->test_random(TEST_HIGH_SPEED, TEST_MAX_ACCEL_CIRCLE, TEST_MAX_ACCEL_LINE, TEST_I_MIN,TEST_I_MAX, TEST_SEED_COUNT);
}

TYPED_TEST(WithDerivativesTestLinAccLineCircle, RandomSpeed)
{
  this->test_random(-1., TEST_MAX_ACCEL_CIRCLE, TEST_MAX_ACCEL_LINE, TEST_I_MIN,TEST_I_MAX, TEST_SEED_COUNT);
}

// ---- Methods without derivatives ---- //

TYPED_TEST_CASE(NoDerivativesTestLinAccLineCircle,PairedTestTypes);


TYPED_TEST(NoDerivativesTestLinAccLineCircle, UnitSpeed)
{
  this->test_random(1., TEST_MAX_ACCEL_CIRCLE, TEST_MAX_ACCEL_LINE, TEST_I_MIN,TEST_I_MAX, TEST_SEED_COUNT);
}

TYPED_TEST(NoDerivativesTestLinAccLineCircle, LowSpeed)
{
  this->test_random(TEST_LOW_SPEED, TEST_MAX_ACCEL_CIRCLE, TEST_MAX_ACCEL_LINE, TEST_I_MIN,TEST_I_MAX, TEST_SEED_COUNT);
}

TYPED_TEST(NoDerivativesTestLinAccLineCircle, HighSpeed)
{
  this->test_random(TEST_HIGH_SPEED, TEST_MAX_ACCEL_CIRCLE, TEST_MAX_ACCEL_LINE, TEST_I_MIN,TEST_I_MAX, TEST_SEED_COUNT);
}

TYPED_TEST(NoDerivativesTestLinAccLineCircle, RandomSpeed)
{
  this->test_random(-1., TEST_MAX_ACCEL_CIRCLE, TEST_MAX_ACCEL_LINE, TEST_I_MIN,TEST_I_MAX, TEST_SEED_COUNT);
}
