#ifndef FL3_MY_TESTS_H
#define FL3_MY_TESTS_H
#include <string>
#include <vector>
#include "my_regexp.h"
struct case_{
    std::string str;
    bool expected;
};
struct tests{
    std::vector <case_> cases = {};
    regexp regexp_;
    std::string regexp_str;
    int passed = 0;
    int failed = 0;
    int number_of_test;
    void launch();
    tests();
};
#endif  // FL3_MY_TESTS_H
