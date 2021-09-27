#include "parser.h"
#include <memory>
#include <cstring>

Char::Char(char value): value(value) {}

bool Char::operator==(const Char& other) const {
    return value == other.value;
};

Star::Star(Regex *inside): inside(inside) {}
Star::Star(const std::shared_ptr<Regex>& inside): inside(inside) {}

std::shared_ptr<Regex> Star::get_inside() const {
    return inside; 
}

std::shared_ptr<Regex> BinaryOperator::get_left() const {
    return left;
}

std::shared_ptr<Regex> BinaryOperator::get_right() const {
    return right;
}

BinaryOperator::BinaryOperator(Regex* left,  Regex* right): left(left), right(right) {}
BinaryOperator::BinaryOperator(const std::shared_ptr<Regex>& left, const std::shared_ptr<Regex>& right): left(left), right(right) {}

Alt::Alt(Regex* left,  Regex* right): BinaryOperator(left, right) {}
Alt::Alt(const std::shared_ptr<Regex>& left, const std::shared_ptr<Regex>& right): BinaryOperator(left, right) {}

Concat::Concat(Regex* left,  Regex* right): BinaryOperator(left, right) {}
Concat::Concat(const std::shared_ptr<Regex>& left, const std::shared_ptr<Regex>& right): BinaryOperator(left, right) {}


static std::shared_ptr<Regex> intersect_regex(const std::shared_ptr<Regex>& left, const std::shared_ptr<Regex>& right) {
    
    if (dynamic_cast<Empty*>(left.get()) || dynamic_cast<Empty*>(right.get())) {
        return std::shared_ptr<Regex>(new Empty());
    }

    if (dynamic_cast<Epsilon*>(left.get()) && dynamic_cast<Epsilon*>(right.get())) {
        return std::shared_ptr<Regex>(new Epsilon());
    }

    // throw
    return nullptr;
}



static std::shared_ptr<Regex> union_regex(std::shared_ptr<Regex> left, std::shared_ptr<Regex> right) {
    
    if (dynamic_cast<Epsilon*>(left.get()) || dynamic_cast<Epsilon*>(right.get())) {
        return std::shared_ptr<Regex>(new Epsilon());
    }

    if (dynamic_cast<Empty*>(left.get()) && dynamic_cast<Empty*>(right.get())) {
        return std::shared_ptr<Regex>(new Empty());
    }

    // throw
    return nullptr;
}

static std::shared_ptr<Regex> nullable(const std::shared_ptr<Regex>& reg) {
    if (dynamic_cast<Empty*>(reg.get())) {
        return std::shared_ptr<Regex>(new Empty());
    }
    
    if (dynamic_cast<Epsilon*>(reg.get())) {
        return std::shared_ptr<Regex>(new Epsilon());
    }

    if (dynamic_cast<Char*>(reg.get())) {
        return std::shared_ptr<Regex>(new Empty());
    }

    if (Concat* concat; concat = dynamic_cast<Concat*>(reg.get())) {
        return std::shared_ptr<Regex>(intersect_regex(nullable(concat->get_left()), nullable(concat->get_right())));
    }

    if (Alt* alt; alt = dynamic_cast<Alt*>(reg.get())) {
        return std::shared_ptr<Regex>(union_regex(nullable(alt->get_left()), nullable(alt->get_right())));
    }

    if (dynamic_cast<Star*>(reg.get())) {
        return std::shared_ptr<Regex>(new Epsilon());
    }

    //throw

    return nullptr;
}



