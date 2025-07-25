# Copyright (c) 2025 Mael FEURGARD <mael.feurgard@enac.fr>
# 
# This software is released under the MIT License.
# https://opensource.org/licenses/MIT


add_executable(ScalarMinTest testing.cpp)

target_link_libraries(ScalarMinTest PUBLIC IntervalSolver BrentSolver GTest::gtest_main)

target_compile_options(ScalarMinTest PUBLIC
"$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Wall;-Wextra;-Wshadow;-Wformat=2;-Wunused>>"
"$<${msvc_cxx}:$<BUILD_INTERFACE:-W3>>"
)

target_include_directories(ScalarMinTest PUBLIC 
                            "${PROJECT_SOURCE_DIR}"
                            "${PROJECT_SOURCE_DIR}/src"
                            "${PROJECT_SOURCE_DIR}/src/IntervalSolver"
                            "${PROJECT_SOURCE_DIR}/src/BrentSolver"
                            )

include(GoogleTest)
gtest_discover_tests(ScalarMinTest)