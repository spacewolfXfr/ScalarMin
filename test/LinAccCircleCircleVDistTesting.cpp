// Copyright (c) 2025 Mael FEURGARD <mael.feurgard@enac.fr>
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "LinAccCircleCircleVDistTesting.hpp"

// ==================== Actual testing ==================== //

// ========== Test on circle-circle distance functions with linear acceleration and VDist correction ========== //

// ---- Brent Methods ---- //
// DISABLED SINCE METHOD IS NOT ACCURATE //
using FloatTestTypes = ::testing::Types<float,double,long double>;

/*
TYPED_TEST_CASE(BrentTestLinAccCircleCircleVDist,FloatTestTypes);

TYPED_TEST(BrentTestLinAccCircleCircleVDist, EasySpecial)
{
    this->test_special_easy();
}

TYPED_TEST(BrentTestLinAccCircleCircleVDist, HardSpecial)
{
    this->test_special_hard();
}

TYPED_TEST(BrentTestLinAccCircleCircleVDist, UnitSpeed)
{
  this->test_random(1., TEST_MAX_ACCEL, TEST_MAX_TDIST, TEST_I_MIN,TEST_I_MAX, TEST_SEED_COUNT);
}

TYPED_TEST(BrentTestLinAccCircleCircleVDist, LowSpeed)
{
  this->test_random(TEST_LOW_SPEED, TEST_MAX_ACCEL, TEST_MAX_TDIST, TEST_I_MIN,TEST_I_MAX, TEST_SEED_COUNT);
}

TYPED_TEST(BrentTestLinAccCircleCircleVDist, HighSpeed)
{
  this->test_random(TEST_HIGH_SPEED, TEST_MAX_ACCEL, TEST_MAX_TDIST, TEST_I_MIN,TEST_I_MAX, TEST_SEED_COUNT);
}

TYPED_TEST(BrentTestLinAccCircleCircleVDist, RandomSpeed)
{
  this->test_random(-1., TEST_MAX_ACCEL, TEST_MAX_TDIST, TEST_I_MIN,TEST_I_MAX, TEST_SEED_COUNT);
}

// Tests specifically for Brent with derivatives
TYPED_TEST_SUITE(BrentDerTestLinAccCircleCircleVDist,FloatTestTypes);

TYPED_TEST(BrentDerTestLinAccCircleCircleVDist, EasySpecial)
{
    this->test_special_easy();
}

TYPED_TEST(BrentDerTestLinAccCircleCircleVDist, HardSpecial)
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


TYPED_TEST_CASE(IntervalTestLinAccCircleCircleVDist,PairedTestTypes);

TYPED_TEST(IntervalTestLinAccCircleCircleVDist, EasySpecial)
{
    this->test_special_easy();
}

TYPED_TEST(IntervalTestLinAccCircleCircleVDist, HardSpecial)
{
    this->test_special_hard();
}

TYPED_TEST(IntervalTestLinAccCircleCircleVDist, UnitSpeed)
{
  this->test_random(1., TEST_MAX_ACCEL, TEST_MAX_TDIST, TEST_I_MIN,TEST_I_MAX, TEST_SEED_COUNT);
}

TYPED_TEST(IntervalTestLinAccCircleCircleVDist, LowSpeed)
{
  this->test_random(TEST_LOW_SPEED, TEST_MAX_ACCEL, TEST_MAX_TDIST, TEST_I_MIN,TEST_I_MAX, TEST_SEED_COUNT);
}

TYPED_TEST(IntervalTestLinAccCircleCircleVDist, HighSpeed)
{
  this->test_random(TEST_HIGH_SPEED, TEST_MAX_ACCEL, TEST_MAX_TDIST, TEST_I_MIN,TEST_I_MAX, TEST_SEED_COUNT);
}

TYPED_TEST(IntervalTestLinAccCircleCircleVDist, RandomSpeed)
{
  this->test_random(-1., TEST_MAX_ACCEL, TEST_MAX_TDIST, TEST_I_MIN,TEST_I_MAX, TEST_SEED_COUNT);
}

// Tests specifically for Interval with derivatives
TYPED_TEST_SUITE(IntervalDerTestLinAccCircleCircleVDist,PairedTestTypes);

TYPED_TEST(IntervalDerTestLinAccCircleCircleVDist, EasySpecial)
{
    this->test_special_easy();
}

TYPED_TEST(IntervalDerTestLinAccCircleCircleVDist, HardSpecial)
{
    this->test_special_hard();
}

// ---- Methods with derivatives ---- //

TYPED_TEST_CASE(WithDerivativesTestLinAccCircleCircleVDist,PairedTestTypes);


TYPED_TEST(WithDerivativesTestLinAccCircleCircleVDist, UnitSpeed)
{
  this->test_random(1., TEST_MAX_ACCEL, TEST_MAX_TDIST, TEST_I_MIN,TEST_I_MAX, TEST_SEED_COUNT);
}

TYPED_TEST(WithDerivativesTestLinAccCircleCircleVDist, LowSpeed)
{
  this->test_random(TEST_LOW_SPEED, TEST_MAX_ACCEL, TEST_MAX_TDIST, TEST_I_MIN,TEST_I_MAX, TEST_SEED_COUNT);
}

TYPED_TEST(WithDerivativesTestLinAccCircleCircleVDist, HighSpeed)
{
  this->test_random(TEST_HIGH_SPEED, TEST_MAX_ACCEL, TEST_MAX_TDIST, TEST_I_MIN,TEST_I_MAX, TEST_SEED_COUNT);
}

TYPED_TEST(WithDerivativesTestLinAccCircleCircleVDist, RandomSpeed)
{
  this->test_random(-1., TEST_MAX_ACCEL, TEST_MAX_TDIST, TEST_I_MIN,TEST_I_MAX, TEST_SEED_COUNT);
}

// ---- Methods without derivatives ---- //

TYPED_TEST_CASE(NoDerivativesTestLinAccCircleCircleVDist,PairedTestTypes);


TYPED_TEST(NoDerivativesTestLinAccCircleCircleVDist, UnitSpeed)
{
  this->test_random(1., TEST_MAX_ACCEL, TEST_MAX_TDIST, TEST_I_MIN,TEST_I_MAX, TEST_SEED_COUNT);
}

TYPED_TEST(NoDerivativesTestLinAccCircleCircleVDist, LowSpeed)
{
  this->test_random(TEST_LOW_SPEED, TEST_MAX_ACCEL, TEST_MAX_TDIST, TEST_I_MIN,TEST_I_MAX, TEST_SEED_COUNT);
}

TYPED_TEST(NoDerivativesTestLinAccCircleCircleVDist, HighSpeed)
{
  this->test_random(TEST_HIGH_SPEED, TEST_MAX_ACCEL, TEST_MAX_TDIST, TEST_I_MIN,TEST_I_MAX, TEST_SEED_COUNT);
}

TYPED_TEST(NoDerivativesTestLinAccCircleCircleVDist, RandomSpeed)
{
  this->test_random(-1., TEST_MAX_ACCEL, TEST_MAX_TDIST, TEST_I_MIN,TEST_I_MAX, TEST_SEED_COUNT);
}