static std::shared_ptr<Regex> optimize(const std::shared_ptr<Regex>& reg) {
    if (Concat* concat; concat = dynamic_cast<Concat*>(reg.get())) {
        if (dynamic_cast<Empty*>(concat->get_left().get()) || 
            dynamic_cast<Empty*>(concat->get_right().get()) ) {
                return std::shared_ptr<Regex>(new Empty());
        }

        if (dynamic_cast<Epsilon*>(concat->get_left().get()) || 
            dynamic_cast<Epsilon*>(concat->get_right().get()) ) {
                return std::shared_ptr<Regex>(new Epsilon());
        }
    }

    if (Alt* alt; alt = dynamic_cast<Alt*>(reg.get())) {
        if (dynamic_cast<Empty*>(alt->get_left().get())) {
            return std::shared_ptr<Regex>(alt->get_right());
        }

        if (dynamic_cast<Empty*>(alt->get_right().get())) {
            return std::shared_ptr<Regex>(alt->get_left());
        }

        if (dynamic_cast<Epsilon*>(alt->get_left().get()) && dynamic_cast<Epsilon*>(nullable(alt->get_left()).get())) {
            return std::shared_ptr<Regex>(alt->get_left());
        }

        if (dynamic_cast<Epsilon*>(alt->get_right().get()) && dynamic_cast<Epsilon*>(nullable(alt->get_right()).get())) {
            return std::shared_ptr<Regex>(alt->get_right());
        }
    }

    if (Star* star; star = dynamic_cast<Star*>(reg.get())) {
        if (dynamic_cast<Empty*>(star->get_inside().get())) {
            return std::shared_ptr<Regex>(new Empty());
        }
        if (dynamic_cast<Epsilon*>(star->get_inside().get())) {
            return std::shared_ptr<Regex>(new Epsilon());
        }
        if (dynamic_cast<Star*>(star->get_inside().get())) {
            return std::shared_ptr<Regex>(star->get_inside());
        }
    }
    
    return reg;
}

std::shared_ptr<Regex> optimize(Regex *reg) {
    return optimize(reg);
}


static std::shared_ptr<Regex> derivative(const std::shared_ptr<Char>& left_character, const std::shared_ptr<Regex>& reg) {
    
    // a empty
    if (dynamic_cast<Empty*>(reg.get())) {
        return optimize(std::shared_ptr<Regex>(new Empty()));
    }
    
    // a epsilon
    if (dynamic_cast<Epsilon*>(reg.get())) {
        return optimize(std::shared_ptr<Regex>(new Empty()));
    }

    // a char
    if (Char* right_character; right_character = dynamic_cast<Char*>(reg.get())) {
        if (*left_character == *right_character) {
            return optimize(std::shared_ptr<Regex>(new Epsilon()));
        }
        else {
            return optimize(std::shared_ptr<Regex>(new Empty()));
        }
    }

    // a (alt s t)
    if (Alt* alt; alt = dynamic_cast<Alt*>(reg.get())) {
        return optimize(new Alt(optimize(derivative(left_character, optimize(alt->get_left()))), optimize(derivative(left_character, optimize(alt->get_right())))));
    }

    // a (star) 
    if (Star* star; star = dynamic_cast<Star*>(reg.get())) {
        return optimize(new Concat(optimize(derivative(left_character, optimize(star->get_inside()))), reg));
    }

    // a (concat s t)
    if (Concat* concat; concat = dynamic_cast<Concat*>(reg.get())) {
        return optimize(new Alt(optimize(new Concat(optimize(derivative(left_character, optimize(concat->get_left()))), optimize(concat->get_right()))), optimize(new Concat(nullable(concat->get_left()), optimize(derivative(left_character, optimize(concat->get_right())))))));
    }
    // throw
    return nullptr;
}

static std::shared_ptr<Regex> derivative_string(const char* s, const std::shared_ptr<Regex>& reg) {
    if (*s == '\0') {
        return optimize(reg);
    }

    return optimize(derivative_string(s + 1, optimize(derivative(std::shared_ptr<Char>(new Char(s[0])), reg))));

}


bool Regex::match(const char* s, const std::shared_ptr<Regex>& reg) {
    std::shared_ptr<Regex> answer = nullable(derivative_string(s, reg));
    return dynamic_cast<Epsilon*>(answer.get());
}
