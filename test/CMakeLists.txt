# Copyright (c) 2025 Mael FEURGARD <mael.feurgard@enac.fr>
# 
# This software is released under the MIT License.
# https://opensource.org/licenses/MIT

add_executable(ScalarMinTest main.cpp testFun.cpp)

target_link_libraries(ScalarMinTest PUBLIC IntervalSolver)

target_include_directories(ScalarMinTest PUBLIC 
                            "${PROJECT_SOURCE_DIR}"
                            "${PROJECT_SOURCE_DIR}/src/IntervalSolver"
                            "${PROJECT_SOURCE_DIR}/src/BrentSolver"
                            )