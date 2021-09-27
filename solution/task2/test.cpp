#include <iostream>
#include <memory>
#include "regexp.h"
#include "test.h"

int main() {
    using namespace Reg;
    using namespace Tests;

    // 1. simple symbol alternative
    {
        auto t1 = std::make_shared<Alt>(std::make_shared<Symb>('1'), std::make_shared<Symb>('0')); // (1|0)
        tester simple1("(1|0)", t1, {{"1",  true},
                                     {"0",  true},
                                     {"10", false},
                                     {"5",  false}});
        simple1.start();
    }

    // 2. simple star
    {
        auto t2 = std::make_shared<Star>(std::make_shared<Symb>('a')); // (a*)
        tester simple2("(a*)", t2, {{"a",           true},
                                    {"aaa",         true},
                                    {"aaaaaaaaaaa", true},
                                    {"aaaaaaaabb",  false}});
        simple2.start();
    }

    // 2. concat of two stars
    {
        auto t3 = std::make_shared<Concat>(std::make_shared<Star>(std::make_shared<Symb>('1')),
                                           std::make_shared<Star>(std::make_shared<Symb>('0'))); // (1*0*)
        tester simple3("(1*0*)", t3, {{"0001",      false},
                                      {"110",       true},
                                      {"111100000", true},
                                      {"456545",    false}});
        simple3.start();
    }

    // 3. binary divided by 3 or epsilon
    {
        auto x1 = std::make_shared<Concat>(std::make_shared<Symb>('0'),
                                           std::make_shared<Concat>(std::make_shared<Star>(std::make_shared<Symb>('1')),
                                                                    std::make_shared<Symb>('0'))); // (01*0)
        auto x2 = std::make_shared<Star>(x1); // (01*0)*
        auto x3 = std::make_shared<Concat>(std::make_shared<Symb>('1'),
                                           std::make_shared<Concat>(x2, std::make_shared<Symb>('1'))); // 1(01*0)*1
        auto t3 = std::make_shared<Star>(std::make_shared<Alt>(std::make_shared<Symb>('0'), x3));
        tester medium1("(0|1(01*0)*1)*", t3, {{"11",     true},
                                              {"10000",  false},
                                              {"1001",   true},
                                              {"100001", true},
                                              {"",       true}});
        medium1.start();
    }
    // 4. star of concat
    {
        auto t4 = std::make_shared<Star>(
                std::make_shared<Concat>(std::make_shared<Symb>('x'), std::make_shared<Symb>('y')));
        tester medium2("(xy)*", t4, {{"",           true},
                                     {"xy",         true},
                                     {"xyxyxy",     true},
                                     {"xyxyxyxyxy", true},
                                     {"xyxyx",      false},
                                     {"yxy",        false}});
        medium2.start();
    }

    return 0;
}