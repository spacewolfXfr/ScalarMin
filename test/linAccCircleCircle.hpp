// Copyright (c) 2025 Mael FEURGARD <mael.feurgard@enac.fr>
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#pragma once

#include "testInterface.hpp"

#include <cmath>
#include <random>

template<typename T, class Policies>
class LinAccCircleCircle : public MinimizeTesting::TestCaseWithDerivatives<T,Policies>
{

    private:
        const T Dx,Dy;
        const T r1,r2;
        const T a1,v1;
        const T a2,v2;
        const T Dphi;

    public:
        void print_params()
        {
            std::cout   << std::setprecision(5)
                        << "Dx   : " << Dx      << std::endl
                        << "Dy   : " << Dy      << std::endl
                        << "r1   : " << r1      << std::endl
                        << "a1   : " << a1      << std::endl
                        << "v1   : " << v1      << std::endl
                        << "r2   : " << r2      << std::endl
                        << "a2   : " << a2      << std::endl
                        << "v2   : " << v2      << std::endl
                        << "Dphi : " << Dphi    << std::endl << std::endl;

        }

        LinAccCircleCircle(MinimizeTesting::TestValues<T> vals, T _Dx, T _Dy, T _r1, T _r2, T _a1, T _v1, T _a2, T _v2, T _Dphi)
        : MinimizeTesting::TestCaseWithDerivatives<T,Policies>(vals),
             Dx(_Dx), Dy(_Dy), r1(_r1), r2(_r2), a1(_a1), v1(_v1), a2(_a2), v2(_v2), Dphi(_Dphi) {}

        pure T f(T t) const
        {
            T theta1 = (a1*t+v1)*t;
            T theta2 = (a2*t+v2)*t + Dphi;
            T x = Dx + r1*std::cos(theta1) - r2*std::cos(theta2);
            T y = Dy + r1*std::sin(theta1) - r2*std::sin(theta2);
            return x*x + y*y;
        }

        pure MinimizeTesting::interval<T,Policies> F(const MinimizeTesting::interval<T,Policies> &I) const
        {
            using namespace boost::numeric;

            interval<T,Policies> theta1_I = (a1*square(I)+v1*I);
            interval<T,Policies> theta2_I = (a2*square(I)+v2*I) + Dphi;
            interval<T,Policies> x_I = Dx + r1*cos(theta1_I) - r2*cos(theta2_I);
            interval<T,Policies> y_I = Dy + r1*sin(theta1_I) - r2*sin(theta2_I);
            return square(x_I) + square(y_I);
        }

        pure T f_d(T t) const
        {
            T theta1 = (a1*t+v1)*t;
            T theta2 = (a2*t+v2)*t + Dphi;

            T c1 = std::cos(theta1);
            T s1 = std::sin(theta1);

            T c2 = std::cos(theta2);
            T s2 = std::sin(theta2);

            T omega1 = 2*a1*t + v1;
            T omega2 = 2*a2*t + v2;

            T x = Dx + r1*c1 - r2*c2;
            T y = Dy + r1*s1 - r2*s2;

            T x_d = -omega1*r1*s1 + omega2*r2*s2;
            T y_d =  omega1*r1*c1 - omega2*r2*c2;

            return (x_d*x + y_d*y)*2;
        }

        pure MinimizeTesting::interval<T,Policies> F_d(const MinimizeTesting::interval<T,Policies> &I) const
        {
            using namespace boost::numeric;

            interval<T,Policies> theta1_I = (a1*square(I)+v1*I);
            interval<T,Policies> theta2_I = (a2*square(I)+v2*I) + Dphi;

            interval<T,Policies> c1 = cos(theta1_I);
            interval<T,Policies> s1 = sin(theta1_I);

            interval<T,Policies> c2 = cos(theta2_I);
            interval<T,Policies> s2 = sin(theta2_I);

            interval<T,Policies> omega1_I = 2*a1*I + v1;
            interval<T,Policies> omega2_I = 2*a2*I + v2;

            interval<T,Policies> x_I = Dx + r1*c1 - r2*c2;
            interval<T,Policies> y_I = Dy + r1*s1 - r2*s2;

            interval<T,Policies> x_d_I = -omega1_I*r1*s1 + omega2_I*r2*s2;
            interval<T,Policies> y_d_I =  omega1_I*r1*c1 - omega2_I*r2*c2;
            return (x_d_I*x_I + y_d_I*y_I)*static_cast<T>(2);
        }

