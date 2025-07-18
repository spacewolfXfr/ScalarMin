// Copyright (c) 2025 Mael FEURGARD <mael.feurgard@enac.fr>
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <boost/numeric/interval.hpp>
#include <math.h>

#include "IntervalSolver.hpp"


// About these GCC attributes, symbolizing 'pure, mathematical functions': https://stackoverflow.com/questions/29117836/attribute-const-vs-attribute-pure-in-gnu-c
#define pure __attribute__((pure))
#define puremath __attribute__((const))

// We need a specific rounding policy to handle cos and sin: https://stackoverflow.com/questions/15860761/c-boost-interval-and-cos
typedef boost::numeric::interval_lib::policies<boost::numeric::interval_lib::save_state<boost::numeric::interval_lib::rounded_transc_std<float> >,
                boost::numeric::interval_lib::checking_base<float>> rounding_policy;

typedef interval<float, rounding_policy> interval_f;



// -------------------------------------------------- //
// Test 1 : Offset square function

const float test1_loc   = 1;
const float test1_val   = 0;
const float test1_left  = -10;
const float test1_right =  10;
const float test1_eps   = 1e-9;


float fun1(float);
interval_f fun1_I(const interval_f&);
float fun1_d(float);
interval_f fun1_I_d(const interval_f&);
float fun1_dd(float);
interval_f fun1_I_dd(const interval_f&);

bool test1();

// -------------------------------------------------- //
// Test 2 : Sinusoid

const float test2_loc = 3*M_PI_2;
const float test2_val = -1;
const float test2_left  = 0;
const float test2_right = 2*M_PI;
const float test2_eps   = 1e-9;

float fun2(float);
interval_f fun2_I(const interval_f&);
float fun2_d(float);
interval_f fun2_I_d(const interval_f&);
float fun2_dd(float);
interval_f fun2_I_dd(const interval_f&);

bool test2();