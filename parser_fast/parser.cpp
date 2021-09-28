#include "parser.h"
#include <cassert>
#include <cstring>
#include <memory>

Char::Char(char value) : value(value) {}

bool Char::operator==(const Char &other) const { return value == other.value; };

Star::Star(Regex *inside) : inside(inside) {}
Star::Star(const std::shared_ptr<Regex> &inside) : inside(inside) {}

std::shared_ptr<Regex> Star::get_inside() const { return inside; }

std::shared_ptr<Regex> BinaryOperator::get_left() const { return left; }

std::shared_ptr<Regex> BinaryOperator::get_right() const { return right; }

BinaryOperator::BinaryOperator(Regex *left, Regex *right)
    : left(left), right(right) {}
BinaryOperator::BinaryOperator(const std::shared_ptr<Regex> &left,
                               const std::shared_ptr<Regex> &right)
    : left(left), right(right) {}

Alt::Alt(Regex *left, Regex *right) : BinaryOperator(left, right) {}
Alt::Alt(const std::shared_ptr<Regex> &left,
         const std::shared_ptr<Regex> &right)
    : BinaryOperator(left, right) {}

Concat::Concat(Regex *left, Regex *right) : BinaryOperator(left, right) {}
Concat::Concat(const std::shared_ptr<Regex> &left,
               const std::shared_ptr<Regex> &right)
    : BinaryOperator(left, right) {}

static bool nullable(const std::shared_ptr<Regex> &reg) {
  if (dynamic_cast<Empty *>(reg.get())) {
    return false;
  }

  if (dynamic_cast<Epsilon *>(reg.get())) {
    return true;
  }

  if (dynamic_cast<Char *>(reg.get())) {
    return false;
  }

  if (Concat * concat; concat = dynamic_cast<Concat *>(reg.get())) {
    return nullable(concat->get_left()) && nullable(concat->get_right());
  }

  if (Alt * alt; alt = dynamic_cast<Alt *>(reg.get())) {
    return nullable(alt->get_left()) || nullable(alt->get_right());
  }

  if (dynamic_cast<Star *>(reg.get())) {
    return true;
  }

  // throw

  return false;
}

static bool is_equal(const std::shared_ptr<Regex> &left,
                     const std::shared_ptr<Regex> &right) {
  if (dynamic_cast<Epsilon *>(left.get()) &&
      dynamic_cast<Epsilon *>(right.get())) {
    return true;
  }

  if (dynamic_cast<Empty *>(left.get()) && dynamic_cast<Empty *>(right.get())) {
    return true;
  }

  if (dynamic_cast<Char *>(left.get()) && dynamic_cast<Char *>(right.get())) {
    return *dynamic_cast<Char *>(left.get()) ==
           *dynamic_cast<Char *>(right.get());
  }

  if (dynamic_cast<Star *>(left.get()) && dynamic_cast<Star *>(right.get())) {
    return is_equal(dynamic_cast<Star *>(left.get())->get_inside(),
                    dynamic_cast<Star *>(right.get())->get_inside());
  }

  if (dynamic_cast<Concat *>(left.get()) &&
      dynamic_cast<Concat *>(right.get())) {
    return is_equal(dynamic_cast<Concat *>(left.get())->get_left(),
                    dynamic_cast<Concat *>(right.get())->get_left()) &&
           is_equal(dynamic_cast<Concat *>(left.get())->get_right(),
                    dynamic_cast<Concat *>(right.get())->get_right());
  }

  if (dynamic_cast<Alt *>(left.get()) && dynamic_cast<Alt *>(right.get())) {
    return is_equal(dynamic_cast<Alt *>(left.get())->get_left(),
                    dynamic_cast<Alt *>(right.get())->get_left()) &&
           is_equal(dynamic_cast<Alt *>(left.get())->get_right(),
                    dynamic_cast<Alt *>(right.get())->get_right());
  }

  return false;
}

