#include <iostream>
#include <memory>
#include "regexp.h"
#include "test.h"

int main() {
    using namespace Reg;
    using namespace Tests;

    // 1. simple symbol alternative
    {
        auto t1 = alt(symb('1'), symb('0')); // (1|0)
        tester test_case1("(1|0)", t1, {{"1",  true},
                                        {"0",  true},
                                        {"10", false},
                                        {"5",  false}});
        test_case1.start();
    }

    // 2. simple star
    {
        auto t2 = star(symb('a')); // (a*)
        tester test_case2("(a*)", t2, {{"a",           true},
                                       {"aaa",         true},
                                       {"aaaaaaaaaaa", true},
                                       {"aaaaaaaabb",  false}});
        test_case2.start();
    }

    // 3. Epsilon
    {
        auto t3 = epsilon();
        tester test_case3("", t3, {{"",   true},
                                   {"a",  false},
                                   {"ba", false}});
        test_case3.start();
    }

    // 4. concat of two stars
    {
        auto t4 = concat(star(symb('1')),
                         star(symb('0'))); // (1*0*)
        tester test_case4("(1*0*)", t4, {{"0001",      false},
                                         {"110",       true},
                                         {"111100000", true},
                                         {"456545",    false}});
        test_case4.start();
    }

    // 5. binary divided by 3 or epsilon
    {
        auto x1 = concat(symb('0'),
                         concat(star(symb('1')),
                                symb('0'))); // (01*0)
        auto x2 = star(x1); // (01*0)*
        auto x3 = concat(symb('1'),
                         concat(x2, symb('1'))); // 1(01*0)*1
        auto t5 = star(alt(symb('0'), x3));

        std::string nine = "1001";
        std::string big_test = nine;
        for (int i = 0; i < 10; i++) {
            big_test += nine;
        }

        tester test_case5("(0|1(01*0)*1)*", t5, {{"11",        true},
                                                 {"10000",     false},
                                                 {"1001",      true},
                                                 {"100001",    true},
                                                 {"111100000", true},
                                                 {"",          true},
                                                 {big_test,    true},
                                                 {"111",       false}});

        test_case5.start();
    }
    // 6. star of concat
    {
        auto t6 = star(
                concat(symb('x'), symb('y')));
        tester test_case6("(xy)*", t6, {{"",                                   true},
                                        {"xy",                                 true},
                                        {"xyxyxy",                             true},
                                        {"xyxyxyxyxyxyxyxyxyxyxyxyxyxyxyxyxy", true},
                                        {"xyxyx",                              false},
                                        {"yxy",                                false}});
        test_case6.start();
    }

    // 7. ((a|b)*x(a|b)*y(a|b)*z)
    {

        auto x1 = star(
                std::make_shared<Alt>(symb('a'), symb('b'))); //(a|b)*
        auto x2 = concat(x1, concat(x1, symb('z'))); // (a|b)*z
        auto x3 = concat(x1,
                         concat(symb('y'), x2)); // (a|b)*y(a|b)*z
        auto x4 = concat(x1, concat(symb('x'), x3));

        std::string part = "aaabbbb";
        for (int i = 0; i < 3; i++) {
            part += part;
        }
        std::string big_str = part + 'x' + part + 'y' + part + 'z';

        tester test_case7("((a|b)*x(a|b)*y(a|b)*z)", x4, {{"aaaxbbbyaaaz",            true},
                                                          {"aaaabbbbbxabababyaaaaaz", true},
                                                          {big_str,                   true}});

        test_case7.start();
    }

    // 8. Nested stars (ab|ba)******
    {
        auto t8 = star(star(star(star(star(star(alt(concat(symb('a'), symb('b')), concat(symb('b'), symb('a')))))))));
        tester test_case8("(ab|ba)******", t8, {{"ababababbabababa", true}});
        test_case8.start();
    }

    // 9. Many concats with Epsilon ({eps}{eps}ab{eps}{eps}{eps}{eps})
    {
        auto t9 = concat(epsilon(), concat(epsilon(), concat(symb('a'), concat(symb('b'), concat(epsilon(),
                                                                                                 concat(epsilon(),
                                                                                                        concat(epsilon(),
                                                                                                               epsilon())))))));
        tester test_case9("({eps}{eps}ab{eps}{eps}{eps}{eps})", t9, {{"ab",  true},
                                                                     {"abb", false}
        });
        test_case9.start();
    }

    // 10. Attempt to exceed 2sec matching time
    // ((a|b)(a|b)(a|b)(a|b))*(abcd)*((fg|nm)*|d*)*
    {
        auto seq = star(concat(symb('a'), concat(symb('b'), concat(symb('c'), symb('d'))))); // (abcd)*
        auto seed = alt(symb('a'), symb('b')); // (a|b)
        auto comb = star(concat(seed, concat(seed, concat(seed, seed)))); // (a|b)(a|b)(a|b)(a|b))*
        auto tail = star(alt(star(alt(concat(symb('f'), symb('g')), concat(symb('n'), symb('m')))), star(symb('d'))));

        auto t10 = concat(comb, concat(seq, tail));

        std::string big_test = "aaaabbbb";
        for (int i = 0; i < 5; i++) {
            big_test += big_test;
        }
        for (int i = 0; i < 3; i++) {
            big_test += "abcd";
        }
        for (int i = 0; i < 5; i++) {
            big_test += (i % 2 == 0 ? "fgnm" : "d");
        }
        tester test_case10("((a|b)(a|b)(a|b)(a|b))*(abcd)*((fg|nm)*|d*)", t10, {{"abababcd", true},
                                                                                {big_test, true}});
        test_case10.start();
    }

    return 0;
}