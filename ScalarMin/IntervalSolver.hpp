// Copyright (c) 2025 Mael FEURGARD <mael.feurgard@enac.fr>
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT
#pragma once

#include <boost/numeric/interval.hpp>

#include <list>
#include <functional>
#include <optional>
#include <utility>
#include <array>

#if DEBUG_STDOUT >= 1
#include <iostream>
#endif

#if DEBUG_VISUALISE >= 1

#include "Visualisation/Visualisation.hpp"
#pragma message "Building with visualisation"

#ifndef DEBUG_VISUALISE_SAMPLES
#warning DEBUG_VISUALISE_SAMPLES is undefined whereas DEBUG_VISUALISE is! Falling back to default value of 200
#define DEBUG_VISUALISE_SAMPLES 200
#endif

#if DEBUG_VISUALISE_SAMPLES <= 0
#warning DEBUG_VISUALISE_SAMPLES is ill-defined (less than 0) ! Falling back to default value of 200
#define DEBUG_VISUALISE_SAMPLES 200
#endif

#endif


namespace IntervalSolver
{
    template<typename T, class Policies>
    using interval = boost::numeric::interval<T, Policies>;

    // A list of codes, for inspiration: https://arnold-neumaier.at/glopt/software_g.html#bb_codes

    /**
     * @brief Global minimization algorithm based on intervals algebra, requiring first and second derivatives
     * 
     * Algorithm based on: "Global optimization using interval analysis: The one-dimensional case", 1979 by Hansen in 'Journal of Optimization Theory and Applications'
     * (https://doi.org/10.1007/BF00933139)
     * 
     * Implementation relying on the Boost Interval library: https://www.boost.org/doc/libs/latest/libs/numeric/interval/doc/interval.htm
     * 
     * @tparam T        : Scalar type (either float, double or long)
     * @tparam Policies : Policies for rounding and checking, from boost::numeric::interval
     * @param left          : Left side of the initial search interval
     * @param right         : Right side of the initial search interval
     * @param f             : Function to minimize
     * @param F             : The interval extension of f
     * @param f_d           : First derivative of f
     * @param F_d           : The interval extension of f_d  
     * @param f_dd          : Second derivative of f
     * @param F_dd          : The interval extension of f_dd 
     * @param epsilon       : The precision for the minimum location
     * @param hot_start     : A guess for the minimum location (optional)
     * @param lower_bound   : A lower bound on the local minimum (optional) 
     * @return T        : The global minimum value of f on [left,right]
     */
    template<typename T, class Policies>
    T minimize_interval_method(T left, T right, 
        std::function<T(const T&)> f,
        std::function<interval<T, Policies>(const interval<T, Policies>&)> F,
        std::function<T(const T&)> f_d,
        std::function<interval<T, Policies>(const interval<T, Policies>&)> F_d,
        std::function<T(const T&)> f_dd,
        std::function<interval<T, Policies>(const interval<T, Policies>&)> F_dd,
        T epsilon,
        std::optional<T> hot_start   = std::nullopt,
        std::optional<T> lower_bound = std::nullopt
    );

    /**
     * @brief A simpler Global minimization algorithm based on intervals algebra, derivatives-free
     * 
     * Algorithm based on: "Global optimization using interval analysis: The one-dimensional case", 1979 by Hansen in 'Journal of Optimization Theory and Applications'
     * (https://doi.org/10.1007/BF00933139)
     * 
     * Implementation relying on the Boost Interval library: https://www.boost.org/doc/libs/latest/libs/numeric/interval/doc/interval.htm
     * 
     * @tparam T        : Scalar type (either float, double or long)
     * @tparam Policies : Policies for rounding and checking, from boost::numeric::interval
     * @param left          : Left side of the initial search interval
     * @param right         : Right side of the initial search interval
     * @param f             : Function to minimize
     * @param F             : The interval extension of f
     * @param epsilon       : The precision for the minimum location
     * @param hot_start     : A guess for the minimum location (optional)
     * @param lower_bound   : A lower bound on the local minimum (optional) 
     * @return T        : The global minimum value of f on [left,right]
     */
    template<typename T, class Policies>
    T minimize_interval_method(T left, T right, 
        std::function<T(const T&)> f,
        std::function<interval<T, Policies>(const interval<T, Policies>&)> F,
        T epsilon,
        std::optional<T> hot_start   = std::nullopt,
        std::optional<T> lower_bound = std::nullopt
    );

