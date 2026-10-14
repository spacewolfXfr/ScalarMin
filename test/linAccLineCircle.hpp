// Copyright (c) 2025 Mael FEURGARD <mael.feurgard@enac.fr>
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#pragma once

#include "testInterface.hpp"

#include <cmath>
#include <random>

template<typename T, class Policies>
class LinAccLineCircle : public MinimizeTesting::TestCaseWithDerivatives<T,Policies>
{

    private:
        const T Dx,Dy;
        const T r,phi;
        const T aC,vC;
        const T aL,vL;
        const T nx,ny;

    public:
        void print_params()
        {
            std::cout   << std::setprecision(5)
                        << "Dx   : " << Dx      << std::endl
                        << "Dy   : " << Dy      << std::endl
                        << "r    : " << r       << std::endl
                        << "phi  : " << phi     << std::endl
                        << "aC   : " << aC      << std::endl
                        << "vC   : " << vC      << std::endl
                        << "aL   : " << aL      << std::endl
                        << "vL   : " << vL      << std::endl
                        << "nx   : " << nx      << std::endl
                        << "ny   : " << ny      << std::endl << std::endl;
        }

        LinAccLineCircle(MinimizeTesting::TestValues<T> vals, T _Dx, T _Dy, T _r, T _phi, T _aC, T _vC, T _aL, T _vL, T _nx, T _ny)
        : MinimizeTesting::TestCaseWithDerivatives<T,Policies>(vals),
             Dx(_Dx), Dy(_Dy), r(_r), phi(_phi), aC(_aC), vC(_vC), aL(_aL), vL(_vL), nx(_nx), ny(_ny) {}

        pure T f(T t) const
        {
            T theta = (aC*t+vC)*t + phi;
            T line_speed = aL*t+vL;
            T x = Dx + r*std::cos(theta) - t*nx*line_speed;
            T y = Dy + r*std::sin(theta) - t*ny*line_speed;
            return x*x + y*y;
        }

        pure MinimizeTesting::interval<T,Policies> F(const MinimizeTesting::interval<T,Policies> &I) const
        {
            using namespace boost::numeric;

            interval<T,Policies> theta_I = aC*square(I)+vC*I + phi;
            interval<T,Policies> x_I = Dx + r*cos(theta_I) - nx*(aL*square(I)+vL*I);
            interval<T,Policies> y_I = Dy + r*sin(theta_I) - ny*(aL*square(I)+vL*I);
            return square(x_I) + square(y_I);
        }

        pure T f_d(T t) const
        {
            T theta = (aC*t+vC)*t + phi;
            T c = std::cos(theta);
            T s = std::sin(theta);

            T omega = 2*aC*t + vC;
            T speed = aL*t + vL;
            T line_d = 2*aL*t + vL;

            T x = Dx + r*c - t*nx*speed;
            T y = Dy + r*s - t*ny*speed;

            T x_d = -omega*r*s - nx*line_d;
            T y_d =  omega*r*c - ny*line_d;

            return (x_d*x + y_d*y)*2;
        }

        pure MinimizeTesting::interval<T,Policies> F_d(const MinimizeTesting::interval<T,Policies> &I) const
        {
            using namespace boost::numeric;

            interval<T,Policies> theta_I = aC*square(I)+vC*I + phi;

            interval<T,Policies> c = cos(theta_I);
            interval<T,Policies> s = sin(theta_I);

            interval<T,Policies> omega_I = 2*aC*I + vC;
            interval<T,Policies> line_d_I = 2*aL*I + vL;

            interval<T,Policies> x_I = Dx + r*c - nx*(aL*square(I)+vL*I);
            interval<T,Policies> y_I = Dy + r*s - ny*(aL*square(I)+vL*I);

            interval<T,Policies> x_d_I = -omega_I*r*s - nx*line_d_I;
            interval<T,Policies> y_d_I =  omega_I*r*c - ny*line_d_I;
            return (x_d_I*x_I + y_d_I*y_I)*static_cast<T>(2);
        }

        pure T f_dd(T t) const
        {
            T theta = (aC*t+vC)*t + phi;
            T c = std::cos(theta);
            T s = std::sin(theta);

            T omega = 2*aC*t + vC;
            T omega_d = 2*aC;
            T line_d = 2*aL*t + vL;

            T x = Dx + r*c - t*nx*(aL*t + vL);
            T y = Dy + r*s - t*ny*(aL*t + vL);

            T x_d = -omega*r*s - nx*line_d;
            T y_d =  omega*r*c - ny*line_d;

            T x_dd = -r*(omega*omega*c + omega_d*s) - 2*aL*nx;
            T y_dd =  r*(-omega*omega*s + omega_d*c) - 2*aL*ny;

            return (x_d*x_d+x*x_dd + y_d*y_d+y*y_dd)*2;
        }

