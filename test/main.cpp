// Copyright (c) 2025 Mael FEURGARD <mael.feurgard@enac.fr>
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

// #include "ProjectHeader.h"
#include "testFun.hpp"

#include <iostream>

int main(int, char**){
    std::cout << "Hello, from ScalarMin version ";// << ScalarMin_VERSION_STR << std::endl;

    // assert(IntervalTesting::test_interval_with_derivatives(test1));
    assert(IntervalTesting::test_interval_no_derivatives(test1));
    // assert(IntervalTesting::test_interval_with_derivatives(test2));
    assert(IntervalTesting::test_interval_no_derivatives(test2));
}