    // ----- Template implementations ----- //

    template<typename T, class Policies>
    std::array<interval<T, Policies>,4> interval_reduction(interval<T, Policies> &X0,
        BOOST_ATTRIBUTE_UNUSED std::function<T(const T&)> f,
        BOOST_ATTRIBUTE_UNUSED std::function<interval<T, Policies>(const interval<T, Policies>&)> F,
        BOOST_ATTRIBUTE_UNUSED std::function<T(const T&)> f_d,
        BOOST_ATTRIBUTE_UNUSED std::function<interval<T, Policies>(const interval<T, Policies>&)> F_d,
        BOOST_ATTRIBUTE_UNUSED std::function<T(const T&)> f_dd,
        std::function<interval<T, Policies>(const interval<T, Policies>&)> F_dd,
        T x, T f_x, T f_d_x, T best_value, T zero_tol=1e-30
    )
    {
        typedef interval<T, Policies> interval_t;

        std::array<interval_t,4> output{
            interval_t::empty(),
            interval_t::empty(),
            interval_t::empty(),
            interval_t::empty()
        };
        
        interval_t dd_interval = F_dd(X0);
        T u = dd_interval.lower();
        T v = dd_interval.upper();

        // Part 5.
        // Concavity argument: if v < 0, then f is concave on X0, and as such cannot contain a minimum
        if (v < 0)
        {
            return output;
        }

        // Part 6.
        // Newton's step
        interval_t S1;
        interval_t S2;

        if (u > 0)
        {
            S1 = x - f_d_x/dd_interval;
            S2 = interval_t::empty();
        }
        else
        {
            S1 = interval_t::empty();
            S2 = interval_t::empty();

            if (f_d_x >= 0)
            {
                if (v > 0)
                {
                    T d = x - f_d_x/v;
                    if (d >= X0.lower())
                    {
                        S1 = interval_t(X0.lower(),d);
                    }
                }

                if (u < 0)
                {
                    T c = x - f_d_x/u;
                    if (c <= X0.upper())
                    {
                        S2 = interval_t(c,X0.upper());
                    }
                }
            }
            else
            {
                if (u < 0)
                {
                    T c = x - f_d_x/u;
                    if (c >= X0.lower())
                    {
                        S1 = interval_t(X0.lower(),c);
                    }
                }

                if (v > 0)
                {
                    T d = x - f_d_x/v;
                    if (d <= X0.upper())
                    {
                        S2 = interval_t(d,X0.upper());
                    }
                }
            }

        }

        interval_t Xs1 = intersect(X0,S1);
        interval_t Xs2 = intersect(X0,S2);

        // Part 7.
        // Using Taylor expension, we get rid of values y in X0 such that f(y) >= best_value
        // i.e: f(x) + (y-x)f'(x) + (y-x)²f''(z) >= best_value ; with some x in X0 and z in X0, unknown (Taylor Lagrange remainder)
        // By interval algebra, we know that f''(z) is in [u,v] := f''(X0)
        // Hence f''(z) >= u

        T E = best_value - f_x;

        // We solve the following polynomial inequality in y:
        // (y-x)f'(x) + (y-x)² u >= E := best_value - f(x)

        T delta = f_d_x * f_d_x + 2 * E * u;
        if (delta < 0)
        {
            return output;
        }

        if (abs(u) < zero_tol) // u ~= 0 (&& delta >= 0)
        {
            if (abs(f_d_x) < zero_tol)
            {
                return output;
            }
            else if (f_d_x > 0)
            {
                interval_t filter(Policies::checking::neg_inf(),x+E/f_d_x);
                output[0] = intersect(Xs1,filter);
                output[1] = intersect(Xs2,filter);
                return output;
            }
            else // (f_d_x < 0)
            {
                interval_t filter(x+E/f_d_x,Policies::checking::pos_inf());
                output[0] = intersect(Xs1,filter);
                output[1] = intersect(Xs2,filter);
                return output;
            }
            
        }
        else // (u != 0 && delta > 0)
        {
            T a = u/2;
            T b = f_d_x - x*u;
            T r1 = (-b+sqrt(delta))/(2*a);
            T r2 = (-b-sqrt(delta))/(2*a);

            T r_min = std::min(r1,r2);
            T r_max = std::max(r1,r2);

            if (u > 0)
            {
                interval_t filter(r_min,r_max);
                output[0] = intersect(Xs1,filter);
                output[1] = intersect(Xs2,filter);
                return output;
            }
            else // (u < 0)
            {
                interval_t filter_min(Policies::checking::neg_inf(),r_min);
                output[0] = intersect(Xs1,filter_min);
                output[1] = intersect(Xs2,filter_min);

                interval_t filter_max(r_max,Policies::checking::pos_inf());
                output[2] = intersect(Xs1,filter_max);
                output[3] = intersect(Xs2,filter_max);
                return output;
            }
        }
    }


