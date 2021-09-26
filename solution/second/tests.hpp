#pragma once
#include "regexp.hpp"
#include <chrono>
#include <iostream>
#include <exception>

namespace NTests
{
    struct TestSuite
    {
        TestSuite(std::string reg_text_, NRegexp::RegPtr reg_ptr_, std::vector<std::pair<std::string, bool>> samples_) : reg_text(std::move(reg_text_)),
                                                                                                     reg_ptr(std::move(reg_ptr_)),
                                                                                                     samples(std::move(samples_))
        {
        }
        void run()
        {
            std::cout << "\n\t---- NEW TEST SUITE ----\n";
            std::cout << "Regexp: " << (reg_text.empty() ? std::string("{epsilon}") : reg_text) << "\n\n";
            size_t bad_tests = 0;
            for (const auto &sample : samples)
            {
                std::cout << "Sample: " << (sample.first.empty() ? std::string("{epsilon}") : sample.first) << '\n';
                std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
                try
                {
                    bool resp = NRegexp::Match(reg_ptr, sample.first);
                    auto match_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begin).count();

                    if (resp != sample.second) {
                        bad_tests++;
                    }

                    std::cout << "Matched\\Expected: " << std::boolalpha << resp << "\\" << sample.second << '\n';
                    std::cout << "Matching time(ms): " << match_time << "\n\n";
                    total_time += match_time;
                }
                catch (const std::exception &e)
                {
                    std::cout << "Exception: " << e.what() << '\n';
                }
            }
            std::cout << "BROKEN TESTS: " << bad_tests << "\n";
            std::cout << "TOTAL TIME OF SUIT(ms): " << total_time << '\n';
        }

    private:
        std::string reg_text;
        NRegexp::RegPtr reg_ptr;
        std::vector<std::pair<std::string, bool>> samples;
        std::size_t total_time = 0;
    };
} // namespace NTests
