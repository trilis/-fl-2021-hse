#include "my_regexp.h"

regexp::regexp(int type_) : type(type_) {
}

regexp::regexp(int type_, regexp &&first, const regexp &second) : type(type_) {
    if (type_ == alt) {
        if (first.type == empty) {
            *this = second;
        } else if (second.type == empty) {
            *this = first;
        } else if (first.type == epsilon) {
            if (nullable(second)) {
                *this = second;
            }
        } else if (second.type == epsilon) {
            if (nullable(first)) {
                *this = first;
            }
        } else {
            alt_ = {std::make_shared<regexp>(first),
                    std::make_shared<regexp>(second)};
        }
    }
    if (type_ == concat) {
        if (first.type == empty || second.type == empty) {
            type = empty;
        } else if (first.type == epsilon) {
            *this = second;
        } else if (second.type == epsilon) {
            *this = first;
        } else {
            concat_ = {std::make_shared<regexp>(first),
                       std::make_shared<regexp>(second)};
        }
    }
}

regexp::regexp(int type_, regexp &&regexp_)
    : type(type_), star_(std::make_shared<regexp>(regexp_)) {
    if (regexp_.type == empty) {
        *this = regexp(empty);
    }
    if (regexp_.type == epsilon) {
        *this = regexp(epsilon);
    }
    if (regexp_.type == star) {
        *this = regexp_;
    }
}

regexp::regexp(int type_, regexp &&first, regexp &&second) : type(type_) {
    if (type_ == alt) {
        alt_ = {std::make_shared<regexp>(first),
                std::make_shared<regexp>(second)};
    }
    if (type_ == concat) {
        concat_ = {std::make_shared<regexp>(first),
                   std::make_shared<regexp>(second)};
    }
}

regexp::regexp(int type_, char a) : type(type_), sym(a) {
}

[[nodiscard]] bool regexp::nullable(const regexp &regexp_) {
    if (regexp_.type == empty || regexp_.type == char_) {
        return 0;
    }
    if (regexp_.type == epsilon || regexp_.type == star) {
        return 1;
    }
    if (regexp_.type == concat) {
        return nullable(*regexp_.concat_.first) &&
               nullable(*regexp_.concat_.second);
    }
    if (regexp_.type == alt) {
        return nullable(*regexp_.alt_.first) || nullable(*regexp_.alt_.second);
    }
}

[[nodiscard]] regexp regexp::derivative(char a, const regexp &regexp_) {
    if (regexp_.type == epsilon || regexp_.type == empty) {
        return regexp(empty);
    }
    if (regexp_.type == char_) {
        if (regexp_.sym == a) {
            return regexp(epsilon);
        } else {
            return regexp(empty);
        }
    }
    if (regexp_.type == alt) {
        return regexp(alt, std::move(derivative(a, *regexp_.alt_.first)),
                      std::move(derivative(a, *regexp_.alt_.second)));
    }
    if (regexp_.type == star) {
        return regexp(concat, std::move(derivative(a, *regexp_.star_)),
                      regexp_);
    }
    if (regexp_.type == concat) {
        if (nullable(*regexp_.concat_.first)) {
            return regexp(
                alt,
                regexp(concat, std::move(derivative(a, *regexp_.concat_.first)),
                       std::move(*regexp_.concat_.second)),
                regexp(concat, regexp(epsilon),
                       std::move(derivative(a, *regexp_.concat_.second))));
        } else {
            return regexp(
                alt,
                regexp(concat, std::move(derivative(a, *regexp_.concat_.first)),
                       std::move(*regexp_.concat_.second)),
                regexp(concat, regexp(empty),
                       std::move(derivative(a, *regexp_.concat_.second))));
        }
    }
}

[[nodiscard]] regexp regexp::derivative(std::string &&s, regexp regexp_) {
    if (s.empty()) {
        return regexp_;
    }
    return derivative(s.substr(1, s.size() - 1), derivative(s[0], regexp_));
}

[[nodiscard]] bool match(const regexp &regexp_, std::string &&s) {
    return regexp::nullable(regexp::derivative(std::move(s), regexp_));
}
