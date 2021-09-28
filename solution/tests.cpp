#include <iostream>
#include <string>
#include <chrono>
#include "regexpr.h"
#include "tests.h"

void tests::start() {
    std::cout << "Name: " << name << '\n';
    std::cout << "Test: " << str << '\n' << '\n';
    for (const auto &case_: cases) {
        std::cout << "Case: " << case_.str << '\n';
        std::chrono::steady_clock::time_point st = std::chrono::steady_clock::now();
        bool ans = match(re, case_.str);
        std::chrono::steady_clock::time_point fn = std::chrono::steady_clock::now();
        std::cout << "Expected: ";
        if (case_.answer) {
            std::cout << "True\n";
        } else {
            std::cout << "False\n";
        }
        std::cout << "Match: ";
        if (ans) {
            std::cout << "True\n";
        } else {
            std::cout << "False\n";
        }
        std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(fn - st);
        std::cout << "Time: " << time_span.count() * 1000 << " ms" << '\n' << '\n';
        if (ans == case_.answer) {
            passed++;
        }
    }

    std::cout << '\n' << "Passed: " << passed << '/' << cases.size() << '\n';
    std::cout << "-----------------------------------------------\n\n";


}
