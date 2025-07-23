// Copyright (c) 2025 Mael FEURGARD <mael.feurgard@enac.fr>
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <functional>


#pragma once

namespace BrentSolver
{
    template<typename T>
    T minimize_brent(T left, T right, 
        std::function<T(const T&)> f,
        T epsilon,
        T M
    )
    {
        T best_loc  = left;
        T best      = f(left);
        
        if (f(right) < best)
        {
            best_loc = right;
            best = f(right);
        }

        // For Concave function, solution is immediate
        if (M <= 0)
        {
            return best_loc;
        }

        T a2 = left;
        while(a2 < right)
        {
            // Pick a3 in (a2,b]
            // TODO: Implement smart pick
            T a3 = (a2 + right)/2;
            
            if (f(a3) < best)
            {
                best = f(a3);
                best_loc = a3;
            }

            // Parabolla interpolation: P(x) = (M/2)*x² + b x + c
            // With P(a2) = f(a2) and P(a3) = f(a3)
            
            T A3 = f(a3);
            T A2 = f(a2);

            T b = (A3-A2)/(a3-a2) - (M/2)*(a3+a2);
            T c = A2 - b*a2 - (M/2)*a2*a2;



        }

        return best_loc;

    }
};