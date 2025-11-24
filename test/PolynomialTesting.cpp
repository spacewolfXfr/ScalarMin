// Copyright (c) 2025 Mael FEURGARD <mael.feurgard@enac.fr>
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "PolynomialTesting.hpp"


// ==================== Actual testing ==================== //

// ========== Test on polynomials ========== //

// ---- Brent Methods ---- //
// DISABLED SINCE METHOD IS NOT ACCURATE //

using FloatTestTypes = ::testing::Types<float,double,long double>;

/*
// Derivatives free
TYPED_TEST_CASE(BrentTestCircleCircle,FloatTestTypes);


TYPED_TEST(BrentTestCircleCircle,RootOne)
{
  this->test_root_one();
}

TYPED_TEST(BrentTestCircleCircle,RightEdge)
{
  this->test_right_edge();
}

TYPED_TEST(BrentTestCircleCircle,LeftEdge)
{
  this->test_left_edge();
}

TYPED_TEST(BrentTestCircleCircle, RootsInInterval)
{
  auto interval_and_roots = TestFixture::roots_in_interval();

  this->test_random(interval_and_roots,TEST_SEED_COUNT);
}

TYPED_TEST(BrentTestCircleCircle, RootsOutInterval)
{
  auto interval_and_roots = TestFixture::roots_out_interval();

  this->test_random(interval_and_roots,TEST_SEED_COUNT);
}

// Using (first) derivative
TYPED_TEST_CASE(BrentDerTestPolynomial,FloatTestTypes);

TYPED_TEST(BrentDerTestPolynomial,RootOne)
{
  this->test_root_one();
}

TYPED_TEST(BrentDerTestPolynomial,RightEdge)
{
  this->test_right_edge();
}

TYPED_TEST(BrentDerTestPolynomial,LeftEdge)
{
  this->test_left_edge();
}

TYPED_TEST(BrentDerTestPolynomial, RootsInInterval)
{
  auto interval_and_roots = TestFixture::roots_in_interval();

  this->test_random(interval_and_roots,TEST_SEED_COUNT);
}

TYPED_TEST(BrentDerTestPolynomial, RootsOutInterval)
{
  auto interval_and_roots = TestFixture::roots_out_interval();

  this->test_random(interval_and_roots,TEST_SEED_COUNT);
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

// Derivatives free
TYPED_TEST_CASE(IntervalTestCircleCircle,PairedTestTypes);

TYPED_TEST(IntervalTestCircleCircle,RootOne)
{
  this->test_root_one();
}

TYPED_TEST(IntervalTestCircleCircle,RightEdge)
{
  this->test_right_edge();
}

TYPED_TEST(IntervalTestCircleCircle,LeftEdge)
{
  this->test_left_edge();
}

TYPED_TEST(IntervalTestCircleCircle, RootsInInterval)
{
  auto interval_and_roots = TestFixture::roots_in_interval();

  this->test_random(interval_and_roots,TEST_SEED_COUNT);
}

TYPED_TEST(IntervalTestCircleCircle, RootsOutInterval)
{
  auto interval_and_roots = TestFixture::roots_out_interval();

  this->test_random(interval_and_roots,TEST_SEED_COUNT);
}

// Using first and second derivatives 
TYPED_TEST_CASE(IntervalDerTestPolynomial,PairedTestTypes);

TYPED_TEST(IntervalDerTestPolynomial,RootOne)
{
  this->test_root_one();
}

TYPED_TEST(IntervalDerTestPolynomial,RightEdge)
{
  this->test_right_edge();
}

TYPED_TEST(IntervalDerTestPolynomial,LeftEdge)
{
  this->test_left_edge();
}

TYPED_TEST(IntervalDerTestPolynomial, RootsInInterval)
{
  auto interval_and_roots = TestFixture::roots_in_interval();

  this->test_random(interval_and_roots,TEST_SEED_COUNT);
}

TYPED_TEST(IntervalDerTestPolynomial, RootsOutInterval)
{
  auto interval_and_roots = TestFixture::roots_out_interval();

  this->test_random(interval_and_roots,TEST_SEED_COUNT);
}

