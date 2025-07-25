// Copyright (c) 2025 Mael FEURGARD <mael.feurgard@enac.fr>
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#pragma once

#include <boost/math/tools/polynomial.hpp>
#include "testInterface.hpp"

#include <vector>
#include <random>
#include <initializer_list>
#include <iostream>

template<typename T, class Policies>
class Polynomial : public MinimizeTesting::TestCaseWithDerivatives<T,Policies>
{
protected:
    const std::vector<T> coefs;

public:
    void print_params()
    {
        std::cout << "P(x) = " << std::setprecision(5);
        size_t i = 0;
        for (T c : coefs)
        {
            if (i > 0)
            {
                std::cout << " + " << c << " x^" << i;
            }
            else
            {
                std::cout << c;
            }
            
            i++;
        }
        std::cout << std::endl << std::endl;
    }

    size_t degree() const {return coefs.size()-1;}

    Polynomial(MinimizeTesting::TestValues<T> vals, std::vector<T> _coefs)
    : MinimizeTesting::TestCaseWithDerivatives<T,Policies>(vals), coefs(_coefs) 
    {
        assert(_coefs.size() > 0);
    }

    Polynomial(MinimizeTesting::TestValues<T> vals, std::initializer_list<T> _coefs)
    : MinimizeTesting::TestCaseWithDerivatives<T,Policies>(vals), coefs(_coefs) 
    {
        assert(_coefs.size() > 0);
    }


    pure T f(T t) const
    {
        T output = static_cast<T>(0);
        for(size_t i = 0 ; i <= degree() ; i++)
        {
            output = output*t+coefs[degree() - i];
        }
        return output;
    }

    pure MinimizeTesting::interval<T,Policies> F(const MinimizeTesting::interval<T,Policies> &I) const
    {
        MinimizeTesting::interval<T,Policies> output(static_cast<T>(0));
        for(size_t i = 0 ; i <= degree() ; i++)
        {
            output = output*I+coefs[degree() - i];
        }
        return output;
    }

    pure T f_d(T t) const
    {
        T output = static_cast<T>(0);
        for(size_t i = 0 ; i <= degree()-1 ; i++)
        {
            size_t k = degree()-i;
            output = output*t+k*coefs[k];
        }
        return output;
    }

    pure MinimizeTesting::interval<T,Policies> F_d(const MinimizeTesting::interval<T,Policies> &I) const
    {
        MinimizeTesting::interval<T,Policies> output(static_cast<T>(0));
        for(size_t i = 0 ; i <= degree()-1 ; i++)
        {
            size_t k = degree()-i;
            output = output*I+k*coefs[k];
        }
        return output;
    }

    pure T f_dd(T t) const
    {
        T output = static_cast<T>(0);
        for(size_t i = 0 ; i <= degree()-2 ; i++)
        {
            size_t k = degree()-i;
            output = output*t+k*(k-1)*coefs[k];
        }
        return output;
    }

    pure MinimizeTesting::interval<T,Policies> F_dd(const MinimizeTesting::interval<T,Policies> &I) const
    {
        MinimizeTesting::interval<T,Policies> output(static_cast<T>(0));
        for(size_t i = 0 ; i <= degree()-2 ; i++)
        {
            size_t k = degree()-i;
            output = output*I+k*(k-1)*coefs[k];
        }
        return output;
    }

};

template<typename T, class Policies>
Polynomial<T,Policies> generate_polynomial_test_case(int seed, size_t degree, T r_min, T r_max, T I_min, T I_max, T precision)
{
    assert(r_min < r_max);
    assert(I_min < I_max);
    assert(degree > 1);

    std::default_random_engine gen(seed); // Some seeded RNG 
    std::uniform_real_distribution<T> dis(r_min, r_max);
    
    typedef boost::math::tools::polynomial<T> poly_t;

    MinimizeTesting::TestValues<T> vals;
    vals.init_left  = I_min;
    vals.init_right = I_max;
    vals.precision  = precision;

    std::vector<T> first_root;
    poly_t first_de{{static_cast<T>(1)}};

    for(size_t i = 0 ; i < degree-1 ; i++)
    {
        T root = dis(gen);
        first_root.push_back(root);



        poly_t root_poly = poly_t({-root,static_cast<T>(1)});
        assert(abs(root_poly(root)) < precision);
        first_de *= root_poly;

        // std::cout   << "Root number n° " << i << std::endl
        //             << "Root location: " << root << std::endl
        //             << "Root value   : " << first_de(root) << std::endl << std::endl;

        assert(abs(first_de(root)) < precision);
    }

    for(size_t i = 0 ; i < first_root.size() ; i++)
    {
        // std::cout   << "Root number n° " << i << std::endl
        //             << "Root location: " << first_root[i] << std::endl
        //             << "Root value   : " << first_de(first_root[i]) << std::endl << std::endl;

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

    return Polynomial<T,Policies>(vals,base_poly.data());
}