    template<typename T, class Policies>
    T minimize_interval_method(T left, T right, 
        std::function<T(const T&)> f,
        std::function<interval<T, Policies>(const interval<T, Policies>&)> F,
        std::function<T(const T&)> f_d,
        std::function<interval<T, Policies>(const interval<T, Policies>&)> F_d,
        std::function<T(const T&)> f_dd,
        std::function<interval<T, Policies>(const interval<T, Policies>&)> F_dd,
        T epsilon,
        std::optional<T> hot_start  ,
        std::optional<T> lower_bound
    )
    {

        // 0. Debug (visualisation) setup if needed
#ifdef DEBUG_VISUALISE
        using namespace Visualisation;
#endif

        typedef interval<T, Policies> interval_t;

        std::list<interval_t> interval_list;
        
        // 2. Initialize minimum estimate
        T best_loc = left;
        T best = f(left);
        
        if (f(right) < best)
        {
            best = f(right);
            best_loc = right;
        }

        // 1. Initialize search list (and additional inital value)
        assert(left < right);

        if (hot_start.has_value())
        {
            T hloc = hot_start.value();
            assert(left <= hloc && hloc <= right);
            interval<T, Policies> X0l(left,hloc);
            interval<T, Policies> X0r(hloc,right);

            interval_list.push_back(X0l);
            interval_list.push_back(X0r);

            if (f(hloc) < best)
            {
                best = f(hloc);
                best_loc = hloc;
            }
        }
        else
        {
            interval<T, Policies> X0(left,right);
            interval_list.push_back(X0);
            
            T mid = median(X0);
            if (f(mid) < best)
            {
                best = f(mid);
                best_loc = mid;
            }
        }

        //3.
        while (!interval_list.empty())
        {
            // a.
            interval_t X = interval_list.front();

#ifdef DEBUG_VISUALISE
            Plot2D plot = init_plot(left,right);

            plot_function(left,right,DEBUG_VISUALISE_SAMPLES,f,plot).label("Obj function(x)");
            plot_hline(left,right,best,plot).label("Best_known value");

            plot_intervals(interval_list,F,plot);
            Figure fig = {{plot}};
            Canvas canvas = {{fig}};
            canvas.show();
#endif

            // b.
            interval_t F0 = F(X);
            if (F0.lower() > best)
            {
                interval_list.pop_front();
                continue;
            }

            // c.
            best = std::min(best,F0.upper());

            // g.

            T x_mid = median(X);
            T f_mid = f(x_mid);
            T f_d_mid = f_d(x_mid);
            if (f_mid < best)
            {
                best = f_mid;
                best_loc = x_mid;

#if DEBUG_STDOUT >= 1
                std::cout   << "Current estimate (location,value): ( " << best_loc << " , " << best << " )" << std::endl
                            << "Processed interval  : [ " << X.lower() << " , " << X.upper() << " ]  (size: " << width(X) << " )" << std::endl
                            << "Interval image      : [ " << F0.lower() << " , " << F0.upper() << " ]" << std::endl << std::endl;
#endif

                if (lower_bound.has_value())
                {
                    if (abs(best-lower_bound.value()) < epsilon)
                    {
                        return best_loc;
                    }
                }
            }

            auto candidate_intervals = interval_reduction(X,f,F,f_d,F_d,f_dd,F_dd,x_mid,f_mid,f_d_mid,best);
            for(auto& interval : candidate_intervals)
            {
                if (width(interval) > epsilon/4)
                {
                    interval_list.push_back(interval);
                }
                else
                {
                    if (!empty(interval))
                    {
                        T candidate_loc = median(interval);
                        T candidate = f(candidate_loc);
                        if (candidate < best)
                        {
                            best = candidate;
                            best_loc = candidate_loc;
                        }
                    }
                }
            }

            // Pop at the end to avoid killing the reference
            interval_list.pop_front();
        }

        return best_loc;
    }