        pure MinimizeTesting::interval<T,Policies> F_dd(const MinimizeTesting::interval<T,Policies> &I) const
        {
            using namespace boost::numeric;

            interval<T,Policies> theta_I = aC*square(I)+vC*I + phi;

            interval<T,Policies> c = cos(theta_I);
            interval<T,Policies> s = sin(theta_I);

            interval<T,Policies> omega_I = 2*aC*I + vC;
            interval<T,Policies> line_d_I = 2*aL*I + vL;

            interval<T,Policies> x_I = Dx + r*c - nx*(aL*square(I)+vL*I);
            interval<T,Policies> y_I = Dy + r*s - ny*(aL*square(I)+vL*I);

            interval<T,Policies> x_d_I = -omega_I*r*s - nx*line_d_I;
            interval<T,Policies> y_d_I =  omega_I*r*c - ny*line_d_I;

            interval<T,Policies> x_dd_I = -r*(square(omega_I)*c + 2*aC*s) - 2*aL*nx;
            interval<T,Policies> y_dd_I =  r*(-square(omega_I)*s + 2*aC*c) - 2*aL*ny;

            return (square(x_d_I)+x_I*x_dd_I + square(y_d_I)+y_I*y_dd_I)*static_cast<T>(2);
        }

};

template<typename T, class Policies>
LinAccLineCircle<T,Policies> linacclinecircle_easy_special_case(T precision)
{
    MinimizeTesting::TestValues<T> vals;
    vals.init_left  = 0;
    vals.init_right = 5;
    vals.precision  = precision;
    vals.location   = 1.39796;
    vals.value      = 0.237015;
    vals.hotstart   = 1.;

    return LinAccLineCircle<T,Policies>(vals,
        1,1,1,0.,0,M_PI,0,0.5,0.8,-0.6);
}

template<typename T, class Policies>
LinAccLineCircle<T,Policies> linacclinecircle_hard_special_case(T precision)
{
    MinimizeTesting::TestValues<T> vals;
    vals.init_left  = -5;
    vals.init_right = 5;
    vals.precision  = precision;
    vals.location   = 1.39796;
    vals.value      = 0.237015;
    vals.hotstart   = -2.;

    return LinAccLineCircle<T,Policies>(vals,
        1,1,1,0.,0,M_PI,0,0.5,0.8,-0.6);
}

/**
 * @brief Generate a random test case for the Line-circle distance function with linear acceleration
 * 
 * @tparam T Float subtype (float,T, long T)
 * @tparam Policies Rounding policies (relevant for Interval methods)
 * @param seed RNG seed
 * @param speed Initial speed for circular and line motions. If non-positive, separate random values are sampled
 * @param max_accel_circle Maximum absolute acceleration for the circular angular speed term
 * @param max_accel_line Maximum absolute acceleration for the line speed term
 * @param I_min Start of the search interval
 * @param I_max End of the search interval
 * @param precision Required precision result
 * @return LinAccLineCircle<T,Policies> A Line-circle test case with linearly varying speeds
 */
template<typename T, class Policies>
LinAccLineCircle<T,Policies> generate_linacclinecircle_test_case(
    int seed, T speed, T max_accel_circle, T max_accel_line, T I_min, T I_max, T precision)
{
    std::default_random_engine gen(seed); // Some seeded RNG

    T v_circle, v_line;

    if (speed > 0)
    {
        v_circle = speed;
        v_line = speed;
    }
    else
    {
        std::uniform_real_distribution<T> dis_speed(0.1, 10);
        v_circle = dis_speed(gen);
        v_line = dis_speed(gen);
    }

    T a_circle = 0;
    T a_line = 0;

    if (max_accel_circle > 0)
    {
        std::uniform_real_distribution<T> dis_accel_circle(-max_accel_circle, max_accel_circle);
        a_circle = dis_accel_circle(gen);
    }

    if (max_accel_line > 0)
    {
        std::uniform_real_distribution<T> dis_accel_line(-max_accel_line, max_accel_line);
        a_line = dis_accel_line(gen);
    }

    T phi,alpha;
    std::uniform_real_distribution<T> dis_phi(-M_PI, M_PI);
    phi     = dis_phi(gen);
    alpha   = dis_phi(gen);

    T r;
    std::uniform_real_distribution<T> dis_radius(0.1, 10);
    r = dis_radius(gen);

    T Dx,Dy;
    Dx = dis_radius(gen);
    Dy = dis_radius(gen);

    MinimizeTesting::TestValues<T> vals;
    vals.init_left  = I_min;
    vals.init_right = I_max;
    vals.precision  = precision;

    vals.location   = NAN;
    vals.value      = NAN;

    return LinAccLineCircle<T,Policies>(vals,
        Dx,Dy,r,phi,
        a_circle,v_circle/r,
        a_line,v_line,
        std::cos(alpha),std::sin(alpha));
}




