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

bool test1()
{
    float r = minimize_interval_method<float,rounding_policy>(test1_left,test1_right,
    fun1,fun1_I,fun1_d,fun1_I_d,fun1_dd,fun1_I_dd,test1_eps);

    return abs(r - test1_val) < test1_eps;
}

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


bool test2()
{
    float r = minimize_interval_method<float,rounding_policy>(test2_left,test2_right,
    fun2,fun2_I,fun2_d,fun2_I_d,fun2_dd,fun2_I_dd,test2_eps);

    return abs(r - test2_val) < test2_eps;
}