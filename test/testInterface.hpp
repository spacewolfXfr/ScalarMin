// Copyright (c) 2025 Mael FEURGARD <mael.feurgard@enac.fr>
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT
#pragma once

#include <functional>
#include <optional>
#include <iostream>

#include "IntervalSolver.hpp"


// About these GCC attributes, symbolizing 'pure, mathematical functions': https://stackoverflow.com/questions/29117836/attribute-const-vs-attribute-pure-in-gnu-c

#define pure __attribute__((pure))
#define puremath __attribute__((const))


// -------------------------------------------------- //
// Testing setup

namespace IntervalTesting
{

    using namespace IntervalSolver;

    template<typename T, class Policies>
    class TestCase
    {
    protected:
        T location;     /// Where the minimum is located
        T value;        /// The value of f at the minimum
        T init_left;    /// Lower bound of the initial research interval
        T init_right;   /// Upper bound of the initial research interval
        T precision;    /// Precision desired for the minimum location and value
        std::optional<T> lower_bound = std::nullopt;    /// A known lower-bound on the global minimum

        void print_results(T expected_loc, T expected_val,
                    T loc, T val, T epsilon)
        {

            bool loc_check = abs(expected_loc-loc) < epsilon;
            bool val_check = abs(expected_val-val) < epsilon;

            auto loc_txt = (loc_check) ? "True" : "False";
            auto val_txt = (val_check) ? "True" : "False";


            std::cout   << "/----------------------------------------\\" << std::endl
                        << "| Tests results, with precision " << epsilon << std::endl << std::endl;

            std::cout   << "| --- Value ---"                                        << std::endl
                        << "| Expected value   : "  << expected_val                 << std::endl
                        << "| Got value        : "  << val                          << std::endl
                        << "| Value diff       : "  << abs(expected_val-val)        << std::endl
                        << "| Value check      : "  << val_txt                      << std::endl << std::endl;


            std::cout   << "| --- Location ---"                                     << std::endl
                        << "| Expected location   : "   << expected_loc             << std::endl
                        << "| Got location        : "   << loc                      << std::endl
                        << "| Location diff       : "   << abs(expected_loc-loc)    << std::endl
                        << "| Location check      : "   << loc_txt                  << std::endl << std::endl;
        }


    public:
        virtual T f(T) = 0;                                             /// The function the minimum is to be found
        virtual interval<T,Policies> F(const interval<T,Policies>&) = 0;    /// f translation in interval algebra


    };

    template<typename T, class Policies>
    class TestCaseWithDerivatives : public TestCase
    {
    public:
        virtual T f_d(T) = 0;                                           /// f First derivative
        virtual interval<T,Policies> F_d(const interval<T,Policies>&) = 0;  /// f_d translation in the interval algebra
        virtual T f_dd(T) = 0;                                          /// f Seconde derivative
        virtual interval<T,Policies> F_dd(const interval<T,Policies>&) = 0; /// f_dd translation in the interval algebra
    };

    template<typename T, class Policies>
    struct test_case 
    {
        T location;     /// Where the minimum is located
        T value;        /// The value of f at the minimum
        T init_left;    /// Lower bound of the initial research interval
        T init_right;   /// Upper bound of the initial research interval
        T precision;    /// Precision desired for the minimum location and value
        std::optional<T> lower_bound = std::nullopt;    /// A known lower-bound on the global minimum

        std::function<T(T)> f;                                          /// The function the minimum is to be found
        std::function<interval<T,Policies>(const interval<T,Policies>&)> F; /// f translation in interval algebra
        std::optional<std::function<T(T)>> f_d = std::nullopt;                                              /// f First derivative
        std::optional<std::function<interval<T,Policies>(const interval<T,Policies>&)>> F_d = std::nullopt;     /// f_d translation in the interval algebra 
        std::optional<std::function<T(T)>> f_dd = std::nullopt;                                             /// f Seconde derivative
        std::optional<std::function<interval<T,Policies>(const interval<T,Policies>&)>> F_dd = std::nullopt;    /// f_dd translation in the interval algebra
    };

    template<typename T>
    void print_results(T expected_loc, T expected_val,
                    T loc, T val, T epsilon)
    {

        bool loc_check = abs(expected_loc-loc) < epsilon;
        bool val_check = abs(expected_val-val) < epsilon;

        auto loc_txt = (loc_check) ? "True" : "False";
        auto val_txt = (val_check) ? "True" : "False";


        std::cout   << "/----------------------------------------\\" << std::endl
                    << "| Tests results, with precision " << epsilon << std::endl << std::endl;

        std::cout   << "| --- Value ---"                                        << std::endl
                    << "| Expected value   : "  << expected_val                 << std::endl
                    << "| Got value        : "  << val                          << std::endl
                    << "| Value diff       : "  << abs(expected_val-val)        << std::endl
                    << "| Value check      : "  << val_txt                      << std::endl << std::endl;


        std::cout   << "| --- Location ---"                                     << std::endl
                    << "| Expected location   : "   << expected_loc             << std::endl
                    << "| Got location        : "   << loc                      << std::endl
                    << "| Location diff       : "   << abs(expected_loc-loc)    << std::endl
                    << "| Location check      : "   << loc_txt                  << std::endl << std::endl;
    }

    /**
     * @brief Function to test an Interval Global minimization test case
     * 
     * @tparam T        : Scalar type (either float, double or long)
     * @tparam Policies : A Policies for rounding and checking, from boost::numeric::interval
     * @param t         : The test case structure
     * @return true     : Test successful, minimum is at the correct location and have the correct value
     * @return false    : Test failure
     */
    template<typename T, class Policies>
    bool test_interval_with_derivatives(const struct test_case<T,Policies> &t)
    {
        assert(t.f_d.has_value()  );
        assert(t.F_d.has_value()  );
        assert(t.f_dd.has_value() );
        assert(t.F_dd.has_value() );

        T min_res = IntervalSolver::minimize_interval_method<T,Policies>(
            t.init_left,t.init_right,
            t.f,t.F,
            t.f_d.value(),t.F_d.value(),
            t.f_dd.value(),t.F_dd.value(),
            t.precision,t.lower_bound
        );

        print_results<T>(t.location,t.value,min_res,t.f(min_res),t.precision);

        return (abs(min_res - t.location) < t.precision) && (abs(t.f(min_res) - t.value) < t.precision);
    }

    /**
     * @brief Function to test an Interval Global minimization test case, but with the derivatives-free method
     * 
     * @tparam T        : Scalar type (either float, double or long)
     * @tparam Policies : A Policies for rounding and checking, from boost::numeric::interval
     * @param t         : The test case structure
     * @return true     : Test successful, minimum is at the correct location and have the correct value
     * @return false    : Test failure
     */
    template<typename T, class Policies>
    bool test_interval_no_derivatives(const struct test_case<T,Policies> &t)
    {
        T min_res = IntervalSolver::minimize_interval_method<T,Policies>(
            t.init_left,t.init_right,
            t.f,t.F,
            t.precision,t.lower_bound
        );

        print_results<T>(t.location,t.value,min_res,t.f(min_res),t.precision);

        return (abs(min_res - t.location) < t.precision) && (abs(t.f(min_res) - t.value) < t.precision);
    }

}