        pure T f_dd(T t) const
        {
            T theta1 = (a1*t+v1)*t;
            T theta2 = (a2*t+v2)*t + Dphi;

            T c1 = std::cos(theta1);
            T s1 = std::sin(theta1);

            T c2 = std::cos(theta2);
            T s2 = std::sin(theta2);

            T omega1 = 2*a1*t + v1;
            T omega2 = 2*a2*t + v2;

            T omega1_d = 2*a1;
            T omega2_d = 2*a2;

            T x = Dx + r1*c1 - r2*c2;
            T y = Dy + r1*s1 - r2*s2;

            T x_d = -omega1*r1*s1 + omega2*r2*s2;
            T y_d =  omega1*r1*c1 - omega2*r2*c2;

            T x_dd = -r1*(omega1*omega1*c1 + omega1_d*s1) + r2*(omega2*omega2*c2 + omega2_d*s2);
            T y_dd =  r1*(-omega1*omega1*s1 + omega1_d*c1) - r2*(-omega2*omega2*s2 + omega2_d*c2);

            return (x_d*x_d+x*x_dd + y_d*y_d+y*y_dd)*2;
        }

        pure MinimizeTesting::interval<T,Policies> F_dd(const MinimizeTesting::interval<T,Policies> &I) const
        {
            using namespace boost::numeric;

            interval<T,Policies> theta1_I = (a1*square(I)+v1*I);
            interval<T,Policies> theta2_I = (a2*square(I)+v2*I) + Dphi;

            interval<T,Policies> c1 = cos(theta1_I);
            interval<T,Policies> s1 = sin(theta1_I);

            interval<T,Policies> c2 = cos(theta2_I);
            interval<T,Policies> s2 = sin(theta2_I);

            interval<T,Policies> omega1_I = 2*a1*I + v1;
            interval<T,Policies> omega2_I = 2*a2*I + v2;

            interval<T,Policies> x_I = Dx + r1*c1 - r2*c2;
            interval<T,Policies> y_I = Dy + r1*s1 - r2*s2;

            interval<T,Policies> x_d_I = -omega1_I*r1*s1 + omega2_I*r2*s2;
            interval<T,Policies> y_d_I =  omega1_I*r1*c1 - omega2_I*r2*c2;

            interval<T,Policies> x_dd_I = -r1*(square(omega1_I)*c1 + 2*a1*s1) + r2*(square(omega2_I)*c2 + 2*a2*s2);
            interval<T,Policies> y_dd_I =  r1*(-square(omega1_I)*s1 + 2*a1*c1) - r2*(-square(omega2_I)*s2 + 2*a2*c2);

            return (square(x_d_I)+x_I*x_dd_I + square(y_d_I)+y_I*y_dd_I)*static_cast<T>(2);
        }

};

template<typename T, class Policies>
LinAccCircleCircle<T,Policies> linacccirclecircle_easy_special_case(T precision)
{
    MinimizeTesting::TestValues<T> vals;
    vals.init_left  = 0;
    vals.init_right = 3;
    vals.precision  = precision;
    vals.location   = 2.081770418654643169728874842;
    vals.value      = 0.027119781606604591528668;
    vals.hotstart   = 1.8;

    return LinAccCircleCircle<T,Policies>(vals,
        1,1,1,2,1,2,2,1,M_PI);
}

