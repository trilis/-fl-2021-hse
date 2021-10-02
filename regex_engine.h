//
// Created by egor on 27.09.2021.
//

#ifndef FL_2021_HSE_REGEX_ENGINE_H
#define FL_2021_HSE_REGEX_ENGINE_H

namespace regex_engine {

struct empty_label_t {
};
struct epsilon_label_t {
};

enum regex_types {
  STAR, CHAR, ALT, CONCAT, EPSILON, EMPTY
};

struct abstract_regex {

  virtual bool nullable() = 0;

  virtual std::shared_ptr<abstract_regex> derivative(char c) = 0;

  virtual regex_types type() const = 0;

  virtual ~abstract_regex() = default;

};

using common_regex_t = std::shared_ptr<abstract_regex>;

bool is_epsilon(common_regex_t const &regex);

bool is_empty(common_regex_t const &regex);

struct empty_regex : abstract_regex {
  // In case of SFINAE workarounds, also occurred pretty handy in debugging,
  // it was easier to see what type does object actually have
  empty_label_t empty_label;

  bool nullable() override;

  std::shared_ptr<abstract_regex> derivative(char c) override;

  regex_types type() const override;

};

struct epsilon_regex : abstract_regex {
  // In case of SFINAE workarounds
  epsilon_label_t epsilon_label;

  bool nullable() override;

  std::shared_ptr<abstract_regex> derivative(char c) override;

  regex_types type() const override;

};

struct concat_regex : abstract_regex {
  std::shared_ptr<abstract_regex> concat1, concat2;

  concat_regex(std::shared_ptr<abstract_regex> concat1, std::shared_ptr<abstract_regex> concat2);

  bool nullable() override;

  std::shared_ptr<abstract_regex> derivative(char c) override;

  regex_types type() const override;
};

struct star_regex : abstract_regex {
  std::shared_ptr<abstract_regex> regex;

  explicit star_regex(std::shared_ptr<abstract_regex> regex);

  bool nullable() override;

  std::shared_ptr<abstract_regex> derivative(char c) override;

  regex_types type() const override;
};

struct alt_regex : abstract_regex {
  std::shared_ptr<abstract_regex> alt1, alt2;

  alt_regex(std::shared_ptr<abstract_regex> alt1, std::shared_ptr<abstract_regex> alt2);

  bool nullable() override;

  std::shared_ptr<abstract_regex> derivative(char c) override;

  regex_types type() const override;
};

struct char_regex : abstract_regex {
  char value = '\0';

  explicit char_regex(char c);

  bool nullable() override;

  std::shared_ptr<abstract_regex> derivative(char c) override;

  regex_types type() const override;
};

std::shared_ptr<abstract_regex>
concat(std::shared_ptr<abstract_regex> const &lhs, std::shared_ptr<abstract_regex> const &rhs);

std::shared_ptr<abstract_regex>
either(std::shared_ptr<abstract_regex> const &lhs, std::shared_ptr<abstract_regex> const &rhs);

std::shared_ptr<abstract_regex> many(std::shared_ptr<abstract_regex> const &regex);

std::shared_ptr<abstract_regex> symb(char c);

std::shared_ptr<abstract_regex> empty_set();

std::shared_ptr<abstract_regex> epsilon();

common_regex_t operator+(common_regex_t const &lhs, common_regex_t const &rhs);

common_regex_t operator*(common_regex_t const &regex);

common_regex_t operator|(common_regex_t const &lhs, common_regex_t const &rhs);

bool same(common_regex_t const &lhs, common_regex_t const &rhs);

}

#endif //FL_2021_HSE_REGEX_ENGINE_H
