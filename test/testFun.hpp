// Copyright (c) 2025 Mael FEURGARD <mael.feurgard@enac.fr>
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT
#pragma once

#include <boost/numeric/interval.hpp>
#include <math.h>
#include <functional>
#include <optional>

#include "IntervalSolver.hpp"
#include "testInterface.hpp"


// About these GCC attributes, symbolizing 'pure, mathematical functions': https://stackoverflow.com/questions/29117836/attribute-const-vs-attribute-pure-in-gnu-c
#define pure __attribute__((pure))
#define puremath __attribute__((const))

using namespace IntervalSolver;

// We need a specific rounding policy to handle cos and sin: https://stackoverflow.com/questions/15860761/c-boost-interval-and-cos
typedef boost::numeric::interval_lib::policies<boost::numeric::interval_lib::save_state<boost::numeric::interval_lib::rounded_transc_std<float> >,
                boost::numeric::interval_lib::checking_base<float>> rounding_policy;

typedef interval<float, rounding_policy> interval_f;



// -------------------------------------------------- //
// Test 1 : Offset square function

float fun1(float);
interval_f fun1_I(const interval_f&);
float fun1_d(float);
interval_f fun1_I_d(const interval_f&);
float fun1_dd(float);
interval_f fun1_I_dd(const interval_f&);

extern struct IntervalTesting::test_case<float,rounding_policy> test1;

// -------------------------------------------------- //
// Test 2 : Sinusoid

float fun2(float);
interval_f fun2_I(const interval_f&);
float fun2_d(float);
interval_f fun2_I_d(const interval_f&);
float fun2_dd(float);
interval_f fun2_I_dd(const interval_f&);

extern struct IntervalTesting::test_case<float,rounding_policy> test2;