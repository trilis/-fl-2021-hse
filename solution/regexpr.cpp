#include "regexpr.h"
#include <string>
#include <memory>


std::shared_ptr<regexpr> union_(const std::shared_ptr<regexpr> &l, const std::shared_ptr<regexpr> &r) {
    if (l->t == type::epsilon || r->t == type::epsilon) {
        epsilon e;
        return std::make_shared<epsilon>(e);
    }
    if (l->t == type::empty && r->t == type::empty) {
        empty e;
        return std::make_shared<empty>(e);
    }
}

std::shared_ptr<regexpr> intersect(const std::shared_ptr<regexpr> &l, const std::shared_ptr<regexpr> &r) {
    if (l->t == type::empty || r->t == type::empty) {
        empty e;
        return std::make_shared<empty>(e);
    }
    if (l->t == type::epsilon && r->t == type::epsilon) {
        epsilon e;
        return std::make_shared<epsilon>(e);
    }
}

std::shared_ptr<regexpr> nullable(const std::shared_ptr<regexpr>& re) {
    if (re->t == type::empty) {
        empty e;
        return std::make_shared<empty>(e);
    }
    if (re->t == type::epsilon) {
        epsilon e;
        return std::make_shared<epsilon>(e);
    }
    if (re->t == type::ch) {
        empty e;
        return std::make_shared<empty>(e);
    }
    if (re->t == type::concat) {
        auto *c = dynamic_cast<concat *>(re.get());
        return intersect(nullable(c->l), nullable(c->r));
    }
    if (re->t == type::alt) {
        alt *a = dynamic_cast<alt *>(re.get());
        return union_(nullable(a->l), nullable(a->r));
    }
    if (re->t == type::star) {
        epsilon e;
        return std::make_shared<epsilon>(e);
    }
}

std::shared_ptr<regexpr> derivative(char ch, const std::shared_ptr<regexpr>& re) {
    if (re->t == type::empty) {
        empty e;
        return std::make_shared<empty>(e);
    }
    if (re->t == type::epsilon) {
        empty e;
        return std::make_shared<empty>(e);
    }
    if (re->t == type::ch) {
        auto c = dynamic_cast<char_ *>(re.get());
        if (ch == c->ch) {
            epsilon e;
            return std::make_shared<epsilon>(e);
        }
        empty e;
        return std::make_shared<empty>(e);
    }
    if (re->t == type::concat) {
        auto c = dynamic_cast<concat *>(re.get());
        return std::make_shared<alt>(std::make_shared<concat>(derivative(ch, c->l), c->r),
                                     std::make_shared<concat>(nullable(c->l), derivative(ch, c->r)));
    }
    if (re->t == type::alt) {
        alt *a = dynamic_cast<alt *>(re.get());
        return std::make_shared<alt>(derivative(ch, a->l), derivative(ch, a->r));
    }
    if (re->t == type::star) {
        star *s = dynamic_cast<star *>(re.get());
        return std::make_shared<concat>(derivative(ch, s->re), std::make_shared<star>(*s));
    }
}

std::shared_ptr<regexpr> derivative_s(const std::string &str, std::shared_ptr<regexpr> re) {
    if (str.empty()) {
        return re;
    } else {
        return derivative_s(str.substr(1), derivative(str[0], re));
    }
}


bool match(std::shared_ptr<regexpr> re, const std::string& str) {
    return nullable(derivative_s(str, re))->t == type::epsilon;
}
