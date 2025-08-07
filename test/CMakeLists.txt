# Copyright (c) 2025 Mael FEURGARD <mael.feurgard@enac.fr>
# 
# This software is released under the MIT License.
# https://opensource.org/licenses/MIT


#################### Test folder ####################

include(GoogleTest)

macro(make_ScalarMin_test testName testFile)
    add_executable(${testName} ${testFile})
    target_link_libraries(${testName} PUBLIC IntervalSolver BrentSolver GTest::gtest_main)

    target_compile_options(${testName} PUBLIC
        "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Wall;-Wextra;-Wshadow;-Wformat=2;-Wunused>>"
        "$<${msvc_cxx}:$<BUILD_INTERFACE:-W3>>"
        )

    target_include_directories(${testName} PUBLIC 
                            "${PROJECT_SOURCE_DIR}"
                            "${PROJECT_SOURCE_DIR}/ScalarMin"
                            )

    gtest_discover_tests(${testName})
endmacro()


########## Tests on polynomial functions 

make_ScalarMin_test(ScalarMinTestPolynomials PolynomialTesting.cpp)

########## Tests on trigonometric functions 

make_ScalarMin_test(ScalarMinTestCircles CirclesTesting.cpp)
make_ScalarMin_test(ScalarMinTestLineCircle LineCircleTesting.cpp)