#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include "regexpr.h"
#include "tests.h"


#define Char std::make_shared<char_>
#define Alt std::make_shared<alt>
#define Concat std::make_shared<concat>
#define Star std::make_shared<star>

int main() {
    std::vector<tests> vec_tests;

    tests test1;
    test1.name = "simple char";
    test1.re = Char(char_('a'));
    test1.str = "a";
    test1.cases = {{"a",          true},
                   {"aa",         false},
                   {"aabb",       false},
                   {"bbbabbaaaa", false}};
    vec_tests.push_back(test1);

    tests test2;
    test2.name = "simple concat";
    test2.re = Concat(Concat(Char(char_('a')), Char(char_('b'))), Char(char_('c')));
    test2.str = "abc";
    test2.cases = {{"abc",  true},
                   {"aab",  false},
                   {"abcc", false},
                   {"a",    false},
                   {"cbs",  false}};
    vec_tests.push_back(test2);

    tests test3;
    test3.name = "simple star";
    test3.re = Star(Char(char_('a')));
    test3.str = "a*";
    test3.cases = {{"a",     true},
                   {"aaaaa", true},
                   {"aaba",  false},
                   {"baaa",  false}};
    vec_tests.push_back(test3);


    tests test4;
    test4.name = "simple alt";
    test4.re = Alt(Concat(Char(char_('a')), Char(char_('b'))), Concat(Char(char_('b')), Char(char_('a'))));
    test4.str = "(ab|ba)";
    test4.cases = {{"ab",   true},
                   {"ba",   true},
                   {"aaaa", false},
                   {"baa",  false}};
    vec_tests.push_back(test4);

    tests test5;
    test5.name = "many stars";
    test5.re = Star(Star(Star(Star(Star(Star(Star(Star(Star(Char(char_('a')))))))))));
    test5.str = "a*********";
    test5.cases = {{"a",               true},
                   {"aaaaaa", true},
                   {"baaaa",           false},
                   {"aaaaaaac",      false}};
    vec_tests.push_back(test5);


    tests test6;
    test6.name = "binary numbers multiple of 3";
    test6.re = Star(Alt(Concat(
            Concat(Char(char_('1')), Star(Concat(Concat(Char(char_('0')), Star(Char(char_('1')))), Char(char_('0'))))),
            Char(char_('1'))), Char(char_('0'))));
    test6.str = "(1(01*0)*1|0)*";
    test6.cases = {{"11",    true},
                   {"10101000", true},
                   {"10001", false},
                   {"1101",  false}};
    vec_tests.push_back(test6);


    tests test7;
    test7.name = "long complex regexpr";
    test7.re = Concat(Concat(Concat(Concat(Star(
                                                   Alt(Concat(Concat(Char(char_('m')), Char(char_('e'))), Concat(Char(char_('o')), Char(char_('w')))),
                                                       Star(Char(char_('a'))))),
                                           Char(char_('a'))),
                                    Alt(Char(char_('b')), Char(char_('c')))),
                             Star(Char(char_('d')))),
                      Star(Star(Concat(Char(char_('x')), Char(char_('y'))))));
    test7.str = "(meow|a*)*a(b|c)d*(xy)**";
    test7.cases = {{"meowmeowacddxy", true},
                   {"aaaabdxyxy",     true},
                   {"meowabd",        true},
                   {"meow",           false},
                   {"xyxyxy",         false},
                   {"aabcdxy",        false},
                   {"meowacdxyxyxyg", false}};
    vec_tests.push_back(test7);


    tests test8;
    test8.name = "long complex regexpr for long time";
    test8.re = Concat(Star(Concat(Alt(Star(Char(char_('a'))), Star(Char(char_('b')))), Concat(Alt(Char(char_('a')), Char(char_('b'))),Alt(Char(char_('a')), Char(char_('b')))))),
                      Concat(Alt(Concat(Char(char_('a')), Char(char_('b'))), Concat(Char(char_('c')),Char(char_('d')))),
                             Star(Concat(Alt(Char(char_('c')), Char(char_('d'))), Concat(Alt(Char(char_('c')), Char(char_('d'))),Alt(Char(char_('c')), Char(char_('d'))))))));
    test8.str = "((a*|b*)(a|b)(a|b))*(ab|cd)((c|d)(c|d)(c|d))*";
    test8.cases = {{"aaabacdccc", true},
                   {"aaaabddd",     true},
                   {"bbbbbaaaaaaabababbbbbbbaaaaaabbbbbbcdcccdddcccdddcdcdcd",        true},
                   {"aaaaababbbbbdcccc",           false},
                   {"bbbabccddd",         false}};
    vec_tests.push_back(test8);

    freopen("result_optimized.txt", "w", stdout);
    for (auto test: vec_tests) {
        test.start();
    }
    return 0;
}