template<typename T, class Policies>
LinAccCircleCircle<T,Policies> linacccirclecircle_hard_special_case(T precision)
{
    MinimizeTesting::TestValues<T> vals;
    vals.init_left  = 0;
    vals.init_right = 3;
    vals.precision  = precision;
    vals.location   = 2.081770418654643169728874842;
    vals.value      = 0.027119781606604591528668;
    vals.hotstart   = -1;

    return LinAccCircleCircle<T,Policies>(vals,
        1,1,1,2,1,2,2,1,M_PI);
}

/**
 * @brief Generate a random test case for the Circle-circle distance function with linear acceleration
 * 
 * @tparam T Float subtype (float,T, long T)
 * @tparam Policies Rounding policies (relevant for Interval methods)
 * @param seed RNG seed
 * @param speed Initial linear speed for the two vehicles on the circles. If non-positive, is picked randomly for each vehicle
 * @param max_accel Maximum absolute acceleration for angular speed. If non-positive, both accelerations are set to zero
 * @param I_min Start of the search interval
 * @param I_max End of the search interval
 * @param precision Required precision result
 * @return LinAccCircleCircle<T,Policies> A Circle-circle test case with linearly varying angular speed
 */
template<typename T, class Policies>
LinAccCircleCircle<T,Policies> generate_linacccirclecircle_test_case(int seed, T speed, T max_accel, T I_min, T I_max, T precision)
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

    T a1,a2;
    if (max_accel > 0)
    {
        std::uniform_real_distribution<T> dis_accel(-max_accel, max_accel);
        a1 = dis_accel(gen);
        a2 = dis_accel(gen);
    }
    else
    {
        a1 = 0;
        a2 = 0;
    }

    T Dphi;
    std::uniform_real_distribution<T> dis_dphi(-M_PI, M_PI);
    Dphi = dis_dphi(gen);

    T r1,r2;
    std::uniform_real_distribution<T> dis_radius(0.1, 10);

    r1 = dis_radius(gen);
    r2 = dis_radius(gen);

    T Dx,Dy;
    Dx = dis_radius(gen);
    Dy = dis_radius(gen);

    MinimizeTesting::TestValues<T> vals;
    vals.init_left  = I_min;
    vals.init_right = I_max;
    vals.precision  = precision;

    vals.location   = NAN;
    vals.value      = NAN;

    return LinAccCircleCircle<T,Policies>(vals,Dx,Dy,r1,r2,a1,v1/r1,a2,v2/r2,Dphi);
}



/***** Variant with corrected acceleration formula and time-speed based distance computation *****/

template<typename T, class Policies>
class LinAccCircleCircleVDist : public MinimizeTesting::TestCaseWithDerivatives<T,Policies>
{
    private:
        const T Dx,Dy;
        const T r1,r2;
        const T a1,v1;
        const T a2,v2;
        const T phi1,phi2;
        const T tdist;

    public:
        void print_params()
        {
            std::cout   << std::setprecision(5)
                        << "Dx   : " << Dx      << std::endl
                        << "Dy   : " << Dy      << std::endl
                        << "r1   : " << r1      << std::endl
                        << "a1   : " << a1      << std::endl
                        << "v1   : " << v1      << std::endl
                        << "r2   : " << r2      << std::endl
                        << "a2   : " << a2      << std::endl
                        << "v2   : " << v2      << std::endl
                        << "phi1 : " << phi1    << std::endl
                        << "phi2 : " << phi2    << std::endl
                        << "tdist: " << tdist   << std::endl << std::endl;

        }

        LinAccCircleCircleVDist(MinimizeTesting::TestValues<T> vals, T _Dx, T _Dy, T _r1, T _r2, T _a1, T _v1, T _a2, T _v2, T _phi1, T _phi2, T _tdist)
        : MinimizeTesting::TestCaseWithDerivatives<T,Policies>(vals),
            Dx(_Dx), Dy(_Dy), r1(_r1), r2(_r2), a1(_a1), v1(_v1), a2(_a2), v2(_v2), phi1(_phi1), phi2(_phi2), tdist(_tdist) {}