    template<typename T, class Policies>
    T minimize_interval_method(T left, T right, 
        std::function<T(const T&)> f,
        std::function<interval<T, Policies>(const interval<T, Policies>&)> F,
        T epsilon,
        std::optional<T> hot_start  ,
        std::optional<T> lower_bound
    )
    {
        // 0. Debug (visualisation) setup if needed
#ifdef DEBUG_VISUALISE
        using namespace Visualisation;
#endif
        
        typedef interval<T, Policies> interval_t;

        std::list<interval_t> interval_list;
        
        // 2. Initialize minimum estimate
        T best_loc = left;
        T best = f(left);
        
        if (f(right) < best)
        {
            best = f(right);
            best_loc = right;
        }

        // 1. Initialize search list (and additional inital value)
        assert(left < right);

        if (hot_start.has_value())
        {
            T hloc = hot_start.value();
            assert(left <= hloc && hloc <= right);
            interval<T, Policies> X0l(left,hloc);
            interval<T, Policies> X0r(hloc,right);

            interval_list.push_back(X0l);
            interval_list.push_back(X0r);

            if (f(hloc) < best)
            {
                best = f(hloc);
                best_loc = hloc;
            }
        }
        else
        {
            interval<T, Policies> X0(left,right);
            interval_list.push_back(X0);
            
            T mid = median(X0);
            if (f(mid) < best)
            {
                best = f(mid);
                best_loc = mid;
            }
        }

        //3. Loop through the search intervals
        while (!interval_list.empty())
        {
            // a. Get a search interval
            interval_t X = interval_list.front();

            // a(bis). Debug visualisation
#ifdef DEBUG_VISUALISE
            Plot2D plot = init_plot(left,right);

            plot_function(left,right,DEBUG_VISUALISE_SAMPLES,f,plot).label("Obj function(x)");
            plot_hline(left,right,best,plot).label("Best_known value");

            plot_intervals(interval_list,F,plot);
            Figure fig = {{plot}};
            Canvas canvas = {{fig}};
            canvas.show();
#endif

            // b. Estimate its image; discard it if its lowest is greater than the current estimate of the minimum
            interval_t F0 = F(X);
            if (F0.lower() > best)
            {
                interval_list.pop_front();
                continue;
            }

            // c. Try to improve the minimum by using the worst value of the interval
            best = std::min(best,F0.upper());

            // d. Evaluate f at some point in X : we pick the middle point
            T x_mid = median(X);
            T x_val = f(x_mid);

            // e. Improve the minimum estimate if possible
            if (x_val < best)
            {
                best = x_val;
                best_loc = x_mid;

#if DEBUG_STDOUT >= 1
                std::cout   << "Current estimate (location,value): ( " << best_loc << " , " << best << " )" << std::endl
                            << "Processed interval  : [ " << X.lower() << " , " << X.upper() << " ]  (size: " << width(X) << " )" << std::endl
                            << "Interval image      : [ " << F0.lower() << " , " << F0.upper() << " ]" << std::endl << std::endl;
#endif


                if (lower_bound.has_value())
                {
                    if (abs(best-lower_bound.value()) < epsilon)
                    {
                        return best_loc;
                    }
                }
            }

            // f. Split the interval in two subparts; keep only those large enough
            T a = X.lower();
            T b = X.upper();

            if (std::abs(a - x_mid) > epsilon/4)
            {
                interval_t i_low(a,x_mid);
                interval_list.push_back(i_low);
            }

            if (std::abs(b - x_mid) > epsilon/4)
            {
                interval_t i_high(x_mid,b);
                interval_list.push_back(i_high);
            }
            
            
            // Pop at the end to avoid killing the reference
            interval_list.pop_front();

            
        }

        return best_loc;
    }
};