/***** Variant with corrected acceleration formula and time-speed based distance computation *****/

template<typename T, class Policies>
class LinAccLineCircleVDist : public MinimizeTesting::TestCaseWithDerivatives<T,Policies>
{
    private:
        const T Dx,Dy;
        const T r,phi;
        const T aC,vC;
        const T aL,vL;
        const T nx,ny;
        const T tdist;

    public:
        void print_params()
        {
            std::cout   << std::setprecision(5)
                        << "Dx   : " << Dx      << std::endl
                        << "Dy   : " << Dy      << std::endl
                        << "r    : " << r       << std::endl
                        << "phi  : " << phi     << std::endl
                        << "aC   : " << aC      << std::endl
                        << "vC   : " << vC      << std::endl
                        << "aL   : " << aL      << std::endl
                        << "vL   : " << vL      << std::endl
                        << "nx   : " << nx      << std::endl
                        << "ny   : " << ny      << std::endl
                        << "tdist: " << tdist   << std::endl << std::endl;
        }

        LinAccLineCircleVDist(MinimizeTesting::TestValues<T> vals, T _Dx, T _Dy, T _r, T _phi, T _aC, T _vC, T _aL, T _vL, T _nx, T _ny, T _tdist)
        : MinimizeTesting::TestCaseWithDerivatives<T,Policies>(vals),
            Dx(_Dx), Dy(_Dy), r(_r), phi(_phi), aC(_aC), vC(_vC), aL(_aL), vL(_vL), nx(_nx), ny(_ny), tdist(_tdist) {}

        pure T f(T t) const
        {
            T V1 = (aC*t+vC)*r;
            T V2 = aL*t+vL;
            T Vmax = std::max(V1,V2);

            T theta = (0.5*aC*t+vC)*t + phi;
            T line_speed = 0.5*aL*t+vL;
            T x = Dx + r*std::cos(theta) - t*nx*line_speed;
            T y = Dy + r*std::sin(theta) - t*ny*line_speed;
            return x*x + y*y - tdist*tdist*Vmax*Vmax;
        }

        pure IntervalSolver::interval<T,Policies> F(const IntervalSolver::interval<T,Policies> &I) const
        {
            using namespace boost::numeric;

            IntervalSolver::interval<T,Policies> V1 = (aC*I+vC)*r;
            IntervalSolver::interval<T,Policies> V2 = aL*I+vL;
            IntervalSolver::interval<T,Policies> Vmax = max(V1,V2);

            interval<T,Policies> theta_I = 0.5*aC*square(I)+vC*I + phi;
            interval<T,Policies> x_I = Dx + r*cos(theta_I) - nx*(0.5*aL*square(I)+vL*I);
            interval<T,Policies> y_I = Dy + r*sin(theta_I) - ny*(0.5*aL*square(I)+vL*I);
            return square(x_I) + square(y_I) - tdist*tdist * square(Vmax);
        }

        pure T f_d(T t) const
        {
            T theta = (0.5*aC*t+vC)*t + phi;
            T c = std::cos(theta);
            T s = std::sin(theta);

            T V1 = (aC*t+vC)*r;
            T V2 = aL*t+vL;
            T Vmax,dVmax;
            if (V1 > V2)
            {
                Vmax = V1;
                dVmax = aC*r;
            }
            else
            {
                Vmax = V2;
                dVmax = aL;
            }

            T omega = aC*t + vC;
            T speed = 0.5*aL*t + vL;
            T line_d = aL*t + vL;

            T x = Dx + r*c - t*nx*speed;
            T y = Dy + r*s - t*ny*speed;

            T x_d = -omega*r*s - nx*line_d;
            T y_d =  omega*r*c - ny*line_d;

            return (x_d*x + y_d*y - tdist*tdist*dVmax*Vmax)*2;
        }

