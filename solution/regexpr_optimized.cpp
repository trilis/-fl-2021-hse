#include "regexpr.h"
#include <string>
#include <memory>

bool nullable(const std::shared_ptr<regexpr>& re);


std::shared_ptr<regexpr> make_star(std::shared_ptr<regexpr> re) {
    if (re->t == type::empty) {
        return std::make_shared<empty>();
    }
    if (re->t == type::epsilon) {
        return std::make_shared<epsilon>();
    }
    if (re->t == type::star) {
        return re;
    }
    return std::make_shared<star>(re);
}

std::shared_ptr<regexpr> make_alt(std::shared_ptr<regexpr> l, std::shared_ptr<regexpr> r) {
    if (r->t == type::empty) {
        return l;
    }
    if (l->t == type::empty) {
        return r;
    }
    if (l->t == type::epsilon) {
        if (nullable(r)) {
            return r;
        }
    }
    if (r->t == type::epsilon) {
        if (nullable(l)) {
            return l;
        }
    }
    if (l == r) {
        return l;
    }
    return std::make_shared<alt>(l, r);
}

std::shared_ptr<regexpr> make_concat(std::shared_ptr<regexpr> l, std::shared_ptr<regexpr> r) {
    if (l->t == type::empty || r->t == type::empty) {
        return std::make_shared<empty>();
    }
    if (l->t == type::epsilon) {
        return r;
    }
    if (r->t == type::epsilon) {
        return l;
    }
    return std::make_shared<concat>(l, r);
}

bool union_(bool first, bool second) {
    if (!first && !second) {
        return false;
    }
    return true;
}

bool intersect(bool first, bool second) {
    if (first && second) {
        return true;
    }
    return false;
}

bool nullable(const std::shared_ptr<regexpr>& re) {
    if (re->t == type::empty) {
        return false;
    }
    if (re->t == type::epsilon) {
        return true;
    }
    if (re->t == type::ch) {
        return false;
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
        return true;
    }
    return false;
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
        auto *c = dynamic_cast<char_ *>(re.get());
        if (ch == c->ch) {
            epsilon e;
            return std::make_shared<epsilon>(e);
        }
        empty e;
        return std::make_shared<empty>(e);
    }
    if (re->t == type::concat) {
        auto *c = dynamic_cast<concat *>(re.get());
        if (nullable(c->l)) {
            return make_alt(make_concat(derivative(ch, c->l), c->r),
                            derivative(ch, c->r));
        } else {
            return make_alt(make_concat(derivative(ch, c->l), c->r),
                            std::make_shared<empty>());
        }
    }
    if (re->t == type::alt) {
        alt *a = dynamic_cast<alt *>(re.get());
        return make_alt(derivative(ch, a->l), derivative(ch, a->r));
    }
    if (re->t == type::star) {
        star *s = dynamic_cast<star *>(re.get());
        return make_concat(derivative(ch, s->re), make_star(s->re));
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
    return nullable(derivative_s(str, re));
}
