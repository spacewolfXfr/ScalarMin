# CMake general configuration
cmake_minimum_required(VERSION 3.28.0)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_DEBUG_POSTFIX d)

# set(CMAKE_VERBOSE_MAKEFILE ON)

# -------------------- Project declaration -------------------- #
project(ScalarMin VERSION 0.1.0 LANGUAGES C CXX)

# -------------------- Required packages -------------------- #
set(Boost_USE_STATIC_LIBS OFF) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 

find_package(Boost CONFIG REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})

# -------------------- Targets -------------------- #

add_subdirectory(ScalarMin)


# # If we need accurate rounding: https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html#index-frounding-math
# target_compile_options(ScalarMin PRIVATE -frounding-math)
# target_link_options(ScalarMin PRIVATE -frounding-math)

# -------------------- Compiler options -------------------- #

set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")

# -------------------- Testing -------------------- #

option(SCALARMIN_VISUALISATION "Include visualisation in the solver (for debug/demo purpose)" OFF)

if(SCALARMIN_VISUALISATON)
    target_compile_definitions(ScalarMinTest PUBLIC DEBUG_VISUALISE 1)
    target_compile_definitions(ScalarMinTest PUBLIC DEBUG_VISUALISE_SAMPLES 200)
endif()

option(SCALARMIN_TESTING "Include test, using GoogleTest" OFF) # https://google.github.io/googletest/quickstart-cmake.html

if(SCALARMIN_TESTING)
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        52eb8108c5bdec04579160ae17225d66034bd723 # release-1.17.0
        )

    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
    
    include(CTest)
    enable_testing()
    add_subdirectory(test)
endif()

# if(BENCHMARK)
#     include(FetchContent)
#     FetchContent_Declare(
#         googlebench
#         GIT_REPOSITORY https://github.com/google/benchmark.git
#         GIT_TAG        eddb0241389718a23a42db6af5f0164b6e0139af # release-1.9.4
#         )
# endif()
