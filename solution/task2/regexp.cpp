#include <iostream>
#include <memory>
#include <cassert>
#include <type_traits>
#include "regexp.h"

namespace Reg {

    Regexp::~Regexp() = default;

    Empty::~Empty() = default;

    Epsilon::~Epsilon() = default;

    Symb::Symb(char symb_) : symb(symb_) {}

    Symb::~Symb() = default;

    Star::Star(std::shared_ptr<Regexp> arg_) : arg(std::move(arg_)) {}


    Alt::Alt(std::shared_ptr<Regexp> arg1_, std::shared_ptr<Regexp> arg2_) : arg1(std::move(arg1_)),
                                                                             arg2(std::move(arg2_)) {}

    Alt::~Alt() = default;

    Concat::Concat(std::shared_ptr<Regexp> arg1_, std::shared_ptr<Regexp> arg2_) : arg1(std::move(arg1_)),
                                                                                   arg2(std::move(arg2_)) {}

    Concat::~Concat() = default;

    template<class type>
    bool verify_type(std::shared_ptr<Regexp> r) {
        return std::dynamic_pointer_cast<type>(r) != nullptr;
    }

    bool nullable(std::shared_ptr<Regexp> r) {
        if (verify_type<Empty>(r))
            return false;
        if (verify_type<Epsilon>(r))
            return true;
        if (verify_type<Symb>(r))
            return false;
        if (verify_type<Star>(r))
            return true;
        if (verify_type<Alt>(r)) {
            auto alt_reg = dynamic_cast<Alt *>(r.get());
            return nullable(alt_reg->arg1) || nullable(alt_reg->arg2);
        }
        if (verify_type<Concat>(r)) {
            auto concat_reg = dynamic_cast<Concat *>(r.get());
            return nullable(concat_reg->arg1) && nullable(concat_reg->arg2);
        }
        return false;
    }


    std::shared_ptr<Regexp> der(char c, std::shared_ptr<Regexp> lang) {
        if (verify_type<Empty>(lang))
            return std::make_shared<Empty>();
        if (verify_type<Epsilon>(lang))
            return std::make_shared<Empty>();
        if (verify_type<Symb>(lang)) {
            auto symb_reg = dynamic_cast<Symb *>(lang.get());
            if (c == symb_reg->symb) {
                return std::make_shared<Epsilon>();
            }
            return std::make_shared<Empty>();
        }
        if (verify_type<Alt>(lang)) {
            auto alt_reg = dynamic_cast<Alt *>(lang.get());
            return std::make_shared<Alt>(der(c, alt_reg->arg1), der(c, alt_reg->arg2));
        }
        if (verify_type<Star>(lang)) {
            auto star_reg = dynamic_cast<Star *>(lang.get());
            return std::make_shared<Concat>(der(c, star_reg->arg), std::make_shared<Star>(star_reg->arg));
        }
        if (verify_type<Concat>(lang)) {
            auto concat_reg = dynamic_cast<Concat *>(lang.get());
            auto alt1 = std::make_shared<Concat>(der(c, concat_reg->arg1), concat_reg->arg2);
            std::shared_ptr<Regexp> alt2 = nullptr;
            if (nullable(concat_reg->arg1)) {
                alt2 = der(c, concat_reg->arg2);
            } else {
                alt2 = std::make_shared<Empty>();
            }
            return std::make_shared<Alt>(alt1, alt2);
        }
        return nullptr;
    }

    std::shared_ptr<Regexp> derivative(const std::string &s, std::shared_ptr<Regexp> lang) {
        if (s.empty()) {
            return lang;
        }
        std::string tail = std::string(std::next(s.begin()), s.end());
        return derivative(tail, der(s[0], lang));
    }

    bool match(std::shared_ptr<Regexp> reg, const std::string &s) {
        return nullable(derivative(s, reg));
    }

    // optimization # 2

    std::shared_ptr<Regexp> concat(std::shared_ptr<Regexp> a, std::shared_ptr<Regexp> b) {
        if (verify_type<Empty>(a) || verify_type<Empty>(b))
            return std::make_shared<Empty>();
        if (verify_type<Epsilon>(a))
            return b;
        if (verify_type<Epsilon>(b))
            return a;
        return std::make_shared<Concat>(a, b);
    }

    std::shared_ptr<Regexp> alt(std::shared_ptr<Regexp> a, std::shared_ptr<Regexp> b) {
        if (verify_type<Empty>(a))
            return b;
        if (verify_type<Empty>(b))
            return a;
        if (verify_type<Epsilon>(b) && nullable(a))
            return a;
        if (verify_type<Epsilon>(a) && nullable(b))
            return b;
        if (a == b)
            return a;
        return std::make_shared<Alt>(a, b);
    }

    std::shared_ptr<Regexp> star(std::shared_ptr<Regexp> a) {
        if (verify_type<Empty>(a))
            return std::make_shared<Empty>();
        if (verify_type<Epsilon>(a))
            return std::make_shared<Epsilon>();
        if (verify_type<Star>(a)) {
            auto cast = std::dynamic_pointer_cast<Star>(a);
            return star(cast->arg);
        }
        return std::make_shared<Star>(a);
    }

    std::shared_ptr<Epsilon> epsilon() {
        return std::make_shared<Epsilon>();
    }

    std::shared_ptr<Empty> empty() {
        return std::make_shared<Empty>();
    }

    std::shared_ptr<Symb> symb(char c) {
        return std::make_shared<Symb>(c);
    }
}