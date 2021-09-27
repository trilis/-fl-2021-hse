#include <iostream>
#include "my_regexp.h"
#include "my_tests.h"
#include <chrono>
#include <fstream>
void tests::launch() {
    std::cout << "TEST CASE №" << number_of_test << "\n"
              << "\n";
    std::cout << "REGULAR EXPRESSION: " << regexp_str << "\n"
              << "\n";
    for (auto Case : cases) {
        std::cout << "STRING: " << Case.str << "\n";
        bool expected_ = Case.expected;
        std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();
        bool matched_ = match(regexp_, std::move(Case.str));
        std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();
        if (expected_) {
            std::cout << "EXPECTED: TRUE\n";
        } else {
            std::cout << "EXPECTED: FALSE\n";
        }
        if (matched_) {
            std::cout << "MATCHED: TRUE\n";
        } else {
            std::cout << "MATCHED: FALSE\n";
        }
        std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
        std::cout << "TIME AFTER OPTIMIZATION: " << time_span.count() * 1000 << " ms" << "\n";
        std::cout << "\n";
        if (matched_ == expected_) {
            passed++;
        } else {
            failed++;
        }
    }
    std::cout << "PASSED: " << passed << "\n";
    std::cout << "FAILED: " << failed << "\n"
              << "\n";
}
tests::tests()
    : cases(),
      regexp_(empty),
      regexp_str(),
      passed(0),
      failed(0),
      number_of_test(0) {
}
