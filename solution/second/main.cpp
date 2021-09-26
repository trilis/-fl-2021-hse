#include <iostream>
#include <memory>
#include <type_traits>
#include <cassert>
#include <sstream>
#include <vector>

#include "regexp.hpp"
#include "tests.hpp"

int main()
{
    using namespace NRegexp;
    using namespace NTests;
    using std::make_shared;

    std::string reg_txt;
    RegPtr ptr = nullptr;
    std::vector<std::pair<std::string, bool>> samples;

    std::vector<TestSuite> tests;

#define UPLOAD_TEST tests.push_back(TestSuite(std::move(reg_txt), ptr, std::move(samples)))

    // 1. simple
    {
        reg_txt = "a";
        ptr = make_shared<Char>('a');
        samples.clear();
        samples = {{"a", true},
                   {"", false},
                   {"aa", false},
                   {"aaaaaaaaaaa", false},
                   {"ab", false},
                   {"b", false},
                   {"ba", false},
                   {"A", false}};
        UPLOAD_TEST;
    }

    // 2. with Alternative
    {
        reg_txt = "a|b";
        ptr = make_shared<Alt>(make_shared<Char>('a'), make_shared<Char>('b'));
        samples = {{"", false},
                   {"a", true},
                   {"b", true},
                   {"ab", false},
                   {"ba", false},
                   {"bb", false}};
        UPLOAD_TEST;
    }

    // 3. epsilon
    {
        reg_txt = "";
        ptr = make_shared<Epsilon>();
        samples = {{"", true},
                   {"ab", false},
                   {"ba", false},
                   {"12345", false}};
        UPLOAD_TEST;
    }

    // 4. simple star
    {
        reg_txt = "a*";
        ptr = make_shared<Star>(make_shared<Char>('a'));
        samples = {
            {"", true},
            {"a", true},
            {"aaa", true},
            {"aaaaaaa", true},
            {"abc", false},
            // {"aaaaaaaaaaaaaaabaaaaaaaa", false} // Cannot be computed in normal time without optimization
        };
        UPLOAD_TEST;
    }

    // 5. simple concat
    {
        reg_txt = "ab";
        ptr = make_shared<Concat>(make_shared<Char>('a'), make_shared<Char>('b'));
        samples = {
            {"ab", true},
            {"abab", false},
            {"a", false},
            {"b", false},
            {"", false},
            {"aa", false},
            {"ba", false}};
        UPLOAD_TEST;
    }

    // 6. simple concat + star
    {
        reg_txt = "(ab)*";
        auto inside = make_shared<Concat>(make_shared<Char>('a'), make_shared<Char>('b'));
        ptr = make_shared<Star>(inside);
        samples = {
            {"ab", true},
            {"", true},
            {"abab", true},
            {"a", false},
            {"aba", false}
        };
        UPLOAD_TEST;
    }

    // run tests
    for (auto &test : tests)
    {
        test.run();
    }
}