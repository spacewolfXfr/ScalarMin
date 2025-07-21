// Copyright (c) 2025 Mael FEURGARD <mael.feurgard@enac.fr>
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#pragma once

#include <sciplot/sciplot.hpp>
#include <boost/numeric/interval.hpp>

#include <list>
#include <valarray>

namespace Visualisation
{
    using namespace sciplot;
    
    template<typename T, class Policies>
    using interval = boost::numeric::interval<T,Policies>;

    template<typename T>
    Plot2D init_plot(T xmin, T xmax)
    {
        // Create a Plot object
        Plot2D plot;

        // Set the x and y labels
        plot.xlabel("x");
        plot.ylabel("y");

        // Set x and y ranges
        plot.xrange(xmin,xmax);
        plot.yrange("","*"); // Autorange

        // Set the legend to be on the bottom along the horizontal
        plot.legend()
            .atOutsideBottom()
            .displayHorizontal();

        return plot;
    }

    template<typename T>
    auto plot_function(T left, T right, unsigned samples,
        std::function<T(const T&)> f, Plot2D &plot)
    {
        std::valarray<T> xs = linspace<T,T,T>(left,right, samples);
        std::valarray<T> ys(xs.size());

        for(size_t i = 0; i < xs.size() ; i++)
        {
            ys[i] = f(xs[i]);
        }

        // Plot the curve
        return plot.drawCurve(xs,ys);
    }

    template<typename T>
    auto plot_hline(T left, T right, T val, Plot2D &plot)
    {
        std::valarray<T> xs({left,right});
        std::valarray<T> ys({val,val});

        // Plot the line as a curve with two points
        return plot.drawCurve(xs,ys);
    }

    template<typename T>
    auto plot_interval(T xleft, T xright, T ylow, T yhigh, Plot2D &plot)
    {
        T xmid = (xleft+xright)/static_cast<T>(2);
        T ymid = (ylow+yhigh)/static_cast<T>(2);

        return plot.drawErrorBarsXY(xmid,ymid,xleft,xright,ylow,yhigh);
    }

    template<typename T, class Policies>
    auto plot_intervals(std::list<interval<T,Policies>> intervals_list,std::function<interval<T, Policies>(const interval<T, Policies>&)> F, Plot2D &plot)
    {
        size_t int_num = intervals_list.size();

        std::valarray<T> xs_left(int_num),xs_right(int_num),xs_middle(int_num);
        std::valarray<T> ys_left(int_num),ys_right(int_num),ys_middle(int_num);

        size_t i = 0;
        for (const interval<T,Policies> &I : intervals_list)
        {
            xs_left[i]      = I.lower();
            xs_right[i]     = I.upper();
            xs_middle[i]    = (I.lower()+I.upper())/static_cast<T>(2);

            interval<T,Policies> f_I = F(I);

            ys_left[i]      = f_I.lower();
            ys_right[i]     = f_I.upper();
            ys_middle[i]    = (f_I.lower()+f_I.upper())/static_cast<T>(2);

            i++;
        }

        return plot.drawErrorBarsXY(xs_middle,ys_middle,xs_left,xs_right,ys_left,ys_right);

    }

}