        pure IntervalSolver::interval<T,Policies> F_d(const IntervalSolver::interval<T,Policies> &I) const
        {
            using namespace boost::numeric;

            interval<T,Policies> theta_I = 0.5*aC*square(I)+vC*I + phi;

            interval<T,Policies> V1 = (aC*I+vC)*r;
            interval<T,Policies> V2 = aL*I+vL;
            interval<T,Policies> Vmax = max(V1,V2);
            T dV1 = aC*r;
            T dV2 = aL;
            interval<T,Policies> dVmax(std::min(dV1,dV2), std::max(dV1,dV2));

            interval<T,Policies> c = cos(theta_I);
            interval<T,Policies> s = sin(theta_I);

            interval<T,Policies> omega_I = aC*I + vC;
            interval<T,Policies> line_d_I = aL*I + vL;

            interval<T,Policies> x_I = Dx + r*c - nx*(0.5*aL*square(I)+vL*I);
            interval<T,Policies> y_I = Dy + r*s - ny*(0.5*aL*square(I)+vL*I);

            interval<T,Policies> x_d_I = -omega_I*r*s - nx*line_d_I;
            interval<T,Policies> y_d_I =  omega_I*r*c - ny*line_d_I;
            return (x_d_I*x_I + y_d_I*y_I - tdist*tdist*dVmax*Vmax)*static_cast<T>(2);
        }

        pure T f_dd(T t) const
        {
            T theta = (0.5*aC*t+vC)*t + phi;
            T c = std::cos(theta);
            T s = std::sin(theta);

            T V1 = (aC*t+vC)*r;
            T V2 = aL*t+vL;
            T Vmax,dVmax;
            if (V1 > V2)
            {
                Vmax = V1;
                dVmax = aC*r;
            }
            else
            {
                Vmax = V2;
                dVmax = aL;
            }


            T omega = aC*t + vC;
            T omega_d = aC;
            T line_d = aL*t + vL;

            T x = Dx + r*c - t*nx*(0.5*aL*t + vL);
            T y = Dy + r*s - t*ny*(0.5*aL*t + vL);

            T x_d = -omega*r*s - nx*line_d;
            T y_d =  omega*r*c - ny*line_d;

            T x_dd = -r*(omega*omega*c + omega_d*s) - aL*nx;
            T y_dd =  r*(-omega*omega*s + omega_d*c) - aL*ny;

            return (x_d*x_d+x*x_dd + y_d*y_d+y*y_dd - tdist*tdist*dVmax*dVmax)*2;
        }

        pure IntervalSolver::interval<T,Policies> F_dd(const IntervalSolver::interval<T,Policies> &I) const
        {
            using namespace boost::numeric;

            interval<T,Policies> theta_I = 0.5*aC*square(I)+vC*I + phi;

            interval<T,Policies> V1 = (aC*I+vC)*r;
            interval<T,Policies> V2 = aL*I+vL;
            interval<T,Policies> Vmax = max(V1,V2);
            T dV1 = aC*r;
            T dV2 = aL;
            interval<T,Policies> dVmax(std::min(dV1,dV2), std::max(dV1,dV2));


            interval<T,Policies> c = cos(theta_I);
            interval<T,Policies> s = sin(theta_I);

            interval<T,Policies> omega_I = aC*I + vC;
            interval<T,Policies> line_d_I = aL*I + vL;

            interval<T,Policies> x_I = Dx + r*c - nx*(0.5*aL*square(I)+vL*I);
            interval<T,Policies> y_I = Dy + r*s - ny*(0.5*aL*square(I)+vL*I);

            interval<T,Policies> x_d_I = -omega_I*r*s - nx*line_d_I;
            interval<T,Policies> y_d_I =  omega_I*r*c - ny*line_d_I;

            interval<T,Policies> x_dd_I = -r*(square(omega_I)*c + aC*s) - aL*nx;
            interval<T,Policies> y_dd_I =  r*(-square(omega_I)*s + aC*c) - aL*ny;

            return (square(x_d_I)+x_I*x_dd_I + square(y_d_I)+y_I*y_dd_I - tdist*tdist*dVmax*dVmax)*static_cast<T>(2);
        }

};