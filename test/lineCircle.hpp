// Copyright (c) 2025 Mael FEURGARD <mael.feurgard@enac.fr>
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT


#pragma once

#include "testInterface.hpp"

#include <cmath>
#include <random>

template<typename T, class Policies>
class LineCircle : public MinimizeTesting::TestCaseWithDerivatives<T,Policies>
{

    private:
        const T Dx,Dy;
        const T r,w,phi;
        const T vx,vy;

    public:
        void print_params()
        {
            std::cout   << std::setprecision(5)
                        << "Dx   : " << Dx      << std::endl
                        << "Dy   : " << Dy      << std::endl
                        << "r    : " << r       << std::endl
                        << "w    : " << w       << std::endl
                        << "phi  : " << phi     << std::endl
                        << "vx   : " << vx      << std::endl
                        << "vy   : " << vy      << std::endl << std::endl; 
            
        }

        LineCircle(MinimizeTesting::TestValues<T> vals, T _Dx, T _Dy, T _r, T _w, T _phi, T _vx, T _vy)
        : MinimizeTesting::TestCaseWithDerivatives<T,Policies>(vals),
             Dx(_Dx), Dy(_Dy), r(_r), w(_w), phi(_phi), vx(_vx), vy(_vy) {}

        pure T f(T t) const
        {
            T x = Dx + r*std::cos(w*t+phi) - t*vx;
            T y = Dy + r*std::sin(w*t+phi) - t*vy;
            return x*x + y*y;
        } 

        pure MinimizeTesting::interval<T,Policies> F(const MinimizeTesting::interval<T,Policies> &I) const
        {
            using namespace boost::numeric;

            interval<T,Policies> x_I = Dx + r*cos(w*I+phi) - I*vx;
            interval<T,Policies> y_I = Dy + r*sin(w*I+phi) - I*vy;
            return square(x_I) + square(y_I);
        }

        pure T f_d(T t) const
        {
            T c1 = std::cos(w*t+phi);
            T s1 = std::sin(w*t+phi);

            T x = Dx + r*c1 - t*vx;
            T y = Dy + r*s1 - t*vy;

            T x_d = -w*r*s1 - vx;
            T y_d =  w*r*c1 - vy;

            return (x_d*x + y_d*y)*2;
        } 

        pure MinimizeTesting::interval<T,Policies> F_d(const MinimizeTesting::interval<T,Policies> &I) const
        {
            using namespace boost::numeric;

            interval<T,Policies> c1 = cos(w*I+phi);
            interval<T,Policies> s1 = sin(w*I+phi);

            interval<T,Policies> x_I = Dx + r*c1 - t*vx;
            interval<T,Policies> y_I = Dy + r*s1 - t*vy;

            interval<T,Policies> x_d_I = -w*r*s1 - vx;
            interval<T,Policies> y_d_I =  w*r*c1 - vy;
            return (x_d_I*x_I + y_d_I*y_I)*static_cast<T>(2);
        }

        pure T f_dd(T t) const
        {
            T c1 = std::cos(w*t+phi);
            T s1 = std::sin(w*t+phi);

            T x = Dx + r*c1 - t*vx;
            T y = Dy + r*s1 - t*vy;

            T x_d = -w*r*s1 - vx;
            T y_d =  w*r*c1 - vy;

            T x_dd = -w*w*r*c1;
            T y_dd = -w*w*r*s1;

            return (x_d*x_d+x*x_dd + y_d*y_d+y*y_dd)*2;
        } 

        pure MinimizeTesting::interval<T,Policies> F_dd(const MinimizeTesting::interval<T,Policies> &I) const
        {
            using namespace boost::numeric;

            interval<T,Policies> c1 = cos(w*I+phi);
            interval<T,Policies> s1 = sin(w*I+phi);

            interval<T,Policies> x_I = Dx + r*c1 - t*vx;
            interval<T,Policies> y_I = Dy + r*s1 - t*vy;

            interval<T,Policies> x_d_I = -w*r*s1 - vx;
            interval<T,Policies> y_d_I =  w*r*c1 - vy;

            interval<T,Policies> x_dd_I = -w*w*r*c1;
            interval<T,Policies> y_dd_I = -w*w*r*s1;
            
            return (square(x_d_I)+x_I*x_dd_I + square(y_d_I)+y_I*y_dd_I)*static_cast<T>(2);
        }

};

// template<typename T, class Policies>
// LineCircle<T,Policies> linecircle_easy_special_case(T precision)
// {
//     MinimizeTesting::TestValues<T> vals;
//     vals.init_left  = 0;
//     vals.init_right = 5;
//     vals.precision  = precision;
//     vals.location   = 4.12087;
//     vals.value      = 0.000722251;


//     return LineCircle<T,Policies>(vals,
//         1,1,1,0.5,M_PI,1.,1.);

// }

// template<typename T, class Policies>
// LineCircle<T,Policies> linecircle_hard_special_case(T precision)
// {
//     MinimizeTesting::TestValues<T> vals;
//     vals.init_left  = -5;
//     vals.init_right = 5;
//     vals.precision  = precision;
//     vals.location   = 4.12087;
//     vals.value      = 0.000722251;


//     return LineCircle<T,Policies>(vals,
//         1,1,1,0.5,1,M_PI,0);
// }

/**
 * @brief Generate a random test case for the Line-circle distance function
 * 
 * @tparam T Float subtype (float,double, long double)
 * @tparam Policies Rounding policies (relevant for Interval methods)
 * @param seed  RNG seed
 * @param speed Speed for the two vehicles moving on the circles. If non-positive, is picked randomly for each vehicle 
 *  (they may be different!)
 * @param I_min Start of the search interval
 * @param I_max End of the search interval
 * @param precision Required precision result 
 * @return LineCircle<T,Policies> A Line-circle test case
 */
template<typename T, class Policies>
LineCircle<T,Policies> generate_linecircle_test_case(int seed, T speed, T I_min, T I_max, T precision)
{
    std::default_random_engine gen(seed); // Some seeded RNG 
    
    T v1,v2;

    if (speed > 0)
    {
        v1 = speed;
        v2 = speed;
    }
    else
    {
        std::uniform_real_distribution<T> dis_speed(0.1, 10);
        v1 = dis_speed(gen);
        v2 = dis_speed(gen);
    }

    T phi,alpha; 
    std::uniform_real_distribution<T> dis_phi(-M_PI, M_PI);
    phi     = dis_phi(gen);
    alpha   = dis_phi(alpha);

    T r,w;
    std::uniform_real_distribution<T> dis_radius(0.1, 10);

    r = dis_radius(gen);
    w = v1/r1;

    T Dx,Dy;
    Dx = dis_radius(gen);
    Dy = dis_radius(gen);

    MinimizeTesting::TestValues<T> vals;
    vals.init_left  = I_min;
    vals.init_right = I_max;
    vals.precision  = precision;

    vals.location   = NAN;
    vals.value      = NAN;

    return LineCircle<T,Policies>(vals,Dx,Dy,r,w,phi,v2*std::cos(alpha),v2*std::sin(alpha));
}