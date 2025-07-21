# Copyright (c) 2025 Mael FEURGARD <mael.feurgard@enac.fr>
# 
# This software is released under the MIT License.
# https://opensource.org/licenses/MIT

find_package(sciplot REQUIRED)
find_package(Boost REQUIRED)

add_library(Visualisation INTERFACE)
target_sources(Visualisation INTERFACE Visualisation.hpp)

target_link_libraries(Visualisation INTERFACE sciplot::sciplot Boost::headers)
