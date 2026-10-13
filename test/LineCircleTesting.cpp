// Copyright (c) 2025 Mael FEURGARD <mael.feurgard@enac.fr>
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "LineCircleTesting.hpp"

// ==================== Actual testing ==================== //

// ========== Test on circle-circle distance functions ========== //

// ---- Brent Methods ---- //
// DISABLED SINCE METHOD IS NOT ACCURATE //
using FloatTestTypes = ::testing::Types<float,double,long double>;

/*
TYPED_TEST_CASE(BrentTestLineCircle,FloatTestTypes);

TYPED_TEST(BrentTestLineCircle, EasySpecial)
{
    this->test_special_easy();
}

TYPED_TEST(BrentTestLineCircle, HardSpecial)
{
    this->test_special_hard();
}

TYPED_TEST(BrentTestLineCircle, UnitSpeed)
{
  this->test_random(1., TEST_I_MIN,TEST_I_MAX, TEST_SEED_COUNT);
}

TYPED_TEST(BrentTestLineCircle, LowSpeed)
{
  this->test_random(TEST_LOW_SPEED, TEST_I_MIN,TEST_I_MAX, TEST_SEED_COUNT);
}

TYPED_TEST(BrentTestLineCircle, HighSpeed)
{
  this->test_random(TEST_HIGH_SPEED, TEST_I_MIN,TEST_I_MAX, TEST_SEED_COUNT);
}

TYPED_TEST(BrentTestLineCircle, RandomSpeed)
{
  this->test_random(-1., TEST_I_MIN,TEST_I_MAX, TEST_SEED_COUNT);
}

// Tests specifically for Brent with derivatives
TYPED_TEST_SUITE(BrentDerTestLineCircle,FloatTestTypes);

TYPED_TEST(BrentDerTestLineCircle, EasySpecial)
{
    this->test_special_easy();
}

TYPED_TEST(BrentDerTestLineCircle, HardSpecial)
{
    this->test_special_hard();
}
*/

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


TYPED_TEST_CASE(IntervalTestLineCircle,PairedTestTypes);

TYPED_TEST(IntervalTestLineCircle, EasySpecial)
{
  this->test_special_easy();
}

TYPED_TEST(IntervalTestLineCircle, HardSpecial)
{
  this->test_special_hard();
}

TYPED_TEST(IntervalTestLineCircle, UnitSpeed)
{
  this->test_random(1., TEST_I_MIN,TEST_I_MAX, TEST_SEED_COUNT);
}

TYPED_TEST(IntervalTestLineCircle, LowSpeed)
{
  this->test_random(TEST_LOW_SPEED, TEST_I_MIN,TEST_I_MAX, TEST_SEED_COUNT);
}

TYPED_TEST(IntervalTestLineCircle, HighSpeed)
{
  this->test_random(TEST_HIGH_SPEED, TEST_I_MIN,TEST_I_MAX, TEST_SEED_COUNT);
}

TYPED_TEST(IntervalTestLineCircle, RandomSpeed)
{
  this->test_random(-1., TEST_I_MIN,TEST_I_MAX, TEST_SEED_COUNT);
}

// Tests specifically for Interval with derivatives
TYPED_TEST_SUITE(IntervalDerTestLineCircle,PairedTestTypes);

TYPED_TEST(IntervalDerTestLineCircle, EasySpecial)
{
    this->test_special_easy();
}

TYPED_TEST(IntervalDerTestLineCircle, HardSpecial)
{
    this->test_special_hard();
}

// ---- Methods with derivatives ---- //

TYPED_TEST_CASE(WithDerivativesTestLineCircle,PairedTestTypes);


TYPED_TEST(WithDerivativesTestLineCircle, UnitSpeed)
{
  this->test_random(1., TEST_I_MIN,TEST_I_MAX, TEST_SEED_COUNT);
}

TYPED_TEST(WithDerivativesTestLineCircle, LowSpeed)
{
  this->test_random(TEST_LOW_SPEED, TEST_I_MIN,TEST_I_MAX, TEST_SEED_COUNT);
}

TYPED_TEST(WithDerivativesTestLineCircle, HighSpeed)
{
  this->test_random(TEST_HIGH_SPEED, TEST_I_MIN,TEST_I_MAX, TEST_SEED_COUNT);
}

TYPED_TEST(WithDerivativesTestLineCircle, RandomSpeed)
{
  this->test_random(-1., TEST_I_MIN,TEST_I_MAX, TEST_SEED_COUNT);
}

// ---- Methods without derivatives ---- //

TYPED_TEST_CASE(NoDerivativesTestLineCircle,PairedTestTypes);


TYPED_TEST(NoDerivativesTestLineCircle, UnitSpeed)
{
  this->test_random(1., TEST_I_MIN,TEST_I_MAX, TEST_SEED_COUNT);
}

TYPED_TEST(NoDerivativesTestLineCircle, LowSpeed)
{
  this->test_random(TEST_LOW_SPEED, TEST_I_MIN,TEST_I_MAX, TEST_SEED_COUNT);
}

TYPED_TEST(NoDerivativesTestLineCircle, HighSpeed)
{
  this->test_random(TEST_HIGH_SPEED, TEST_I_MIN,TEST_I_MAX, TEST_SEED_COUNT);
}

TYPED_TEST(NoDerivativesTestLineCircle, RandomSpeed)
{
  this->test_random(-1., TEST_I_MIN,TEST_I_MAX, TEST_SEED_COUNT);
}