#ifndef FL_DERIVATIVE_TESTS_H
#define FL_DERIVATIVE_TESTS_H

#include <iostream>
#include <string>
#include <vector>
#include "regexpr.h"


struct string_case{
    std::string str;
    bool answer;
};


struct tests {
    std::string name;
    std::vector<string_case> cases;
    std::shared_ptr<regexpr> re;
    std::string str;
    int passed = 0;
    void start();
};


#endif //FL_DERIVATIVE_TESTS_H
