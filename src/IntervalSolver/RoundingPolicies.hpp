// Copyright (c) 2025 Mael FEURGARD <mael.feurgard@enac.fr>
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#pragma once

#include <boost/numeric/interval.hpp>

namespace OptimizationRounding
{
    using namespace boost::numeric::interval_lib;
    // We need a specific rounding policy to handle cos and sin: https://stackoverflow.com/questions/15860761/c-boost-interval-and-cos
    template<typename T>
    using standard_rounding = policies<save_state<rounded_transc_std<T> >,
                    checking_base<T>>;

    template<typename T>
    using fast_rounding = policies<save_state<rounded_transc_exact<T> >,
                    checking_base<T>>;
}

