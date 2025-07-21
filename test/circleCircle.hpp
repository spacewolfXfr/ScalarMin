// Copyright (c) 2025 Mael FEURGARD <mael.feurgard@enac.fr>
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "testInterface.hpp"

#include <cmath>

template<typename T, class Policies>
class CircleCircle : public IntervalTesting::TestCaseWithDerivatives
{

    private:
        const T Dx,Dy;
        const T r1,r2;
        const T w1,w2;
        const T Dphi;

    public:
        T f(T t)
        {
            T x = Dx + r1*std::cos(w1*t) - r2*std::cos(w2*t+Dphi);
            T y = Dy + r1*std::sin(w1*t) - r2*std::sin(w2*t+Dphi);
            return x*x + y*y;
        } 

        IntervalTesting::interval<T,Policies> F(const IntervalTesting::interval<T,Policies> &I)
        {
            using boost::numeric;

            interval<T,Policies> x_I = Dx + r1*cos(w1*I) - r2*cos(w2*I+Dphi);
            interval<T,Policies> y_I = Dy + r1*sin(w1*I) - r2*sin(w2*I+Dphi);
            return square(x_I) + square(y_I);
        }

        T f_d(T t)
        {
            T c1 = std::cos(w1*t);
            T s1 = std::sin(w1*t);

            T c2 = std::cos(w2*t+Dphi);
            T s2 = std::sin(w2*t+Dphi);

            T x = Dx + r1*c1 - r2*c2;
            T y = Dy + r1*s1 - r2*s2;

            T x_d = -w1*r1*s1 + w2*r2*s2;
            T y_d =  w1*r1*c1 - w2*r2*c2;

            return (x_d*x + y_d*y)*2;
        } 

        IntervalTesting::interval<T,Policies> F_d(const IntervalTesting::interval<T,Policies> &I)
        {
            using boost::numeric;

            interval<T,Policies> c1 = cos(w1*t);
            interval<T,Policies> s1 = sin(w1*t);

            interval<T,Policies> c2 = cos(w2*t+Dphi);
            interval<T,Policies> s2 = sin(w2*t+Dphi);

            interval<T,Policies> x_I = Dx + r1*c1 - r2*c2;
            interval<T,Policies> y_I = Dy + r1*s1 - r2*s2;

            interval<T,Policies> x_d_I = -w1*r1*s1 + w2*r2*s2;
            interval<T,Policies> y_d_I =  w1*r1*c1 - w2*r2*c2;
            return (x_d_I*x_I + y_d_I*y_I)*2;
        }

        T f_dd(T t)
        {
            T c1 = std::cos(w1*t);
            T s1 = std::sin(w1*t);

            T c2 = std::cos(w2*t+Dphi);
            T s2 = std::sin(w2*t+Dphi);

            T x = Dx + r1*c1 - r2*c2;
            T y = Dy + r1*s1 - r2*s2;

            T x_d = -w1*r1*s1 + w2*r2*s2;
            T y_d =  w1*r1*c1 - w2*r2*c2;

            T x_dd = -w1*w1*r1*c1 + w2*w2*r2*c2;
            T y_dd = -w1*w1*r1*s1 + w2*w2*r2*s2;

            return (x_d*x_d+x*x_dd + y_d*y_d+y*y_dd)*2;
        } 

        IntervalTesting::interval<T,Policies> F_dd(const IntervalTesting::interval<T,Policies> &I)
        {
            using boost::numeric;

            interval<T,Policies> c1 = cos(w1*t);
            interval<T,Policies> s1 = sin(w1*t);

            interval<T,Policies> c2 = cos(w2*t+Dphi);
            interval<T,Policies> s2 = sin(w2*t+Dphi);

            interval<T,Policies> x_I = Dx + r1*c1 - r2*c2;
            interval<T,Policies> y_I = Dy + r1*s1 - r2*s2;

            interval<T,Policies> x_d_I = -w1*r1*s1 + w2*r2*s2;
            interval<T,Policies> y_d_I =  w1*r1*c1 - w2*r2*c2;

            interval<T,Policies> x_dd_I = -w1*w1*r1*c1 + w2*w2*r2*c2;
            interval<T,Policies> y_dd_I = -w1*w1*r1*s1 + w2*w2*r2*s2;
            
            return (square(x_d_I)+x_I*x_x_dd_I + square(y_d_I)+y_I*y_dd_I)*2;
        }

};