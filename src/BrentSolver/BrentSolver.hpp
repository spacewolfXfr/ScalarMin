// Copyright (c) 2025 Mael FEURGARD <mael.feurgard@enac.fr>
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT
#pragma once

#include <math.h>
#include <functional>
#include <tuple>

#if DEBUG_STDOUT >= 1
#include <iostream>
#endif

namespace BrentSolver
{

    /**
     * @brief Naively sample a function to estimate its minimum
     * 
     * @tparam T : Scalar type (either float, double or long)
     * @param left          : Left side of the initial search interval
     * @param right         : Right side of the initial search interval
     * @param f             : Function to minimize
     * @param samples       : Number of regularly spaced samples between left and right. Must be at least 2 (to sample left and right)
     * @return std::pair<T,T> : (Minimum location, Minimum value) computed among the samples
     */
    template<typename T>
    std::pair<T,T> sample_minimize(T left, T right, 
        std::function<T(const T&)> f, int samples)
    {
        assert(samples > 1);
        assert(right > left);

        T min_loc = left;
        T min_val = f(left);

        T delta = (right-left)/static_cast<T>(samples-1);
        for(int i = 1; i < samples; i++)
        {
            T loc = left + i*delta;
            T val = f(loc);

            if (val < min_val)
            {
                min_val = val;
                min_loc = loc;
            }
        }

        return std::pair<T,T>(min_loc,min_val);
    }

    /**
     * @brief Minimize a function using a Brent global optimization method
     * 
     * Cf Chapter 6, Section 3 of "Algorithms for Minimization without Derivatives" by Brent, 1973
     * 
     * @tparam T : Scalar type (either float, double or long)
     * @param left          : Left side of the initial search interval
     * @param right         : Right side of the initial search interval
     * @param f             : Function to minimize
     * @param M             : Upper bound of f second derivative over the interval [left,right]
     * @param epsilon       : Desired precision for result location
     * @param bootstrap     : If positive, bootstrap the minimum search by performing regular sampling. 
     *                        The number of samples is equal to bootstrap+1
     * @return T : Location of the minimum
     */
    template<typename T>
    T minimize_brent(T left, T right, 
        std::function<T(const T&)> f,
        T M, T epsilon, int bootstrap=0
    )
    {
        assert(right > left);

        T best_loc,best;
        if (bootstrap > 0)
        {
            std::pair<T,T> p = sample_minimize(left,right,f,bootstrap+1);
            best_loc    = p.first;
            best        = p.second;
        }
        else
        {
            best_loc    = left;
            best        = f(left);

            if (f(right) < best)
            {
                best_loc = right;
                best = f(right);
            }
        }

        // For Concave function, solution is immediate
        if (M <= 0)
        {
            return best_loc;
        }

        T a2 = left;
        T A2 = f(a2);

        // Pick a3 in (a2,b]
        T a3 = a2 + sqrt((A2 - best + epsilon)/(0.5*M));
        while(a2 < right)
        {

#if DEBUG_STDOUT >= 1
            std::cout   << "a2: " << a2 << std::endl
                        << "a3: " << a3 << std::endl
                        << "best_loc : " << best_loc << std::endl
                        << "best_val : " << best     << std::endl << std::endl;
#endif


            T A3 = f(a3);
                        
            if (A3 < best)
            {
                best = A3;
                best_loc = a3;
            }

            // Parabolla interpolation: P(x) = (M/2)*x² + b x + c
            // With P(a2) = f(a2) and P(a3) = f(a3)
            
            

            T b = (A3-A2)/(a3-a2) - (M/2)*(a3+a2);
            T c = A2 - b*a2 - (M/2)*a2*a2;

            T P_x_min   = -b/M;
            T P_min     = (M/2)*P_x_min*P_x_min + b*P_x_min + c;
            
            // Check if parabolla is above best estimate on [a2,a3]
            bool P_check;
            if (a2 <= P_x_min && P_x_min <= a3)
            {
                P_check = (P_min > best-epsilon);
            }
            else if (P_x_min < a2) // P is increasing on [a2,a3]
            {
                P_check = (A2 > best-epsilon);
            }
            else // (P_x_min > a3) // P is decreasing on [a2,a3]
            {
                P_check = (A3 > best-epsilon);
            }


            if (P_check)
            {
                a2 = a3;
                a3 = a2 + sqrt((A2 - best + epsilon)/(0.5*M));
                A2 = f(a2);
            }
            else
            {
                a3 = (a2+a3)/2;
            }

        }

        return best_loc;
    }

