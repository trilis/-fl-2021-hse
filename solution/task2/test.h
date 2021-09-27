#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include "regexp.h"

namespace Tests {
    struct test {
        std::string expression;
        bool expected_resp;
    };

    class tester {
        std::string regexp_text;
        std::shared_ptr<Reg::Regexp> regexp_ptr;
        std::vector<test> test_cases;
        static int test_number;
        int time_total = 0;


    public:
        tester(std::string reg_text, std::shared_ptr<Reg::Regexp> regexp, std::vector<test> tests, bool optimize = false) : regexp_text(
                std::move(reg_text)), regexp_ptr(std::move(regexp)), test_cases(std::move(tests)) {}

        void start() {
            std::cout << "\n \t Test_Case #" << ++test_number << "\n";
            std::cout << "Regexp: " << ((regexp_text.empty()) ? "{}" : regexp_text) << '\n';
            int test_failed = 0;
            for (auto &test : test_cases) {
                std::cout << "Testing string: " << (test.expression.empty() ? "{}" : test.expression) << '\n';
                std::chrono::steady_clock::time_point start_point = std::chrono::steady_clock::now();
                bool respond = Reg::match(regexp_ptr, test.expression);
                auto matching_time = std::chrono::duration_cast<std::chrono::milliseconds>(
                        std::chrono::steady_clock::now() - start_point).count();

                if (respond != test.expected_resp)
                    test_failed++;

                std::cout << "Expected: " << std::boolalpha << test.expected_resp << "\n Got: " << respond << '\n';
                std::cout << "Matching Duration: " << matching_time << " ms \n\n";
                time_total += matching_time;
            }
            std::cout << "TESTS FAILED: " << test_failed << '\n';
            std::cout << "Total test_case time: " << time_total << '\n';
        }
    };

    int tester::test_number = 0;
}