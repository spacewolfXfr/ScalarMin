// Copyright (c) 2025 Mael FEURGARD <mael.feurgard@enac.fr>
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "IntervalSolver.hpp"

template<typename T, class Policies>
std::pair<std::optional<interval<T, Policies>>,std::optional<interval<T, Policies>>> interval_reduction(interval<T, Policies> &X0,
    std::function<const T(T)> f,
    std::function<interval<T, Policies>(interval<T, Policies>)> F,
    std::function<const T(T)> f_d,
    std::function<interval<T, Policies>(interval<T, Policies>)> F_d,
    std::function<const T(T)> f_dd,
    std::function<interval<T, Policies>(interval<T, Policies>)> F_dd,
    T x, T f_x, T f_d_x, T best_value
)
{
    typedef interval<T, Policies> inteval_t;
    
    inteval_t dd_interval = F_dd(X0);
    T u = dd_interval.lower();
    T v = dd_interval.upper();

    // Part 5.
    // Concavity argument: if v < 0, then f is concave on X0, and as such cannot contain a minimum
    if (v < 0)
    {
        return std::pair(nullopt,nullopt);
    }

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
        return std::pair(nullopt,nullopt);
    }

    if (u == 0) // (&& delta >= 0)
    {
        if (f_d_x == 0)
        {
            return std::pair(nullopt,nullopt);
        }
        else if (f_d_x > 0)
        {
            inteval_t filter(inteval_t::neg_inf(),x+E/f_d_x);
            inteval_t output = intersect(X0,filter);
            return std::pair(output,nullopt);
        }
        else // (f_d_x < 0)
        {
            inteval_t filter(x+E/f_d_x,inteval_t::pos_inf());
            inteval_t output = intersect(X0,filter);
            return std::pair(output,nullopt);
        }
        
    }
    else // (u != 0 && delta > 0)
    {
        T a = u/2;
        T b = f_d_x - x*u;
        T r1 = (-b+sqrt(delta))/(2*a);
        T r2 = (-b-sqrt(delta))/(2*a);

        T r_min = min(r1,r2);
        T r_max = max(r1,r2);

        if (u > 0)
        {
            inteval_t filter(r_min,r_max);
            inteval_t output = intersect(X0,filter);
            return pair(output,nullopt);
        }
        else // (u < 0)
        {
            inteval_t filter_min(inteval_t::neg_inf(),r_min);
            inteval_t output_min = intersect(X0,filter_min);

            inteval_t filter_max(r_max,inteval_t::pos_inf());
            inteval_t output_max = intersect(X0,filter_max);
            return std::pair(output_min,output_max);
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
    std::optional<T> lower_bound
)
{
    typedef interval<T, Policies> inteval_t;
    // 1.
    assert(left < right);
    interval<T, Policies> X0(left,right);
    list<inteval_t> interval_list;
    interval_list.push_back(X0);

    // 2.
    T best_loc = left;
    T best = f(left);
    
    if (f(right) < best)
    {
        best = f(right);
        best_loc = right;
    }

    {
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
        inteval_t X = interval_list.front();

        // b.
        inteval_t F0 = F(X);
        if (F0.lower() > best)
        {
            continue;
        }

        // c.
        best = min(best,F0.upper());

        // g.

        T x_mid = median(X);
        T f_mid = f(x_mid);
        T f_d_mid = f_d(x_mid);
        if (f_mid < best)
        {
            best = f_mid;
            best_loc = x_mid;
        }

        auto candidate_intervals = interval_reduction(X,f,F,f_d,F_d,f_dd,F_dd,x_mid,f_mid,f_d_mid,best);
        auto interval_1 = candidate_intervals.first;
        auto interval_2 = candidate_intervals.second;

        if (interval_1)
        {
            inteval_t i1 = interval_1.value();
            if (width(i1) > epsilon)
            {
                interval_list.push_back(i1);
            }
        }

        if (interval_2)
        {
            inteval_t i2 = interval_2.value();
            if (width(i2) > epsilon)
            {
                interval_list.push_back(i2);
            }
        }

        // Pop at the end to avoid killing the reference
        interval_list.pop_front();
    }

    return best;
}


template<typename T, class Policies>
T minimize_interval_method(T left, T right, 
    std::function<T(const T&)> f,
    std::function<interval<T, Policies>(const interval<T, Policies>&)> F,
    T epsilon,
    std::optional<T> lower_bound
)
{
    typedef interval<T, Policies> inteval_t;
    
    // 1.
    assert(left < right);
    inteval_t X0(left,right);
    list<inteval_t> interval_list;
    interval_list.push_back(X0);

    // 2.
    T best_loc = left;
    T best = f(left);
    
    if (f(right) < best)
    {
        best = f(right);
        best_loc = right;
    }

    {
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
        inteval_t X = interval_list.front();

        // b.
        inteval_t F0 = F(X);
        if (F0.lower() > best)
        {
            continue;
        }

        // c.
        best = min(best,F0.upper());

        // g.

        T x_mid = median(X);
        T x_val = f(x_mid);
        if (x_val < best)
        {
            best = x_val;
            best_loc = x_mid;
        }

        T a = X.lower();
        T b = X.upper();

        if (std::abs(a - x_mid) > epsilon)
        {
            inteval_t i_low(a,x_mid);
            interval_list.push_back(i_low);
        }

        if (std::abs(b - x_mid) > epsilon)
        {
            inteval_t i_high(x_mid,b);
            interval_list.push_back(i_high);
        }
        
        // Pop at the end to avoid killing the reference
        interval_list.pop_front();
    }

    return best;
}