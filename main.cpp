#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include "my_regexp.h"
#include "my_tests.h"
int main() {
    regexp r1(concat, regexp(concat, regexp(char_, 'a'), regexp(char_, 'a')),
              regexp(concat, regexp(char_, 'b'), regexp(char_, 'b')));
    std::vector<tests> testing;
    tests test1;
    test1.number_of_test = 1;
    test1.regexp_ = r1;
    test1.regexp_str = "aabb";
    test1.cases = {{"a", 0},   {"aa", 0},    {"aaa", 0},         {"aabb", 1},
                   {"aab", 0}, {"aabbb", 0}, {"aabbaabbaabb", 0}};
    testing.push_back(test1);
    regexp r2(alt, regexp(concat, regexp(char_, 'a'), regexp(char_, 'a')),
              regexp(concat, regexp(char_, 'b'), regexp(char_, 'b')));
    tests test2;
    test2.number_of_test = 2;
    test2.regexp_ = r2;
    test2.regexp_str = "(aa|bb)";
    test2.cases = {{"a", 0},  {"aa", 1},    {"aaa", 0},         {"aabb", 0},
                   {"bb", 1}, {"aabbb", 0}, {"aabbaabbaabb", 0}};
    testing.push_back(test2);
    tests test3;
    regexp r3(star, regexp(alt, regexp(char_, 'a'), regexp(char_, 'b')));
    test3.number_of_test = 3;
    test3.regexp_ = r3;
    test3.regexp_str = "(a|b)*";
    test3.cases = {{"a", 1},      {"aa", 1}, {"acaef", 0},
                   {"aabb", 1},   {"", 1},   {"aabb21", 0},
                   {"aabbabb", 1}};
    testing.push_back(test3);
    regexp r4(
        star,
        regexp(
            star,
            regexp(
                star,
                regexp(star,
                       regexp(star,
                              regexp(star,
                                     regexp(star,
                                            regexp(concat, regexp(char_, 'a'),
                                                   regexp(char_, 'b')))))))));

    tests test4;
    test4.regexp_str = "(ab)*******";
    test4.regexp_ = r4;
    test4.number_of_test = 4;
    test4.cases = test3.cases = {{"aaaaa", 0},    {"aa", 0}, {"acaefafqe", 0},
                                 {"aabb", 0},     {"", 1},   {"ababab", 1},
                                 {"abaab", 0}};
    testing.push_back(test4);
    tests test5;
    regexp r5(empty);
    test5.number_of_test = 5;
    test5.cases = {{"a", 0},      {"aa", 0}, {"acaefa", 0},
                   {"aabb", 0},   {"", 0},   {"aabb21b", 0},
                   {"aabbabb", 0}};
    testing.push_back(test5);
    for (auto test : testing) {
        test.launch();
    }
    return 0;
}