        pure T f(T t) const
        {
            T V1 = (a1*t+v1)*r1;
            T V2 = (a2*t+v2)*r2;
            T Vmax = std::max(V1,V2);

            T theta1 = (0.5*a1*t+v1)*t + phi1;
            T theta2 = (0.5*a2*t+v2)*t + phi2;
            T x = Dx + r1*std::cos(theta1) - r2*std::cos(theta2);
            T y = Dy + r1*std::sin(theta1) - r2*std::sin(theta2);
            return x*x + y*y - tdist*tdist * Vmax*Vmax;
        }

        pure IntervalSolver::interval<T,Policies> F(const IntervalSolver::interval<T,Policies> &I) const
        {
            using namespace boost::numeric;

            IntervalSolver::interval<T,Policies> V1 = (a1*I+v1)*r1;
            IntervalSolver::interval<T,Policies> V2 = (a2*I+v2)*r2;
            IntervalSolver::interval<T,Policies> Vmax = max(V1,V2);

            IntervalSolver::interval<T,Policies> theta1_I = (0.5*a1*square(I)+v1*I) + phi1;
            IntervalSolver::interval<T,Policies> theta2_I = (0.5*a2*square(I)+v2*I) + phi2;
            IntervalSolver::interval<T,Policies> x_I = Dx + r1*cos(theta1_I) - r2*cos(theta2_I);
            IntervalSolver::interval<T,Policies> y_I = Dy + r1*sin(theta1_I) - r2*sin(theta2_I);
            return square(x_I) + square(y_I) - tdist*tdist * square(Vmax);
        }

        pure T f_d(T t) const
        {
            T theta1 = (0.5*a1*t+v1)*t + phi1;
            T theta2 = (0.5*a2*t+v2)*t + phi2;

            T V1 = (a1*t+v1)*r1;
            T V2 = (a2*t+v2)*r2;
            T Vmax,dVmax;
            if (V1 > V2)
            {
                Vmax = V1;
                dVmax = a1*r1;
            }
            else
            {
                Vmax = V2;
                dVmax = a2*r2;
            }

            T c1 = std::cos(theta1);
            T s1 = std::sin(theta1);

            T c2 = std::cos(theta2);
            T s2 = std::sin(theta2);

            T omega1 = a1*t + v1;
            T omega2 = a2*t + v2;

            T x = Dx + r1*c1 - r2*c2;
            T y = Dy + r1*s1 - r2*s2;

            T x_d = -omega1*r1*s1 + omega2*r2*s2;
            T y_d =  omega1*r1*c1 - omega2*r2*c2;

            return (x_d*x + y_d*y - tdist*tdist*dVmax*Vmax)*2;
        }

        pure IntervalSolver::interval<T,Policies> F_d(const IntervalSolver::interval<T,Policies> &I) const
        {
            using namespace boost::numeric;

            interval<T,Policies> theta1_I = (0.5*a1*square(I)+v1*I) + phi1;
            interval<T,Policies> theta2_I = (0.5*a2*square(I)+v2*I) + phi2;

            interval<T,Policies> V1 = (a1*I+v1)*r1;
            interval<T,Policies> V2 = (a2*I+v2)*r2;
            interval<T,Policies> Vmax = max(V1,V2);
            T dV1 = a1*r1;
            T dV2 = a2*r2;
            interval<T,Policies> dVmax(std::min(dV1,dV2), std::max(dV1,dV2));

            interval<T,Policies> c1 = cos(theta1_I);
            interval<T,Policies> s1 = sin(theta1_I);

            interval<T,Policies> c2 = cos(theta2_I);
            interval<T,Policies> s2 = sin(theta2_I);

            interval<T,Policies> omega1_I = a1*I + v1;
            interval<T,Policies> omega2_I = a2*I + v2;

            interval<T,Policies> x_I = Dx + r1*c1 - r2*c2;
            interval<T,Policies> y_I = Dy + r1*s1 - r2*s2;

            interval<T,Policies> x_d_I = -omega1_I*r1*s1 + omega2_I*r2*s2;
            interval<T,Policies> y_d_I =  omega1_I*r1*c1 - omega2_I*r2*c2;
            return (x_d_I*x_I + y_d_I*y_I - tdist*tdist*dVmax*Vmax)*static_cast<T>(2);
        }

