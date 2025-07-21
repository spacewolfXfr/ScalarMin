// Copyright (c) 2025 Mael FEURGARD <mael.feurgard@enac.fr>
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "testFun.hpp"

// -------------------------------------------------- //
// Test 1 : Offset square function

puremath float fun1(float x)
{
    return (x-1)*(x-1);
}

puremath interval_f fun1_I(const interval_f &X)
{
    return boost::numeric::square(X-1.f);
}

puremath float fun1_d(float x)
{
    return 2*(x-1.f);
}

puremath interval_f fun1_I_d(const interval_f &X)
{
    return (X-1.f)*2.f;
}

puremath float fun1_dd(BOOST_ATTRIBUTE_UNUSED float x)
{
    return 2.f;
}

puremath interval_f fun1_I_dd(BOOST_ATTRIBUTE_UNUSED const interval_f &X)
{
    return interval_f(2.f,2.f);
}

struct IntervalTesting::test_case<float,rounding_policy> test1
{
    .location   = 1.f,
    .value      = 0.f,
    .init_left  = -10.f,
    .init_right =  10.f,
    .precision  = 1e-6f,

    .f  = fun1,
    .F  = fun1_I,
    .f_d    = fun1_d,
    .F_d    = fun1_I_d,
    .f_dd   = fun1_dd,
    .F_dd   = fun1_I_dd,
};

// -------------------------------------------------- //
// Test 2 : Sinusoid

puremath float fun2(float x)
{
    return std::sin(x);
}

puremath interval_f fun2_I(const interval_f &X)
{
    return boost::numeric::sin(X);
}
puremath float fun2_d(float x)
{
    return std::cos(x);
}

puremath interval_f fun2_I_d(const interval_f &X)
{
    return boost::numeric::cos(X);
}

puremath float fun2_dd(float x)
{
    return -std::sin(x);
}

puremath interval_f fun2_I_dd(const interval_f &X)
{
    return -boost::numeric::sin(X);
}

struct IntervalTesting::test_case<float,rounding_policy> test2
{
    .location   = 3*M_PI_2f,
    .value      = -1.f,
    .init_left  = 0.f,
    .init_right = 3*M_PIf,
    .precision  = 1e-6f,

    .f  = fun2,
    .F  = fun2_I,
    .f_d    = fun2_d,
    .F_d    = fun2_I_d,
    .f_dd   = fun2_dd,
    .F_dd   = fun2_I_dd,
};