# Copyright (c) 2025 Mael FEURGARD <mael.feurgard@enac.fr>
# 
# This software is released under the MIT License.
# https://opensource.org/licenses/MIT

configure_file(ProjectHeader.h.in ProjectHeader.h)

# add_subdirectory(BrentSolver) #TODO : Write the Brent global solver
add_subdirectory(IntervalSolver)
