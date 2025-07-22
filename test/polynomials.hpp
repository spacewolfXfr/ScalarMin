// Copyright (c) 2025 Mael FEURGARD <mael.feurgard@enac.fr>
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#pragma once

#include <boost/math/tools/polynomial.hpp>
#include "testInterface.hpp"

#include <array>
#include <random>
#include <initializer_list>

template<typename T, class Policies, unsigned short N>
class Polynomial : public IntervalTesting::TestCaseWithDerivatives<T,Policies>
{
protected:
    const std::array<T,N+1> coefs;

public:
    static unsigned short degree() {return N;}

    Polynomial(IntervalTesting::TestValues<T> vals, std::array<T,N+1> _coefs)
    : IntervalTesting::TestCaseWithDerivatives<T,Policies>(vals), coefs(_coefs) {}

    pure T f(T t) const
    {
        T output = coefs[N];
        for(int i = N-1 ; i >= 0 ; i--)
        {
            output = output*t+coefs[i];
        }
        return output;
    }

    pure IntervalTesting::interval<T,Policies> F(const IntervalTesting::interval<T,Policies> &I) const
    {
        IntervalTesting::interval<T,Policies> output(coefs[N]);
        for(int i = N-1 ; i >= 0 ; i--)
        {
            output = output*I+coefs[i];
        }
        return output;
    }

    pure T f_d(T t) const
    {
        T output = N*coefs[N];
        for(auto i = N-1 ; i >= 1 ; i--)
        {
            output = output*t+i*coefs[i];
        }
        return output;
    }

    pure IntervalTesting::interval<T,Policies> F_d(const IntervalTesting::interval<T,Policies> &I) const
    {
        IntervalTesting::interval<T,Policies> output(N*coefs[N]);
        for(auto i = N-1 ; i >= 1 ; i--)
        {
            output = output*I+i*coefs[i];
        }
        return output;
    }

    pure T f_dd(T t) const
    {
        T output = N*(N-1)*coefs[N];
        for(auto i = N-1 ; i >= 2 ; i--)
        {
            output = output*t+i*(i-1)*coefs[i];
        }
        return output;
    }

    pure IntervalTesting::interval<T,Policies> F_dd(const IntervalTesting::interval<T,Policies> &I) const
    {
        IntervalTesting::interval<T,Policies> output(N*(N-1)*coefs[N]);
        for(auto i = N-1 ; i >= 2 ; i--)
        {
            output = output*I+i*(i-1)*coefs[i];
        }
        return output;
    }

};

template<typename T, class Policies, unsigned short N>
Polynomial<T,Policies,N> generate_polynomial_test_case(int seed, T r_min, T r_max, T I_min, T I_max, T precision)
{
    assert(r_min < r_max);
    assert(I_min < I_max);

    std::default_random_engine gen(seed); // Some seeded RNG 
    std::uniform_real_distribution<T> dis(r_min, r_max);
    
    typedef boost::math::tools::polynomial<T> poly_t;

    IntervalTesting::TestValues<T> vals;
    vals.init_left  = I_min;
    vals.init_right = I_max;
    vals.precision  = precision;

    std::array<T,N-1> first_root;
    poly_t first_de{{1.}};

    for(size_t i = 0 ; i < first_root.size() ; i++)
    {
        first_root[i] = dis(gen);
        first_de *= poly_t({-first_root[i],static_cast<T>(1)});
    }

    for(size_t i = 0 ; i < first_root.size() ; i++)
    {
        assert(abs(first_de(first_root[i])) < precision);
    }
    poly_t base_poly    = first_de.integrate() + poly_t({dis(gen)});

    vals.location   = I_min;
    vals.value      = base_poly(I_min);

    if (base_poly(I_max) < vals.value)
    {
        vals.value      = base_poly(I_max);
        vals.location   = I_max;
    }

    for(size_t i = 0 ; i < first_root.size() ; i++)
    {
        if (first_root[i] > I_max || first_root[i] < I_min)
        {
            continue;
        }
        
        if (base_poly(first_root[i]) < vals.value)
        {
            vals.value      = base_poly(first_root[i]);
            vals.location   = first_root[i];
        }
    }

    std::array<T,N+1> coefs;
    for(size_t i = 0 ; i < coefs.size() ; i++)
    {
        coefs[i] = base_poly.data()[i];
    }

    return Polynomial<T,Policies,N>(vals,coefs);
}