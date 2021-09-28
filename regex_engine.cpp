//
// Created by egor on 27.09.2021.
//
#include <string>
#include <algorithm>
#include <memory>
#include <iostream>
#include <cassert>
#include <utility>
#include "regex_engine.h"

using namespace regex_engine;

std::shared_ptr<abstract_regex>
regex_engine::intersect(std::shared_ptr<abstract_regex> const &lhs, std::shared_ptr<abstract_regex> const &rhs) {
    if (lhs->is_epsilon() && rhs->is_epsilon()) {
        return std::make_shared<epsilon_regex>();
    }
    return std::make_shared<empty_regex>();
}

std::shared_ptr<abstract_regex>
regex_engine::unite(std::shared_ptr<abstract_regex> const &lhs, std::shared_ptr<abstract_regex> const &rhs) {
    if (lhs->is_epsilon() || rhs->is_epsilon()) {
        return std::make_shared<epsilon_regex>();
    }
    return std::make_shared<empty_regex>();
}

std::shared_ptr<abstract_regex>
regex_engine::concat(std::shared_ptr<abstract_regex> const &lhs, std::shared_ptr<abstract_regex> const &rhs) {
    return std::make_shared<concat_regex>(lhs, rhs);
}

std::shared_ptr<abstract_regex>
regex_engine::either(std::shared_ptr<abstract_regex> const &lhs, std::shared_ptr<abstract_regex> const &rhs) {
    return std::make_shared<alt_regex>(lhs, rhs);
}

std::shared_ptr<abstract_regex> regex_engine::many(std::shared_ptr<abstract_regex> const &regex) {
    return std::make_shared<star_regex>(regex);
}

std::shared_ptr<abstract_regex> regex_engine::symb(char c) {
    return std::make_shared<char_regex>(c);
}

std::shared_ptr<abstract_regex> regex_engine::empty_set() {
    return std::make_shared<empty_regex>();
}

std::shared_ptr<abstract_regex> regex_engine::epsilon() {
    return std::make_shared<epsilon_regex>();
}

/* EMPTY REGEX, begin */
std::shared_ptr<abstract_regex> empty_regex::nullable() {
    return empty_set();
}

std::shared_ptr<abstract_regex> empty_regex::derivative(char c) {
    return empty_set();
}

bool empty_regex::is_empty() const { return true; }
/* EMPTY REGEX, end */

/* EPSILON REGEX, begin */
std::shared_ptr<abstract_regex> epsilon_regex::nullable() {
    return epsilon();
}

std::shared_ptr<abstract_regex> epsilon_regex::derivative(char c) {
    return empty_set();
}

bool epsilon_regex::is_epsilon() const { return true; }
/* EPSILON REGEX, end */

/* CONCAT REGEX, begin */
concat_regex::concat_regex(std::shared_ptr<abstract_regex> concat1, std::shared_ptr<abstract_regex> concat2)
    : concat1(std::move(concat1)),
    concat2(std::move(concat2)) {}

std::shared_ptr<abstract_regex> concat_regex::nullable() {
    return intersect(concat1->nullable(), concat2->nullable());
}


std::shared_ptr<abstract_regex> concat_regex::derivative(char c) {
    return either(
            concat(concat1->derivative(c), concat2),
            concat(concat1->nullable(), concat2->derivative(c))
            );
}
/* CONCAT REGEX, end */

/* STAR REGEX, begin */
star_regex::star_regex(std::shared_ptr<abstract_regex> regex) : regex(std::move(regex)) {}

std::shared_ptr<abstract_regex> star_regex::nullable() {
    return epsilon();
}

std::shared_ptr<abstract_regex> star_regex::derivative(char c)  {
    return concat(regex->derivative(c), many(regex));
}
/* STAR REGEX, end */

/* ALT REGEX, begin */
alt_regex::alt_regex(std::shared_ptr<abstract_regex> alt1, std::shared_ptr<abstract_regex> alt2)
    : alt1(std::move(alt1)),
    alt2(std::move(alt2)) {}

std::shared_ptr<abstract_regex> alt_regex::nullable() {
    return unite(alt1->nullable(), alt2->nullable());
}

std::shared_ptr<abstract_regex> alt_regex::derivative(char c) {
    return either(alt1->derivative(c), alt2->derivative(c));
}
/* ALT REGEX, end */

/* CHAR REGEX, begin */
char_regex::char_regex(char c) : value(c) {}

std::shared_ptr<abstract_regex> char_regex::nullable() {
    return empty_set();
}

std::shared_ptr<abstract_regex> char_regex::derivative(char c) {
    if (value == c) {
        return epsilon();
    }
    return empty_set();
}
/* CHAR REGEX, end */

void test() {
    assert(match("aabbba", concat(symb('a'), many(either(symb('a'), symb('b'))))));
    assert(!match("caaa", many(either(symb('a'), symb('b')))));
    assert(match("caaa", many(either(symb('a'), symb('c')))));
    assert(match("aaaabbbbb", concat(many(symb('a')), many(symb('b')))));
    assert(!match("bbbbaaaaa", concat(many(symb('a')), many(symb('b')))));
    assert(match("ababab", many(either(concat(symb('a'), symb('b')), concat(symb('b'), symb('a'))))));
    assert(!match("ababa", many(either(concat(symb('a'), symb('b')), concat(symb('b'), symb('a'))))));
    assert(match("bababa", many(either(concat(symb('a'), symb('b')), concat(symb('b'), symb('a'))))));
    assert(!match("babab", many(either(concat(symb('a'), symb('b')), concat(symb('b'), symb('a'))))));
}

int main() {
    test();
    return 0;
}
