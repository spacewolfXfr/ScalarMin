# CMake general configuration
cmake_minimum_required(VERSION 3.30.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_DEBUG_POSTFIX d)

# set(CMAKE_VERBOSE_MAKEFILE ON)

# -------------------- Project declaration -------------------- #
project(ScalarMin VERSION 0.1.0 LANGUAGES C CXX)

# -------------------- Required packages -------------------- #
set(Boost_USE_STATIC_LIBS OFF) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 

find_package(Boost REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})

# -------------------- Targets -------------------- #

add_executable(ScalarMin main.cpp)
add_subdirectory(src)
add_subdirectory(test)

target_link_libraries(ScalarMin PUBLIC Boost::headers)

# # If we need accurate rounding: https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html#index-frounding-math
# target_compile_options(ScalarMin PRIVATE -frounding-math)
# target_link_options(ScalarMin PRIVATE -frounding-math)

# -------------------- Compiler options -------------------- #

set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")

target_compile_options(ScalarMin PUBLIC
"$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Wall;-Wextra;-Wshadow;-Wformat=2;-Wunused>>"
"$<${msvc_cxx}:$<BUILD_INTERFACE:-W3>>"
)

target_compile_options(ScalarMinTest PUBLIC
"$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Wall;-Wextra;-Wshadow;-Wformat=2;-Wunused>>"
"$<${msvc_cxx}:$<BUILD_INTERFACE:-W3>>"
)

# -------------------- Testing -------------------- #

option(VISUALISATION "Include visualisation in the solver (for debug/demo purpose)" OFF)

if(VISUALISATON)
    target_compile_definitions(ScalarMinTest PUBLIC DEBUG_VISUALISE 1)
    target_compile_definitions(ScalarMinTest PUBLIC DEBUG_VISUALISE_SAMPLES 200)
endif()

option(TESTING "Include test, using GoogleTest" ON) # https://google.github.io/googletest/quickstart-cmake.html

if(TESTING)
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        52eb8108c5bdec04579160ae17225d66034bd723 # release-1.17.0
        )

endif()

if(BENCHMARK)
    include(FetchContent)
    FetchContent_Declare(
        googlebench
        GIT_REPOSITORY https://github.com/google/benchmark.git
        GIT_TAG        eddb0241389718a23a42db6af5f0164b6e0139af # release-1.9.4
        )
endif()
## Testing using CTest... Why not, but may be lacking 

# include(CTest)
# enable_testing()
# add_test(NAME ScalarMinTest COMMAND test)


# function(do_test target arg result)
#   add_test(NAME Comp${arg} COMMAND ${target} ${arg})
#   set_tests_properties(Comp${arg}
#     PROPERTIES PASS_REGULAR_EXPRESSION ${result}
#     )
# endfunction()
