# CMake general configuration
cmake_minimum_required(VERSION 3.30.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_DEBUG_POSTFIX d)

set(CMAKE_VERBOSE_MAKEFILE ON)

# -------------------- Project declaration -------------------- #
project(ScalarMin VERSION 0.1.0 LANGUAGES C CXX)
configure_file(ProjectHeader.h.in ProjectHeader.h)

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


# -------------------- Testing -------------------- #
include(CTest)
enable_testing()
add_test(NAME ScalarMinTest COMMAND test)

# function(do_test target arg result)
#   add_test(NAME Comp${arg} COMMAND ${target} ${arg})
#   set_tests_properties(Comp${arg}
#     PROPERTIES PASS_REGULAR_EXPRESSION ${result}
#     )
# endfunction()