        pure T f_dd(T t) const
        {
            T theta1 = (0.5*a1*t+v1)*t + phi1;
            T theta2 = (0.5*a2*t+v2)*t + phi2;

            T V1 = (a1*t+v1)*r1;
            T V2 = (a2*t+v2)*r2;
            T Vmax,dVmax;
            if (V1 > V2)
            {
                Vmax = V1;
                dVmax = a1*r1;
            }
            else
            {
                Vmax = V2;
                dVmax = a2*r2;
            }

            T c1 = std::cos(theta1);
            T s1 = std::sin(theta1);

            T c2 = std::cos(theta2);
            T s2 = std::sin(theta2);

            T omega1 = a1*t + v1;
            T omega2 = a2*t + v2;

            T omega1_d = a1;
            T omega2_d = a2;

            T x = Dx + r1*c1 - r2*c2;
            T y = Dy + r1*s1 - r2*s2;

            T x_d = -omega1*r1*s1 + omega2*r2*s2;
            T y_d =  omega1*r1*c1 - omega2*r2*c2;

            T x_dd = -r1*(omega1*omega1*c1 + omega1_d*s1) + r2*(omega2*omega2*c2 + omega2_d*s2);
            T y_dd =  r1*(-omega1*omega1*s1 + omega1_d*c1) - r2*(-omega2*omega2*s2 + omega2_d*c2);

            return (x_d*x_d+x*x_dd + y_d*y_d+y*y_dd - tdist*tdist*dVmax*dVmax)*2;
        }

        pure IntervalSolver::interval<T,Policies> F_dd(const IntervalSolver::interval<T,Policies> &I) const
        {
            using namespace boost::numeric;

            interval<T,Policies> theta1_I = (0.5*a1*square(I)+v1*I) + phi1;
            interval<T,Policies> theta2_I = (0.5*a2*square(I)+v2*I) + phi2;

            interval<T,Policies> V1 = (a1*I+v1)*r1;
            interval<T,Policies> V2 = (a2*I+v2)*r2;
            interval<T,Policies> Vmax = max(V1,V2);
            T dV1 = a1*r1;
            T dV2 = a2*r2;
            interval<T,Policies> dVmax(std::min(dV1,dV2), std::max(dV1,dV2));

            interval<T,Policies> c1 = cos(theta1_I);
            interval<T,Policies> s1 = sin(theta1_I);

            interval<T,Policies> c2 = cos(theta2_I);
            interval<T,Policies> s2 = sin(theta2_I);

            interval<T,Policies> omega1_I = a1*I + v1;
            interval<T,Policies> omega2_I = a2*I + v2;

            interval<T,Policies> x_I = Dx + r1*c1 - r2*c2;
            interval<T,Policies> y_I = Dy + r1*s1 - r2*s2;

            interval<T,Policies> x_d_I = -omega1_I*r1*s1 + omega2_I*r2*s2;
            interval<T,Policies> y_d_I =  omega1_I*r1*c1 - omega2_I*r2*c2;

            interval<T,Policies> x_dd_I = -r1*(square(omega1_I)*c1 + a1*s1) + r2*(square(omega2_I)*c2 + a2*s2);
            interval<T,Policies> y_dd_I =  r1*(-square(omega1_I)*s1 + a1*c1) - r2*(-square(omega2_I)*s2 + a2*c2);
            return (square(x_d_I)+x_I*x_dd_I + square(y_d_I)+y_I*y_dd_I - tdist*tdist*dVmax*dVmax)*static_cast<T>(2);
        }

};

