#include <string>
#include <memory>
#include "parser.h"

std::shared_ptr<Regexp> nullable(const std::shared_ptr<Regexp>& regx);
std::shared_ptr<Regexp> derivative_str(const std::string& str, const std::shared_ptr<Regexp>& regx);

bool match(const std::string& str, const std::shared_ptr<Regexp>& regx) {
    return (nullable(derivative_str(str, regx))->type == EPSILON);
};

std::shared_ptr<Regexp> derivative(char ch, const std::shared_ptr<Regexp>& regx) {
    if (regx->type == EMPTY || regx->type == EPSILON) {
        return std::make_shared<Regexp>(EMPTY);
    }
    if (regx->type == CHAR) {
        if (regx->Char == ch) { return std::make_shared<Regexp>(EPSILON); }
        return std::make_shared<Regexp>(EMPTY);
    }
    if (regx->type == ALT) {
        return std::make_shared<Regexp>(ALT, derivative(ch, regx->first), derivative(ch, regx->second));
    }
    if (regx->type == STAR) {
        return std::make_shared<Regexp>(CONCAT, derivative(ch, regx->first), std::make_shared<Regexp>(STAR, regx->first));
    }
    if (regx->type == CONCAT) {
        return std::make_shared<Regexp>(ALT, std::make_shared<Regexp>(CONCAT, derivative(ch, regx->first), regx->second),
                                        std::make_shared<Regexp>(CONCAT, nullable(regx->first), derivative(ch, regx->second)));
    }
}

std::shared_ptr<Regexp> intersect(const std::shared_ptr<Regexp>& regx1, const std::shared_ptr<Regexp>& regx2) {
    if (regx1->type == EMPTY || regx2->type == EMPTY) {
        return std::make_shared<Regexp>(EMPTY);
    }
    if (regx1->type == EPSILON && regx2->type == EPSILON) {
        return std::make_shared<Regexp>(EPSILON);
    }
}

std::shared_ptr<Regexp> unions(const std::shared_ptr<Regexp>& regx1, const std::shared_ptr<Regexp>& regx2) {
    if (regx1->type == EPSILON || regx2->type == EPSILON) {
        return std::make_shared<Regexp>(EPSILON);
    }
    if (regx1->type == EMPTY && regx2->type == EMPTY) {
        return std::make_shared<Regexp>(EMPTY);
    }
}

std::shared_ptr<Regexp> nullable(const std::shared_ptr<Regexp>& regx) {
    if (regx->type == EMPTY || regx->type == CHAR) {
        return std::make_shared<Regexp>(EMPTY);
    }
    if (regx->type == EPSILON || regx->type == STAR) {
        return std::make_shared<Regexp>( EPSILON);
    }
    if (regx->type == CONCAT) {
        return intersect(nullable(regx->first), nullable(regx->second));
    }
    if (regx->type == ALT) {
        return unions(nullable(regx->first), nullable(regx->second));
    }
}

std::shared_ptr<Regexp> derivative_str(const std::string& str, const std::shared_ptr<Regexp>& regx) {
    if (str.empty()) { return regx; }
    return derivative_str(str.substr(1), derivative(str[0], regx));
}


