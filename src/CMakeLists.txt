# Copyright (c) 2025 Mael FEURGARD <mael.feurgard@enac.fr>
# 
# This software is released under the MIT License.
# https://opensource.org/licenses/MIT

#################### General configuration #################### 

configure_file(ProjectHeader.h.in ProjectHeader.h)


#################### Brent Solver ####################

add_library(BrentSolver INTERFACE)
target_sources(BrentSolver INTERFACE BrentSolver.hpp)

#################### Interval Solver ####################

find_package(Boost REQUIRED)

add_library(IntervalSolver INTERFACE)
target_sources(IntervalSolver INTERFACE IntervalSolver.hpp RoundingPolicies.hpp)

target_link_libraries(IntervalSolver INTERFACE Boost::headers)