    /**
     * @brief Minimize a function using a modification of Brent global optimization method, using Taylor-Lagrange approximation 
     * 
     * Cf Chapter 6, Section 3 of "Algorithms for Minimization without Derivatives" by Brent, 1973
     * The modification relies an trying to extend the stepping length by exploiting first order Taylor-Lagrange approximation
     * on the moving edge of the interval. The approximation yields a degree 2 polynomial lower-bounding the function f.
     * 
     * @tparam T : Scalar type (either float, double or long)
     * @param left          : Left side of the initial search interval
     * @param right         : Right side of the initial search interval
     * @param f             : Function to minimize
     * @param f_d           : f first derivative
     * @param M             : ABSOLUTE bound of f second derivative over the interval [left,right] (both lower and upper bound)
     * @param epsilon       : Desired precision for result location
     * @param bootstrap     : If positive, bootstrap the minimum search by performing regular sampling. 
     *                        The number of samples is equal to bootstrap+1
     * @return T : Location of the minimum
     */
    template<typename T>
    T minimize_optbrent(T left, T right, 
        std::function<T(const T&)> f,
        std::function<T(const T&)> f_d,
        T M, T epsilon, int bootstrap=0
    )
    {
        assert(right > left);
        
        T best_loc,best;
        if (bootstrap > 0)
        {
            std::pair<T,T> p = sample_minimize(left,right,f,bootstrap+1);
            best_loc    = p.first;
            best        = p.second;
        }
        else
        {
            best_loc    = left;
            best        = f(left);

            if (f(right) < best)
            {
                best_loc = right;
                best = f(right);
            }
        }

        // For Concave function, solution is immediate
        if (M <= 0)
        {
            return best_loc;
        }

        T a2    = left;
        T A2    = f(a2);
        T A2_d  = f_d(a2);

        // Pick a3 in (a2,b]
        T a3    = a2 + sqrt((A2 - best + epsilon)/(0.5*M));
        while(a2 < right)
        {

#if DEBUG_STDOUT >= 1
            std::cout   << "a2: " << a2 << std::endl
                        << "a3: " << a3 << std::endl
                        << "best_loc : " << best_loc << std::endl
                        << "best_val : " << best     << std::endl << std::endl;
#endif
            // Shrink on left side using Taylor approximation
            T Delta_a = A2_d*A2_d + 2*M*(A2-best);
            T step_a = (A2_d + std::sqrt(Delta_a))/M;
            step_a = std::max(step_a,epsilon);

            // Compute the Brent interpolation point (and improve minimum estimation)
            T A3 = f(a3);
                        
            if (A3 < best)
            {
                best = A3;
                best_loc = a3;
            }

            // Parabolla interpolation: P(x) = (M/2)*x² + b x + c
            // With P(a2) = f(a2) and P(a3) = f(a3)

            T b = (A3-A2)/(a3-a2) - (M/2)*(a3+a2);
            T c = A2 - b*a2 - (M/2)*a2*a2;

            T P_x_min   = -b/M;
            T P_min     = (M/2)*P_x_min*P_x_min + b*P_x_min + c;
            
            // Check if parabolla is above best estimate on [a2,a3]
            bool P_check;
            if (a2 <= P_x_min && P_x_min <= a3)
            {
                P_check = (P_min > best-epsilon);
            }
            else if (P_x_min < a2) // P is increasing on [a2,a3]
            {
                P_check = (A2 > best-epsilon);
            }
            else // (P_x_min > a3) // P is decreasing on [a2,a3]
            {
                P_check = (A3 > best-epsilon);
            }

            T step_a3 = sqrt((A2 - best + epsilon)/(0.5*M));
            T step_abrent;
            if (P_check)
            {
                step_abrent = a3-a2;
                a2 += std::max(step_a,step_abrent);
                a3 = a2 + step_a3;
                A2 = f(a2);
                A2_d = f_d(a2);
            }
            else
            {
                step_abrent = (a3-a2)/2;
                if (step_abrent > step_a)
                {
                    a3 = (a2+a3)/2;
                }
                else
                {
                    a2 += step_a;
                    a3 = a2+step_a3;
                    A2 = f(a2);
                    A2_d = f_d(a2);
                }
            }
#if DEBUG_STDOUT >= 1
            std::cout   << "Taylor step: " << step_a << std::endl
                        << "Brent  step: " << step_abrent << std::endl << std::endl;
#endif

        }

        return best_loc;
    }
};