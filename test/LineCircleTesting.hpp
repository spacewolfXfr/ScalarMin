// Copyright (c) 2025 Mael FEURGARD <mael.feurgard@enac.fr>
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#pragma once

#include "testing.hpp"
#include "lineCircle.hpp"

// ==================== Configurable constants ==================== //

// Vehicle speed
#if !defined(TEST_LOW_SPEED) 
#define TEST_LOW_SPEED 0.1
#endif

#if !defined(TEST_HIGH_SPEED) 
#define TEST_HIGH_SPEED 3
#endif

// Search interval
#if !defined(TEST_I_MIN)
#define TEST_I_MIN -5
#endif

#if !defined(TEST_I_MAX)
#define TEST_I_MAX 5
#endif

// ==================== GTest classes ==================== //

template<typename _T, class _Policies=Default_policies<_T>>
class TestLineCircle : public GeneralTestByFloatType<_T,_Policies>
{
public:
  TestLineCircle() {}
protected:

    void test_special_easy()
    {
        LineCircle<_T,_Policies> test = linecircle_easy_special_case<_T,_Policies>(this->get_precision());
        this->test_and_check(test);
    }

    void test_special_hard()
    {
        LineCircle<_T,_Policies> test = linecircle_hard_special_case<_T,_Policies>(this->get_precision());
        this->test_and_check(test);
    }

    void test_random(_T speed, _T I_min, _T I_max, int sample_num)
    {
        _T precision = TestLineCircle<_T,_Policies>::get_precision();

        for(auto i = 0; i < sample_num; i++)
        {
        auto circCirc_test = generate_linecircle_test_case<_T,_Policies>(i,speed,
            I_min,I_max,precision);

        this->agreement_test(circCirc_test);
        }
    }
};

template<typename _T>
class BrentTestLineCircle : public TestLineCircle<_T,Default_policies<_T>>
{
public:
  BrentTestLineCircle() {}
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
class BrentDerTestLineCircle : public TestLineCircle<_T,Default_policies<_T>>
{
public:
  BrentDerTestLineCircle() {}
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
class IntervalTestLineCircle : public TestLineCircle<typename PairedType::first_type, typename PairedType::second_type>
{
public:
  IntervalTestLineCircle() {}
protected: 
  typedef typename PairedType::first_type _T;
  typedef typename PairedType::second_type _Policies;

  _T find_solution(MinimizeTesting::TestCaseWithDerivatives<_T,_Policies> &c) override
  {
    return c.minimize_interval_method();
  }

  _T find_another_solution(MinimizeTesting::TestCaseWithDerivatives<_T,_Policies> &c) override
  {
    return c.minimize_interval_method_with_derivatives();
  }
};

template<typename PairedType = std::pair<float,OptimizationRounding::standard_rounding<float>>>
class IntervalDerTestLineCircle : public TestLineCircle<typename PairedType::first_type, typename PairedType::second_type>
{
public:
  IntervalDerTestLineCircle() {}
protected: 
  typedef typename PairedType::first_type _T;
  typedef typename PairedType::second_type _Policies;

  _T find_solution(MinimizeTesting::TestCaseWithDerivatives<_T,_Policies> &c) override
  {
    return c.minimize_interval_method_with_derivatives();
  }

  _T find_another_solution(MinimizeTesting::TestCaseWithDerivatives<_T,_Policies> &c) override
  {
    return c.minimize_interval_method();
  }
};


template<typename PairedType = std::pair<float,OptimizationRounding::standard_rounding<float>>>
class NoDerivativesTestLineCircle : public TestLineCircle<typename PairedType::first_type, typename PairedType::second_type>
{
public:
  NoDerivativesTestLineCircle() {}
protected: 
  typedef typename PairedType::first_type _T;
  typedef typename PairedType::second_type _Policies;

  _T find_solution(MinimizeTesting::TestCaseWithDerivatives<_T,_Policies> &c) override
  {
    return c.minimize_interval_method();
  }

  _T find_another_solution(MinimizeTesting::TestCaseWithDerivatives<_T,_Policies> &c) override
  {
    // return c.minimize_brent_method();
    return c.minimize_naive();
  }
};

template<typename PairedType = std::pair<float,OptimizationRounding::standard_rounding<float>>>
class WithDerivativesTestLineCircle : public TestLineCircle<typename PairedType::first_type, typename PairedType::second_type>
{
public:
  WithDerivativesTestLineCircle() {}
protected: 
  typedef typename PairedType::first_type _T;
  typedef typename PairedType::second_type _Policies;

  _T find_solution(MinimizeTesting::TestCaseWithDerivatives<_T,_Policies> &c) override
  {
    // return c.minimize_optbrent_method();
    return c.minimize_naive();
  }

  _T find_another_solution(MinimizeTesting::TestCaseWithDerivatives<_T,_Policies> &c) override
  {
    return c.minimize_interval_method_with_derivatives();
  }
};