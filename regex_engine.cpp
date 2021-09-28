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

common_regex_t regex_engine::operator+(common_regex_t const &lhs, common_regex_t const &rhs) {
  return concat(lhs, rhs);
}

common_regex_t regex_engine::operator*(common_regex_t const &lhs) {
  return many(lhs);
}

common_regex_t regex_engine::operator|(common_regex_t const &lhs, common_regex_t const &rhs) {
  return either(lhs, rhs);
}

// WARNING: shit code
bool regex_engine::are_equal(common_regex_t const &lhs, common_regex_t const &rhs) {
  if (lhs->type() != rhs->type()) return false;

  // Can be done faster I guess, works in O(#if * dynamic_cast)

  if (lhs->type() == CHAR) {
    return std::dynamic_pointer_cast<char_regex>(lhs)->value == std::dynamic_pointer_cast<char_regex>(rhs)->value;
  }

  if (lhs->type() == STAR) {
    return are_equal(std::dynamic_pointer_cast<star_regex>(lhs)->regex,
                     std::dynamic_pointer_cast<star_regex>(rhs)->regex);
  }

  if (lhs->type() == ALT) {
    std::shared_ptr<alt_regex> lhs_cast = std::dynamic_pointer_cast<alt_regex>(lhs);
    std::shared_ptr<alt_regex> rhs_cast = std::dynamic_pointer_cast<alt_regex>(rhs);
    return are_equal(lhs_cast->alt1, rhs_cast->alt1) && are_equal(lhs_cast->alt2, rhs_cast->alt2);

  }

  if (lhs->type() == CONCAT) {
    std::shared_ptr<concat_regex> lhs_cast = std::dynamic_pointer_cast<concat_regex>(lhs);
    std::shared_ptr<concat_regex> rhs_cast = std::dynamic_pointer_cast<concat_regex>(rhs);
    return are_equal(lhs_cast->concat1, rhs_cast->concat1) && are_equal(lhs_cast->concat2, rhs_cast->concat2);
  }

  // I could have tried to do same kind of effect by defining a virtual function, that would work, but then
  // I would have dynamic_cast in my code anyway or maybe use some visitor pattern, that is less disgusting, but still pretty messy.
  // I will try to think of a better solution though


  return true;

}

bool regex_engine::is_epsilon(common_regex_t const &regex) {
  return regex->type() == EPSILON;
}

bool regex_engine::is_empty(common_regex_t const &regex) {
  return regex->type() == EMPTY;
}

common_regex_t
regex_engine::concat(common_regex_t const &lhs, common_regex_t const &rhs) {
  if (is_empty(lhs) || is_empty(rhs)) {
    return empty_set();
  }
  if (is_epsilon(lhs) || is_epsilon(rhs)) {
    return is_epsilon(lhs) ? rhs : lhs;
  }
  return std::make_shared<concat_regex>(lhs, rhs);
}

common_regex_t
regex_engine::either(common_regex_t const &lhs, common_regex_t const &rhs) {
  if (is_empty(lhs) || is_empty(rhs)) {
    return is_empty(lhs) ? rhs : lhs;
  }
  if (is_epsilon(lhs) || is_epsilon(rhs)) {
    if (is_epsilon(lhs) && rhs->nullable()) {
      return rhs;
    }
    if (is_epsilon(rhs) && lhs->nullable()) {
      return lhs;
    }
  }
  if (are_equal(lhs, rhs)) {
    return lhs;
  }
  return std::make_shared<alt_regex>(lhs, rhs);
}

common_regex_t regex_engine::many(common_regex_t const &regex) {
  if (is_empty(regex) || is_epsilon(regex)) {
    return is_empty(regex) ? empty_set() : epsilon();
  }
  if (regex->type() == STAR) {
    return regex;
  }
  return std::make_shared<star_regex>(regex);
}

common_regex_t regex_engine::symb(char c) { return std::make_shared<char_regex>(c); }

