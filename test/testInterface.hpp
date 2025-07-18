// Copyright (c) 2025 Mael FEURGARD <mael.feurgard@enac.fr>
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT
#pragma once

#include <functional>
#include <optional>

#include "IntervalSolver.hpp"

// -------------------------------------------------- //
// Testing setup

namespace IntervalTesting
{

    using namespace IntervalSolver;

    template<typename T, class Policy>
    struct test_case 
    {
        T location;     /// Where the minimum is located
        T value;        /// The value of f at the minimum
        T init_left;    /// Lower bound of the initial research interval
        T init_right;   /// Upper bound of the initial research interval
        T precision;    /// Precision desired for the minimum location and value
        std::optional<T> lower_bound = std::nullopt;    /// A known lower-bound on the global minimum

        std::function<T(T)> f;                                          /// The function the minimum is to be found
        std::function<interval<T,Policy>(const interval<T,Policy>&)> F; /// f translation in interval algebra
        std::optional<std::function<T(T)>> f_d = std::nullopt;                                              /// f First derivative
        std::optional<std::function<interval<T,Policy>(const interval<T,Policy>&)>> F_d = std::nullopt;     /// f_d translation in the interval algebra 
        std::optional<std::function<T(T)>> f_dd = std::nullopt;                                             /// f Seconde derivative
        std::optional<std::function<interval<T,Policy>(const interval<T,Policy>&)>> F_dd = std::nullopt;    /// f_dd translation in the interval algebra
    };

    /**
     * @brief Function to test an Interval Global minimization test case
     * 
     * @tparam T        : Scalar type (either float, double or long)
     * @tparam Policies : A Policy for rounding and checking, from boost::numeric::interval
     * @param t         : The test case structure
     * @return true     : Test successful, minimum is at the correct location and have the correct value
     * @return false    : Test failure
     */
    template<typename T, class Policy>
    bool test_interval_with_derivatives(const struct test_case<T,Policy> &t)
    {
        assert(t.f_d.has_value()  );
        assert(t.F_d.has_value()  );
        assert(t.f_dd.has_value() );
        assert(t.F_dd.has_value() );

        T min_res = IntervalSolver::minimize_interval_method<T,Policy>(
            t.init_left,t.init_right,
            t.f,t.F,
            t.f_d.value(),t.F_d.value(),
            t.f_dd.value(),t.F_dd.value(),
            t.precision,t.lower_bound
        );

        return (abs(min_res - t.location) < t.precision) && (abs(t.f(min_res) - t.value) < t.precision);
    }

    /**
     * @brief Function to test an Interval Global minimization test case, but with the derivatives-free method
     * 
     * @tparam T        : Scalar type (either float, double or long)
     * @tparam Policies : A Policy for rounding and checking, from boost::numeric::interval
     * @param t         : The test case structure
     * @return true     : Test successful, minimum is at the correct location and have the correct value
     * @return false    : Test failure
     */
    template<typename T, class Policy>
    bool test_interval_no_derivatives(const struct test_case<T,Policy> &t)
    {
        T min_res = IntervalSolver::minimize_interval_method<T,Policy>(
            t.init_left,t.init_right,
            t.f,t.F,
            t.precision,t.lower_bound
        );

        return (abs(min_res - t.location) < t.precision) && (abs(t.f(min_res) - t.value) < t.precision);
    }

}
