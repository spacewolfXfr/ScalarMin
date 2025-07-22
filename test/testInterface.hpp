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

    template<typename T>
    struct TestValues
    {
        T location;     /// Where the minimum is located
        T value;        /// The value of f at the minimum
        T init_left;    /// Lower bound of the initial research interval
        T init_right;   /// Upper bound of the initial research interval
        T precision;    /// Precision desired for the minimum location and value
        std::optional<T> lower_bound = std::nullopt;    /// A known lower-bound on the global minimum

    };

    template<typename T, class Policies>
    class TestCase
    {
    public:
        TestCase(struct TestValues<T> _testVals) : testVals(_testVals) {}

        struct TestValues<T> testVals;
        

        void print_results(T loc)
        {
            T expected_loc  = testVals.location;
            T expected_val  = testVals.value;
            T val           = f(loc);
            T epsilon       = testVals.precision;

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


        virtual T f(T) const = 0;                                               /// The function the minimum is to be found
        virtual interval<T,Policies> F(const interval<T,Policies>&) const = 0;  /// f translation in interval algebra

        T minimize_interval_method(void)
        {
            auto fbind = [this](T x){return this->f(x);};
            auto Fbind = [this](const interval<T,Policies> &I){return this->F(I);};

            return IntervalSolver::minimize_interval_method<T,Policies>(
                testVals.init_left,testVals.init_right,
                fbind,Fbind,
                testVals.precision,testVals.lower_bound
            );
        }
    };

    template<typename T, class Policies>
    class TestCaseWithDerivatives : public TestCase<T,Policies>
    {
    public:
        TestCaseWithDerivatives(struct TestValues<T> _testVals) : TestCase<T,Policies>(_testVals) {}

        virtual T f_d(T) const = 0;                                                 /// f First derivative
        virtual interval<T,Policies> F_d(const interval<T,Policies>&) const = 0;    /// f_d translation in the interval algebra
        virtual T f_dd(T) const = 0;                                                /// f Seconde derivative
        virtual interval<T,Policies> F_dd(const interval<T,Policies>&) const = 0;   /// f_dd translation in the interval algebra

        T minimize_interval_method_with_derivatives(void)
        {
            auto fbind = [this](T x){return this->f(x);};
            auto Fbind = [this](const interval<T,Policies> &I){return this->F(I);};

            auto f_dbind = [this](T x){return this->f_d(x);};
            auto F_dbind = [this](const interval<T,Policies> &I){return this->F_d(I);};

            auto f_ddbind = [this](T x){return this->f_dd(x);};
            auto F_ddbind = [this](const interval<T,Policies> &I){return this->F_dd(I);};

            return IntervalSolver::minimize_interval_method<T,Policies>(
                this->testVals.init_left,this->testVals.init_right,
                fbind,Fbind,
                f_dbind,F_dbind,
                f_ddbind,F_ddbind,
                this->testVals.precision,this->testVals.lower_bound
            );
        }
    };
}