common_regex_t regex_engine::empty_set() { return std::make_shared<empty_regex>(); }

common_regex_t regex_engine::epsilon() { return std::make_shared<epsilon_regex>(); }


/* EMPTY REGEX, begin */
bool empty_regex::nullable() { return false; }

common_regex_t empty_regex::derivative(char c) { return empty_set(); }

regex_types empty_regex::type() const { return EMPTY; }
/* EMPTY REGEX, end */


/* EPSILON REGEX, begin */
bool epsilon_regex::nullable() { return true; }

common_regex_t epsilon_regex::derivative(char c) { return empty_set(); }

regex_types epsilon_regex::type() const { return EPSILON; }
/* EPSILON REGEX, end */


/* CONCAT REGEX, begin */
concat_regex::concat_regex(common_regex_t concat1, common_regex_t concat2)
        : concat1(std::move(concat1)),
          concat2(std::move(concat2)) {}

bool concat_regex::nullable() { return concat1->nullable() && concat2->nullable(); }


common_regex_t concat_regex::derivative(char c) {
  return either(
          concat(concat1->derivative(c), concat2),
          !concat1->nullable() ? empty_set() : concat2->derivative(c)
  );
}

regex_types concat_regex::type() const { return CONCAT; }
/* CONCAT REGEX, end */


/* STAR REGEX, begin */
star_regex::star_regex(common_regex_t regex) : regex(std::move(regex)) {}

bool star_regex::nullable() { return true; }

common_regex_t star_regex::derivative(char c) { return concat(regex->derivative(c), many(regex)); }

regex_types star_regex::type() const { return STAR; }
/* STAR REGEX, end */


/* ALT REGEX, begin */
alt_regex::alt_regex(common_regex_t alt1, common_regex_t alt2)
        : alt1(std::move(alt1)),
          alt2(std::move(alt2)) {}

bool alt_regex::nullable() { return alt1->nullable() || alt2->nullable(); }

common_regex_t alt_regex::derivative(char c) {
  auto lhs = alt1->derivative(c);
  auto rhs = alt2->derivative(c);
  return either(lhs, rhs);
}

regex_types alt_regex::type() const { return ALT; }
/* ALT REGEX, end */


/* CHAR REGEX, begin */
char_regex::char_regex(char c) : value(c) {}

bool char_regex::nullable() { return false; }

common_regex_t char_regex::derivative(char c) {
  if (value == c) {
    return epsilon();
  }
  return empty_set();
}

regex_types char_regex::type() const { return CHAR; }
/* CHAR REGEX, end */



common_regex_t derivative_strike(std::string &pattern, common_regex_t regex) {
  if (pattern.empty()) {
    return regex;
  }
  char front_char = pattern.back();
  pattern.pop_back();
  return derivative_strike(pattern, regex->derivative(front_char));
}

bool match_internal(std::string &pattern, common_regex_t regex) {
  return derivative_strike(pattern, std::move(regex))->nullable();
}

bool match(std::string pattern, common_regex_t regex) {
  std::reverse(std::begin(pattern), std::end(pattern));
  return match_internal(pattern, std::move(regex));
}

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
  // Doesn't even finish without current optimizations
  assert(match("aaaaaaaaaaaaaaaaabbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb", many(either(symb('a'), symb('b')))));
  // Equivalent to first assert
  assert(match("aabbba", symb('a') + *(symb('a') | symb('b'))));
  assert(are_equal(symb('a'), symb('a')));
  assert(!are_equal(symb('a'), symb('b')));
  assert(are_equal(symb('a') | symb('c'), symb('a') | symb('c')));
  assert(!are_equal(symb('a') | symb('c'), symb('a') | symb('b')));
  assert(!are_equal(symb('a') | symb('c'), *(symb('a') | symb('b'))));
  assert(!are_equal(*(symb('a') | symb('c')), *(symb('a') | symb('b'))));
}

int main() {
  test();
  return 0;
}
