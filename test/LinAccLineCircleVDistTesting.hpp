// Copyright (c) 2025 Mael FEURGARD <mael.feurgard@enac.fr>
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#pragma once

#include "testing.hpp"
#include "linAccLineCircle.hpp"

// ==================== Configurable constants ==================== //

// Vehicle speed
#if !defined(TEST_LOW_SPEED) 
#define TEST_LOW_SPEED 0.1
#endif

#if !defined(TEST_HIGH_SPEED) 
#define TEST_HIGH_SPEED 2
#endif

// Search interval
#if !defined(TEST_I_MIN)
#define TEST_I_MIN -5
#endif

#if !defined(TEST_I_MAX)
#define TEST_I_MAX 5
#endif

// Maximum acceleration values used for randomized linear-acceleration tests
#if !defined(TEST_MAX_ACCEL_CIRCLE)
#define TEST_MAX_ACCEL_CIRCLE 1
#endif

#if !defined(TEST_MAX_ACCEL_LINE)
#define TEST_MAX_ACCEL_LINE 1
#endif

// Maximum time-distance correction for VDist tests
#if !defined(TEST_MAX_TDIST)
#define TEST_MAX_TDIST 1
#endif

// ==================== GTest classes ==================== //

template<typename _T, class _Policies=Default_policies<_T>>
class TestLinAccLineCircleVDist : public GeneralTestByFloatType<_T,_Policies>
{
public:
  TestLinAccLineCircleVDist() {}
protected:

    void test_special_easy()
    {
        LinAccLineCircleVDist<_T,_Policies> test = linacclinecirclevdist_easy_special_case<_T,_Policies>(this->get_precision());
        this->test_and_check(test);
        this->test_and_check_bis(test);
    }

    void test_special_hard()
    {
        LinAccLineCircleVDist<_T,_Policies> test = linacclinecirclevdist_hard_special_case<_T,_Policies>(this->get_precision());
        this->test_and_check(test);
        this->test_and_check_bis(test);
    }

    void test_random(_T speed, _T max_accel_circle, _T max_accel_line, _T max_tdist, _T I_min, _T I_max, int sample_num)
    {
        _T precision = TestLinAccLineCircleVDist<_T,_Policies>::get_precision();

        for(auto i = 0; i < sample_num; i++)
        {
            auto test = generate_linacclinecirclevdist_test_case<_T,_Policies>(i, speed,
                max_accel_circle, max_accel_line, max_tdist, I_min, I_max, precision);

            this->agreement_test(test);
        }
    }
};

template<typename _T>
class BrentTestLinAccLineCircleVDist : public TestLinAccLineCircleVDist<_T,Default_policies<_T>>
{
public:
  BrentTestLinAccLineCircleVDist() {}
protected:
  using _Policies = Default_policies<_T>;

  _T find_solution(MinimizeTesting::TestCaseWithDerivatives<_T,Default_policies<_T>> &c) override
  {
    return c.minimize_brent_method();
  }

  _T find_another_solution(MinimizeTesting::TestCaseWithDerivatives<_T,Default_policies<_T>> &c) override
  {
    return c.minimize_optbrent_method();
  }
};

template<typename _T>
class BrentDerTestLinAccLineCircleVDist : public TestLinAccLineCircleVDist<_T,Default_policies<_T>>
{
public:
  BrentDerTestLinAccLineCircleVDist() {}
protected:
  using _Policies = Default_policies<_T>;

  _T find_solution(MinimizeTesting::TestCaseWithDerivatives<_T,Default_policies<_T>> &c) override
  {
    return c.minimize_optbrent_method();
  }

  _T find_another_solution(MinimizeTesting::TestCaseWithDerivatives<_T,Default_policies<_T>> &c) override
  {
    return c.minimize_brent_method();
  }
};

// Using paired types to test accross two kinds of types:
// https://stackoverflow.com/questions/52256953/c-multiple-parameters-with-gtest-typed-test
template<typename PairedType = std::pair<float,OptimizationRounding::standard_rounding<float>>>
class IntervalTestLinAccLineCircleVDist : public TestLinAccLineCircleVDist<typename PairedType::first_type, typename PairedType::second_type>
{
public:
  IntervalTestLinAccLineCircleVDist() {}
protected: 
  typedef typename PairedType::first_type _T;
  typedef typename PairedType::second_type _Policies;

  _T find_solution(MinimizeTesting::TestCaseWithDerivatives<_T,_Policies> &c) override
  {
    return c.minimize_interval_method();
  }

  _T find_another_solution(MinimizeTesting::TestCaseWithDerivatives<_T,_Policies> &c) override
  {
    if (c.testVals.hotstart.has_value())
    {
      return c.minimize_interval_method_with_hotstart();
    }
    else
    {
      return c.minimize_interval_method_with_derivatives();
    }
  }
};

template<typename PairedType = std::pair<float,OptimizationRounding::standard_rounding<float>>>
class IntervalDerTestLinAccLineCircleVDist : public TestLinAccLineCircleVDist<typename PairedType::first_type, typename PairedType::second_type>
{
public:
  IntervalDerTestLinAccLineCircleVDist() {}
protected: 
  typedef typename PairedType::first_type _T;
  typedef typename PairedType::second_type _Policies;

  _T find_solution(MinimizeTesting::TestCaseWithDerivatives<_T,_Policies> &c) override
  {
    return c.minimize_interval_method_with_derivatives();
  }

  _T find_another_solution(MinimizeTesting::TestCaseWithDerivatives<_T,_Policies> &c) override
  {
    if (c.testVals.hotstart.has_value())
    {
      return c.minimize_interval_method_with_derivatives_and_hotstart();
    }
    else
    {
      return c.minimize_interval_method();
    }
  }
};

template<typename PairedType = std::pair<float,OptimizationRounding::standard_rounding<float>>>
class NoDerivativesTestLinAccLineCircleVDist : public TestLinAccLineCircleVDist<typename PairedType::first_type, typename PairedType::second_type>
{
public:
  NoDerivativesTestLinAccLineCircleVDist() {}
protected: 
  typedef typename PairedType::first_type _T;
  typedef typename PairedType::second_type _Policies;

  _T find_solution(MinimizeTesting::TestCaseWithDerivatives<_T,_Policies> &c) override
  {
    return c.minimize_interval_method();
  }

  _T find_another_solution(MinimizeTesting::TestCaseWithDerivatives<_T,_Policies> &c) override
  {
    return c.minimize_naive();
  }
};

template<typename PairedType = std::pair<float,OptimizationRounding::standard_rounding<float>>>
class WithDerivativesTestLinAccLineCircleVDist : public TestLinAccLineCircleVDist<typename PairedType::first_type, typename PairedType::second_type>
{
public:
  WithDerivativesTestLinAccLineCircleVDist() {}
protected: 
  typedef typename PairedType::first_type _T;
  typedef typename PairedType::second_type _Policies;

  _T find_solution(MinimizeTesting::TestCaseWithDerivatives<_T,_Policies> &c) override
  {
    return c.minimize_naive();
  }

  _T find_another_solution(MinimizeTesting::TestCaseWithDerivatives<_T,_Policies> &c) override
  {
    return c.minimize_interval_method_with_derivatives();
  }
};