template<typename T, class Policies>
LinAccCircleCircleVDist<T,Policies> linacccirclecirclevdist_easy_special_case(T precision)
{
    MinimizeTesting::TestValues<T> vals;
    vals.init_left  = 0;
    vals.init_right = 3;
    vals.precision  = precision;
    vals.location   = 2.081770418654643169728874842;
    vals.value      = 0.027119781606604591528668;
    vals.hotstart   = 1.8;

    return LinAccCircleCircleVDist<T,Policies>(vals,
        1,1,1,2,2,2,4,1,0,M_PI,0);
}

template<typename T, class Policies>
LinAccCircleCircleVDist<T,Policies> linacccirclecirclevdist_hard_special_case(T precision)
{
    MinimizeTesting::TestValues<T> vals;
    vals.init_left  = 0;
    vals.init_right = 3;
    vals.precision  = precision;
    vals.location   = 2.081770418654643169728874842;
    vals.value      = 0.027119781606604591528668;
    vals.hotstart   = -1;

    return LinAccCircleCircleVDist<T,Policies>(vals,
        1,1,1,2,2,2,4,1,0,M_PI,0);
}

/**
 * @brief Generate a random test case for the corrected circle-circle distance function with linear acceleration
 * 
 * @tparam T Float subtype (float,T, long T)
 * @tparam Policies Rounding policies (relevant for Interval methods)
 * @param seed RNG seed
 * @param speed Initial linear speed for the two vehicles on the circles. If non-positive, is picked randomly for each vehicle
 * @param max_accel Maximum absolute acceleration for angular speed. If non-positive, both accelerations are set to zero
 * @param max_tdist Maximum time distance scaling. If non-positive, time-distance correction is disabled
 * @param I_min Start of the search interval
 * @param I_max End of the search interval
 * @param precision Required precision result
 * @return LinAccCircleCircleVDist<T,Policies> A corrected Circle-circle test case with linearly varying angular speed
 */
template<typename T, class Policies>
LinAccCircleCircleVDist<T,Policies> generate_linacccirclecirclevdist_test_case(
    int seed, T speed, T max_accel, T max_tdist, T I_min, T I_max, T precision)
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

    T a1,a2;
    if (max_accel > 0)
    {
        std::uniform_real_distribution<T> dis_accel(-max_accel, max_accel);
        a1 = dis_accel(gen);
        a2 = dis_accel(gen);
    }
    else
    {
        a1 = 0;
        a2 = 0;
    }

    T phi1,phi2;
    std::uniform_real_distribution<T> dis_phi(-M_PI, M_PI);
    phi1 = dis_phi(gen);
    phi2 = dis_phi(gen);

    T r1,r2;
    std::uniform_real_distribution<T> dis_radius(0.1, 10);

    r1 = dis_radius(gen);
    r2 = dis_radius(gen);

    T Dx,Dy;
    Dx = dis_radius(gen);
    Dy = dis_radius(gen);

    T tdist;
    if (max_tdist > 0)
    {
        std::uniform_real_distribution<T> dis_tdist(0, max_tdist);
        tdist = dis_tdist(gen);
    }
    else
    {
        tdist = 0;
    }

    MinimizeTesting::TestValues<T> vals;
    vals.init_left  = I_min;
    vals.init_right = I_max;
    vals.precision  = precision;

    vals.location   = NAN;
    vals.value      = NAN;

    return LinAccCircleCircleVDist<T,Policies>(vals,Dx,Dy,r1,r2,a1,v1/r1,a2,v2/r2,phi1,phi2,tdist);
}