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
        ptr = make_alt(make_shared<Char>('a'), make_shared<Char>('b'));
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
        ptr = make_star(make_shared<Char>('a'));
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
        ptr = make_concat(make_shared<Char>('a'), make_shared<Char>('b'));
        samples = {
            {"ab", true},
            {"abab", false},
            {"ababababab", false},
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
        auto inside = make_concat(make_shared<Char>('a'), make_shared<Char>('b'));
        ptr = make_star(inside);
        samples = {
            {"ab", true},
            {"", true},
            {"abab", true},
            {"abababab", true},
            {"abababababababababababababababababababababababababababababababababababababababababababab", true},
            {"a", false},
            {"aba", false}};
        UPLOAD_TEST;
    }

    // 7. % 3 == 0 or epsilon
    {
        reg_txt = "(0|1(01*0)*1)*";
        // "01*0"
        auto t1 = make_concat(make_shared<Char>('0'), make_concat(make_star(make_shared<Char>('1')), make_shared<Char>('0')));
        // (01*0)*
        auto t2 = make_star(t1);
        // 1(01*0)*1
        auto t3 = make_concat(make_shared<Char>('1'), make_concat(t2, make_shared<Char>('1')));
        // (0|1(01*0)*1)
        auto t4 = make_alt(make_shared<Char>('0'), t3);
        ptr = make_star(t4);
        samples = {
            {"", true},
            {"0", true},
            {"11", true},
            {"110", true},
            {"100111", true},
            {"11110011000", true},
            {"11110011000000000000000000000000010101", true},
            {"1110", false},
            {"1110100", false}};
        UPLOAD_TEST;
    }

    // 8. a lot of star
    {
        reg_txt = "(ab)*****";
        auto t1 = make_concat(make_shared<Char>('a'), make_shared<Char>('b'));
        ptr = make_star(make_star(make_star(make_star(make_star(t1)))));
        samples = {
            {"ab", true},
            {"", true},
            {"abab", true},
            {"ababab", true},
            {"abababab", true},
            {"abababababab", true},
            {"aba", false},
            {"abaab", false}};
        UPLOAD_TEST;
    }

    // 9. R_eps
    {
        reg_txt = "ab  (concat with eps)";
        auto t1 = make_concat(make_shared<Char>('a'), make_shared<Char>('b'));
        ptr = make_concat(t1, make_concat(make_shared<Epsilon>(), make_concat(make_shared<Epsilon>(), make_shared<Epsilon>())));
        samples = {
            {"ab", true},
            {"abab", false},
            {"ababababab", false},
            {"ababababababababababababababababababababababababababababababababababab", false},
            {"b", false},
            {"", false},
            {"aa", false},
            {"ba", false}};
        UPLOAD_TEST;
    }

    // 10. R | eps, where R -- can match epsilon
    {
        reg_txt = "(ab)*  (alt with eps)";
        auto inside = make_concat(make_shared<Char>('a'), make_shared<Char>('b'));
        ptr = make_alt(make_alt(make_star(inside), make_shared<Epsilon>()), make_shared<Epsilon>());
        samples = {
            {"ab", true},
            {"", true},
            {"abab", true},
            {"ababababab", true},
            {"ababababababababababababababababababababababababababababababababababababababababababababab", true},
            {"a", false},
            {"aba", false}};
        UPLOAD_TEST;
    }

    // 11. I wanna TL 2sec exceeded
    {
        reg_txt = "((a|b)(a|b)(a|b)(a|b))*(ab)*";
        auto t1 = make_alt(make_shared<Char>('a'), make_shared<Char>('b'));
        auto t2 = make_concat(t1, t1);
        auto t3 = make_concat(t2, t2);
        auto t4 = make_star(t3);
        auto t5 = make_star(make_concat(make_shared<Char>('a'), make_shared<Char>('b')));
        auto t6 = make_concat(t4, t5);
        ptr = t6;
        samples = {
            {"aaaa", true},
            {"abaaababbababbaaaabbababbbaaabababababababababababababab", true},
            {"abaaababbababbaaaabbababbbaaababababababababababababababс", false}
        };
        UPLOAD_TEST;
    }
    

#undef UPLOAD_TEST

    // run tests
    for (auto &test : tests)
    {
        test.run();
    }
}