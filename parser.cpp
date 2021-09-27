#include "parser.h"
#include <memory>
#include <cstring>

Char::Char(char value): value(value) {}

bool Char::operator==(const Char& other) const {
    return value == other.value;
};

Star::Star(Regex *inside): inside(inside) {}
Star::Star(std::shared_ptr<Regex> inside): inside(inside) {}

std::shared_ptr<Regex> Star::get_inside() const {
    return inside; 
}

/////////////////////////
Alt::Alt(Regex* left,  Regex* right): left(left), right(right) {}
Alt::Alt(std::shared_ptr<Regex> left, std::shared_ptr<Regex> right): left(left), right(right) {}

std::shared_ptr<Regex> Alt::get_left() const {
    return left;
}

std::shared_ptr<Regex> Alt::get_right() const {
    return right;
}

Concat::Concat(Regex* left,  Regex* right): left(left), right(right) {}
Concat::Concat(std::shared_ptr<Regex> left, std::shared_ptr<Regex> right): left(left), right(right) {}

std::shared_ptr<Regex> Concat::get_left() const {
    return left;
}

std::shared_ptr<Regex> Concat::get_right() const {
    return right;
}
/////////////////////////


static std::shared_ptr<Regex> intersect_regex(std::shared_ptr<Regex> left, std::shared_ptr<Regex> right) {
    
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

static std::shared_ptr<Regex> nullable(std::shared_ptr<Regex> reg) {
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


static std::shared_ptr<Regex> derivative(std::shared_ptr<Char> left_character, std::shared_ptr<Regex> reg) {
    
    // a empty
    if (dynamic_cast<Empty*>(reg.get())) {
        return std::shared_ptr<Regex>(new Empty());
    }
    
    // a epsilon
    if (dynamic_cast<Epsilon*>(reg.get())) {
        return std::shared_ptr<Regex>(new Empty());
    }

    // a char
    if (Char* right_character; right_character = dynamic_cast<Char*>(reg.get())) {
        if (*left_character == *right_character) {
            return std::shared_ptr<Regex>(new Epsilon());
        }
        else {
            return std::shared_ptr<Regex>(new Empty());
        }
    }

    // a (alt s t)
    if (Alt* alt; alt = dynamic_cast<Alt*>(reg.get())) {
        return std::shared_ptr<Regex>(new Alt(derivative(left_character, alt->get_left()), derivative(left_character, alt->get_right())));
    }

    // a (star) 
    if (Star* star; star = dynamic_cast<Star*>(reg.get())) {
        return std::shared_ptr<Regex>(new Concat(derivative(left_character, star->get_inside()), reg));
    }

    // a (concat s t)
    if (Concat* concat; concat = dynamic_cast<Concat*>(reg.get())) {
        return std::shared_ptr<Regex>(new Alt(new Concat(derivative(left_character, concat->get_left()), concat->get_right()), new Concat(nullable(concat->get_left()), derivative(left_character, concat->get_right()))));
    }
    // throw
    return nullptr;
}

static std::shared_ptr<Regex> derivative_string(const char* s, std::shared_ptr<Regex> reg) {
    if (*s == '\0') {
        return reg;
    }

    return derivative_string(s + 1, derivative(std::shared_ptr<Char>(new Char(s[0])), reg));

}


bool Regex::match(const char* s, std::shared_ptr<Regex> reg) {
    std::shared_ptr<Regex> answer = nullable(derivative_string(s, reg));
    return dynamic_cast<Epsilon*>(answer.get());
}