static std::shared_ptr<Regex> optimize(const std::shared_ptr<Regex> &reg) {
  if (Concat * concat; concat = dynamic_cast<Concat *>(reg.get())) {
    if (dynamic_cast<Empty *>(concat->get_left().get())) {
      return concat->get_left();
    }

    if (dynamic_cast<Empty *>(concat->get_right().get())) {
      return concat->get_right();
    }

    if (dynamic_cast<Epsilon *>(concat->get_left().get())) {
      return concat->get_right();
    }

    if (dynamic_cast<Epsilon *>(concat->get_right().get())) {
      return concat->get_left();
    }
  }

  if (Alt * alt; alt = dynamic_cast<Alt *>(reg.get())) {
    if (dynamic_cast<Empty *>(alt->get_left().get())) {
      return alt->get_right();
    }

    if (dynamic_cast<Empty *>(alt->get_right().get())) {
      return alt->get_left();
    }

    if (dynamic_cast<Epsilon *>(alt->get_right().get()) &&
        nullable(alt->get_left())) {
      return alt->get_left();
    }

    if (dynamic_cast<Epsilon *>(alt->get_left().get()) &&
        nullable(alt->get_right())) {
      return alt->get_right();
    }

    if (is_equal(alt->get_left(), alt->get_right())) {
      return alt->get_left();
    }
  }

  if (Star * star; star = dynamic_cast<Star *>(reg.get())) {
    if (dynamic_cast<Empty *>(star->get_inside().get()) ||
        dynamic_cast<Epsilon *>(star->get_inside().get())) {
      return star->get_inside();
    }

    if (dynamic_cast<Star *>(star->get_inside().get())) {
      return star->get_inside();
    }
  }

  return reg;
}

static std::shared_ptr<Regex>
derivative(const std::shared_ptr<Char> &left_character,
           const std::shared_ptr<Regex> &reg) {

  // a empty
  if (dynamic_cast<Empty *>(reg.get())) {
    return std::shared_ptr<Regex>(new Empty());
  }

  // a epsilon
  if (dynamic_cast<Epsilon *>(reg.get())) {
    return std::shared_ptr<Regex>(new Empty());
  }

  // a char
  if (Char * right_character;
      right_character = dynamic_cast<Char *>(reg.get())) {
    if (*left_character == *right_character) {
      return std::shared_ptr<Regex>(new Epsilon());
    } else {
      return std::shared_ptr<Regex>(new Empty());
    }
  }

  // a (alt s t)
  if (Alt * alt; alt = dynamic_cast<Alt *>(reg.get())) {
    return optimize(std::shared_ptr<Regex>(
        new Alt(optimize(derivative(left_character, alt->get_left())),
                optimize(derivative(left_character, alt->get_right())))));
  }

  // a (star)
  if (Star * star; star = dynamic_cast<Star *>(reg.get())) {
    return optimize(std::shared_ptr<Regex>(
        new Concat(derivative(left_character, star->get_inside()), reg)));
  }

  // a (concat s t)
  if (Concat * concat; concat = dynamic_cast<Concat *>(reg.get())) {
    return optimize(std::shared_ptr<Regex>(new Alt(
        optimize(std::shared_ptr<Regex>(
            new Concat(derivative(left_character, concat->get_left()),
                       concat->get_right()))),
        optimize(std::shared_ptr<Regex>(new Concat(
            nullable(concat->get_left()) ? std::shared_ptr<Regex>(new Epsilon())
                                         : std::shared_ptr<Regex>(new Empty()),
            derivative(left_character, concat->get_right())))))));
  }
  // throw
  return nullptr;
}

static std::shared_ptr<Regex>
derivative_string(const char *s, const std::shared_ptr<Regex> &reg) {
  if (*s == '\0') {
    return reg;
  }

  return derivative_string(
      s + 1, derivative(std::shared_ptr<Char>(new Char(s[0])), reg));
}

bool Regex::match(const char *s, const std::shared_ptr<Regex> &reg) {
  return nullable(derivative_string(s, reg));
}
