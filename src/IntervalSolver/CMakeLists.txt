# Copyright (c) 2025 Mael FEURGARD <mael.feurgard@enac.fr>
# 
# This software is released under the MIT License.
# https://opensource.org/licenses/MIT

find_package(Boost REQUIRED)

add_library(IntervalSolver INTERFACE)
target_sources(IntervalSolver INTERFACE IntervalSolver.hpp)

target_link_libraries(IntervalSolver INTERFACE Boost::headers)
