// Copyright (c) 2025 Mael FEURGARD <mael.feurgard@enac.fr>
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "LinAccLineCircleVDistTesting.hpp"

// ==================== Actual testing ==================== //

// ========== Test on line-circle distance functions with linear acceleration and VDist correction ========== //

// ---- Brent Methods ---- //
// DISABLED SINCE METHOD IS NOT ACCURATE //
using FloatTestTypes = ::testing::Types<float,double,long double>;

/*
TYPED_TEST_CASE(BrentTestLinAccLineCircleVDist,FloatTestTypes);

TYPED_TEST(BrentTestLinAccLineCircleVDist, EasySpecial)
{
    this->test_special_easy();
}

TYPED_TEST(BrentTestLinAccLineCircleVDist, HardSpecial)
{
    this->test_special_hard();
}

TYPED_TEST(BrentTestLinAccLineCircleVDist, UnitSpeed)
{
  this->test_random(1., TEST_MAX_ACCEL_CIRCLE, TEST_MAX_ACCEL_LINE, TEST_MAX_TDIST, TEST_I_MIN,TEST_I_MAX, TEST_SEED_COUNT);
}

TYPED_TEST(BrentTestLinAccLineCircleVDist, LowSpeed)
{
  this->test_random(TEST_LOW_SPEED, TEST_MAX_ACCEL_CIRCLE, TEST_MAX_ACCEL_LINE, TEST_MAX_TDIST, TEST_I_MIN,TEST_I_MAX, TEST_SEED_COUNT);
}

TYPED_TEST(BrentTestLinAccLineCircleVDist, HighSpeed)
{
  this->test_random(TEST_HIGH_SPEED, TEST_MAX_ACCEL_CIRCLE, TEST_MAX_ACCEL_LINE, TEST_MAX_TDIST, TEST_I_MIN,TEST_I_MAX, TEST_SEED_COUNT);
}

TYPED_TEST(BrentTestLinAccLineCircleVDist, RandomSpeed)
{
  this->test_random(-1., TEST_MAX_ACCEL_CIRCLE, TEST_MAX_ACCEL_LINE, TEST_MAX_TDIST, TEST_I_MIN,TEST_I_MAX, TEST_SEED_COUNT);
}

// Tests specifically for Brent with derivatives
TYPED_TEST_SUITE(BrentDerTestLinAccLineCircleVDist,FloatTestTypes);

TYPED_TEST(BrentDerTestLinAccLineCircleVDist, EasySpecial)
{
    this->test_special_easy();
}

TYPED_TEST(BrentDerTestLinAccLineCircleVDist, HardSpecial)
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


TYPED_TEST_CASE(IntervalTestLinAccLineCircleVDist,PairedTestTypes);

TYPED_TEST(IntervalTestLinAccLineCircleVDist, EasySpecial)
{
  this->test_special_easy();
}

TYPED_TEST(IntervalTestLinAccLineCircleVDist, HardSpecial)
{
  this->test_special_hard();
}

TYPED_TEST(IntervalTestLinAccLineCircleVDist, UnitSpeed)
{
  this->test_random(1., TEST_MAX_ACCEL_CIRCLE, TEST_MAX_ACCEL_LINE, TEST_MAX_TDIST, TEST_I_MIN,TEST_I_MAX, TEST_SEED_COUNT);
}

TYPED_TEST(IntervalTestLinAccLineCircleVDist, LowSpeed)
{
  this->test_random(TEST_LOW_SPEED, TEST_MAX_ACCEL_CIRCLE, TEST_MAX_ACCEL_LINE, TEST_MAX_TDIST, TEST_I_MIN,TEST_I_MAX, TEST_SEED_COUNT);
}

TYPED_TEST(IntervalTestLinAccLineCircleVDist, HighSpeed)
{
  this->test_random(TEST_HIGH_SPEED, TEST_MAX_ACCEL_CIRCLE, TEST_MAX_ACCEL_LINE, TEST_MAX_TDIST, TEST_I_MIN,TEST_I_MAX, TEST_SEED_COUNT);
}

TYPED_TEST(IntervalTestLinAccLineCircleVDist, RandomSpeed)
{
  this->test_random(-1., TEST_MAX_ACCEL_CIRCLE, TEST_MAX_ACCEL_LINE, TEST_MAX_TDIST, TEST_I_MIN,TEST_I_MAX, TEST_SEED_COUNT);
}

// Tests specifically for Interval with derivatives
TYPED_TEST_SUITE(IntervalDerTestLinAccLineCircleVDist,PairedTestTypes);

TYPED_TEST(IntervalDerTestLinAccLineCircleVDist, EasySpecial)
{
    this->test_special_easy();
}

TYPED_TEST(IntervalDerTestLinAccLineCircleVDist, HardSpecial)
{
    this->test_special_hard();
}

// ---- Methods with derivatives ---- //

TYPED_TEST_CASE(WithDerivativesTestLinAccLineCircleVDist,PairedTestTypes);


TYPED_TEST(WithDerivativesTestLinAccLineCircleVDist, UnitSpeed)
{
  this->test_random(1., TEST_MAX_ACCEL_CIRCLE, TEST_MAX_ACCEL_LINE, TEST_MAX_TDIST, TEST_I_MIN,TEST_I_MAX, TEST_SEED_COUNT);
}

TYPED_TEST(WithDerivativesTestLinAccLineCircleVDist, LowSpeed)
{
  this->test_random(TEST_LOW_SPEED, TEST_MAX_ACCEL_CIRCLE, TEST_MAX_ACCEL_LINE, TEST_MAX_TDIST, TEST_I_MIN,TEST_I_MAX, TEST_SEED_COUNT);
}

TYPED_TEST(WithDerivativesTestLinAccLineCircleVDist, HighSpeed)
{
  this->test_random(TEST_HIGH_SPEED, TEST_MAX_ACCEL_CIRCLE, TEST_MAX_ACCEL_LINE, TEST_MAX_TDIST, TEST_I_MIN,TEST_I_MAX, TEST_SEED_COUNT);
}

TYPED_TEST(WithDerivativesTestLinAccLineCircleVDist, RandomSpeed)
{
  this->test_random(-1., TEST_MAX_ACCEL_CIRCLE, TEST_MAX_ACCEL_LINE, TEST_MAX_TDIST, TEST_I_MIN,TEST_I_MAX, TEST_SEED_COUNT);
}

// ---- Methods without derivatives ---- //

TYPED_TEST_CASE(NoDerivativesTestLinAccLineCircleVDist,PairedTestTypes);


TYPED_TEST(NoDerivativesTestLinAccLineCircleVDist, UnitSpeed)
{
  this->test_random(1., TEST_MAX_ACCEL_CIRCLE, TEST_MAX_ACCEL_LINE, TEST_MAX_TDIST, TEST_I_MIN,TEST_I_MAX, TEST_SEED_COUNT);
}

TYPED_TEST(NoDerivativesTestLinAccLineCircleVDist, LowSpeed)
{
  this->test_random(TEST_LOW_SPEED, TEST_MAX_ACCEL_CIRCLE, TEST_MAX_ACCEL_LINE, TEST_MAX_TDIST, TEST_I_MIN,TEST_I_MAX, TEST_SEED_COUNT);
}

TYPED_TEST(NoDerivativesTestLinAccLineCircleVDist, HighSpeed)
{
  this->test_random(TEST_HIGH_SPEED, TEST_MAX_ACCEL_CIRCLE, TEST_MAX_ACCEL_LINE, TEST_MAX_TDIST, TEST_I_MIN,TEST_I_MAX, TEST_SEED_COUNT);
}

TYPED_TEST(NoDerivativesTestLinAccLineCircleVDist, RandomSpeed)
{
  this->test_random(-1., TEST_MAX_ACCEL_CIRCLE, TEST_MAX_ACCEL_LINE, TEST_MAX_TDIST, TEST_I_MIN,TEST_I_MAX, TEST_SEED_COUNT);
}
