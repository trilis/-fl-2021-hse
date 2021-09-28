#ifndef FL_DERIVATIVE_REGEXPR_H
#define FL_DERIVATIVE_REGEXPR_H

#include <string>
#include <memory>
#include <utility>

enum class type {
    empty,
    epsilon,
    ch,
    star,
    alt,
    concat,
};

struct regexpr {
    type t;

    virtual ~regexpr() = default;
};


struct empty : regexpr {
    empty() {
        t = type::empty;
    }

    ~empty() override = default;
};

struct epsilon : regexpr {
    epsilon() {
        t = type::epsilon;
    }

    ~epsilon() override = default;
};


struct char_ : regexpr {
    char ch;

    explicit char_(char ch_) : ch(ch_) {
        t = type::ch;
    }

    ~char_() override = default;
};

struct star : regexpr {
    std::shared_ptr<regexpr> re;

    explicit star(std::shared_ptr<regexpr> re_) : re(std::move(re_)) {
        t = type::star;
    }

    ~star() override = default;
};

struct alt : regexpr {
    std::shared_ptr<regexpr> l;
    std::shared_ptr<regexpr> r;

    explicit alt(std::shared_ptr<regexpr> l_, std::shared_ptr<regexpr> r_) : r(std::move(r_)), l(std::move(l_)) {
        t = type::alt;
    }

    ~alt() override = default;
};


struct concat : regexpr {
    std::shared_ptr<regexpr> l;
    std::shared_ptr<regexpr> r;

    explicit concat(std::shared_ptr<regexpr> l_, std::shared_ptr<regexpr> r_) : r(std::move(r_)), l(std::move(l_)) {
        t = type::concat;
    }

    ~concat() override = default;
};

bool match(std::shared_ptr<regexpr> re, const std::string& str);


#endif //FL_DERIVATIVE_REGEXPR_H
