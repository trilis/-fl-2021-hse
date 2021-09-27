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

    std::shared_ptr<Regexp> intersect(std::shared_ptr<Regexp> a, std::shared_ptr<Regexp> b) {
        if (verify_type<Empty>(a) || verify_type<Empty>(b)) {
            return std::make_shared<Empty>();
        }
        return std::make_shared<Epsilon>();
    };

    std::shared_ptr<Regexp> unite(std::shared_ptr<Regexp> a, std::shared_ptr<Regexp> b) {
        if (verify_type<Empty>(a) && verify_type<Empty>(b)) {
            return std::make_shared<Empty>();
        }
        return std::make_shared<Epsilon>();
    }

    std::shared_ptr<Regexp> nullable(std::shared_ptr<Regexp> r) {
        if (verify_type<Empty>(r))
            return std::make_shared<Empty>();
        if (verify_type<Epsilon>(r))
            return std::make_shared<Epsilon>();
        if (verify_type<Symb>(r))
            return std::make_shared<Empty>();
        if (verify_type<Star>(r))
            return std::make_shared<Epsilon>();
        if (verify_type<Alt>(r)) {
            auto alt_reg = dynamic_cast<Alt *>(r.get());
            return unite(nullable(alt_reg->arg1), nullable(alt_reg->arg2));
        }
        if (verify_type<Concat>(r)) {
            auto concat_reg = dynamic_cast<Concat *>(r.get());
            return intersect(nullable(concat_reg->arg1), nullable(concat_reg->arg2));
        }
        return nullptr;
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
            return std::make_shared<Alt>(std::make_shared<Concat>(der(c, concat_reg->arg1), concat_reg->arg2),
                                         std::make_shared<Concat>(nullable(concat_reg->arg1),
                                                                  der(c, concat_reg->arg2)));
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
        return verify_type<Epsilon>(nullable(derivative(s, reg)));
    }
}