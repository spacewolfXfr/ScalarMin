// Copyright (c) 2025 Mael FEURGARD <mael.feurgard@enac.fr>
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "LinAccCircleCircleTesting.hpp"

// ==================== Actual testing ==================== //

// ========== Test on circle-circle distance functions with linear acceleration ========== //

// ---- Brent Methods ---- //
// DISABLED SINCE METHOD IS NOT ACCURATE //
using FloatTestTypes = ::testing::Types<float,double,long double>;

/*
TYPED_TEST_CASE(BrentTestLinAccCircleCircle,FloatTestTypes);

TYPED_TEST(BrentTestLinAccCircleCircle, EasySpecial)
{
    this->test_special_easy();
}

TYPED_TEST(BrentTestLinAccCircleCircle, HardSpecial)
{
    this->test_special_hard();
}

TYPED_TEST(BrentTestLinAccCircleCircle, UnitSpeed)
{
  this->test_random(1., TEST_MAX_ACCEL, TEST_I_MIN,TEST_I_MAX, TEST_SEED_COUNT);
}

TYPED_TEST(BrentTestLinAccCircleCircle, LowSpeed)
{
  this->test_random(TEST_LOW_SPEED, TEST_MAX_ACCEL, TEST_I_MIN,TEST_I_MAX, TEST_SEED_COUNT);
}

TYPED_TEST(BrentTestLinAccCircleCircle, HighSpeed)
{
  this->test_random(TEST_HIGH_SPEED, TEST_MAX_ACCEL, TEST_I_MIN,TEST_I_MAX, TEST_SEED_COUNT);
}

TYPED_TEST(BrentTestLinAccCircleCircle, RandomSpeed)
{
  this->test_random(-1., TEST_MAX_ACCEL, TEST_I_MIN,TEST_I_MAX, TEST_SEED_COUNT);
}

// Tests specifically for Brent with derivatives
TYPED_TEST_SUITE(BrentDerTestLinAccCircleCircle,FloatTestTypes);

TYPED_TEST(BrentDerTestLinAccCircleCircle, EasySpecial)
{
    this->test_special_easy();
}

TYPED_TEST(BrentDerTestLinAccCircleCircle, HardSpecial)
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


TYPED_TEST_CASE(IntervalTestLinAccCircleCircle,PairedTestTypes);

TYPED_TEST(IntervalTestLinAccCircleCircle, EasySpecial)
{
    this->test_special_easy();
}

TYPED_TEST(IntervalTestLinAccCircleCircle, HardSpecial)
{
    this->test_special_hard();
}

TYPED_TEST(IntervalTestLinAccCircleCircle, UnitSpeed)
{
  this->test_random(1., TEST_MAX_ACCEL, TEST_I_MIN,TEST_I_MAX, TEST_SEED_COUNT);
}

TYPED_TEST(IntervalTestLinAccCircleCircle, LowSpeed)
{
  this->test_random(TEST_LOW_SPEED, TEST_MAX_ACCEL, TEST_I_MIN,TEST_I_MAX, TEST_SEED_COUNT);
}

TYPED_TEST(IntervalTestLinAccCircleCircle, HighSpeed)
{
  this->test_random(TEST_HIGH_SPEED, TEST_MAX_ACCEL, TEST_I_MIN,TEST_I_MAX, TEST_SEED_COUNT);
}

TYPED_TEST(IntervalTestLinAccCircleCircle, RandomSpeed)
{
  this->test_random(-1., TEST_MAX_ACCEL, TEST_I_MIN,TEST_I_MAX, TEST_SEED_COUNT);
}

// Tests specifically for Interval with derivatives
TYPED_TEST_SUITE(IntervalDerTestLinAccCircleCircle,PairedTestTypes);

TYPED_TEST(IntervalDerTestLinAccCircleCircle, EasySpecial)
{
    this->test_special_easy();
}

TYPED_TEST(IntervalDerTestLinAccCircleCircle, HardSpecial)
{
    this->test_special_hard();
}

// ---- Methods with derivatives ---- //

TYPED_TEST_CASE(WithDerivativesTestLinAccCircleCircle,PairedTestTypes);


TYPED_TEST(WithDerivativesTestLinAccCircleCircle, UnitSpeed)
{
  this->test_random(1., TEST_MAX_ACCEL, TEST_I_MIN,TEST_I_MAX, TEST_SEED_COUNT);
}

TYPED_TEST(WithDerivativesTestLinAccCircleCircle, LowSpeed)
{
  this->test_random(TEST_LOW_SPEED, TEST_MAX_ACCEL, TEST_I_MIN,TEST_I_MAX, TEST_SEED_COUNT);
}

TYPED_TEST(WithDerivativesTestLinAccCircleCircle, HighSpeed)
{
  this->test_random(TEST_HIGH_SPEED, TEST_MAX_ACCEL, TEST_I_MIN,TEST_I_MAX, TEST_SEED_COUNT);
}

TYPED_TEST(WithDerivativesTestLinAccCircleCircle, RandomSpeed)
{
  this->test_random(-1., TEST_MAX_ACCEL, TEST_I_MIN,TEST_I_MAX, TEST_SEED_COUNT);
}

// ---- Methods without derivatives ---- //

TYPED_TEST_CASE(NoDerivativesTestLinAccCircleCircle,PairedTestTypes);


TYPED_TEST(NoDerivativesTestLinAccCircleCircle, UnitSpeed)
{
  this->test_random(1., TEST_MAX_ACCEL, TEST_I_MIN,TEST_I_MAX, TEST_SEED_COUNT);
}

TYPED_TEST(NoDerivativesTestLinAccCircleCircle, LowSpeed)
{
  this->test_random(TEST_LOW_SPEED, TEST_MAX_ACCEL, TEST_I_MIN,TEST_I_MAX, TEST_SEED_COUNT);
}

TYPED_TEST(NoDerivativesTestLinAccCircleCircle, HighSpeed)
{
  this->test_random(TEST_HIGH_SPEED, TEST_MAX_ACCEL, TEST_I_MIN,TEST_I_MAX, TEST_SEED_COUNT);
}

TYPED_TEST(NoDerivativesTestLinAccCircleCircle, RandomSpeed)
{
  this->test_random(-1., TEST_MAX_ACCEL, TEST_I_MIN,TEST_I_MAX, TEST_SEED_COUNT);
}
