#ifndef FL3_MY_REGEXP_H
#define FL3_MY_REGEXP_H
#include <memory>
enum { empty, epsilon, char_, star, alt, concat };
class regexp {
    int type = empty;
    char sym = 0;
    std::shared_ptr<regexp> star_ = nullptr;
    std::pair<std::shared_ptr<regexp>, std::shared_ptr<regexp>> alt_ = {
        nullptr, nullptr};
    std::pair<std::shared_ptr<regexp>, std::shared_ptr<regexp>> concat_ = {
        nullptr, nullptr};
    static bool nullable(const regexp &regexp_);
    static regexp derivative(char a, const regexp &regexp_);
    static regexp derivative(std::string &&s, regexp regexp_);

public:
    explicit regexp(int type_);
    explicit regexp(int type_, regexp &&regexp_);
    explicit regexp(int type_, regexp &&first, regexp &&second);
    explicit regexp(int type_, char a);
    explicit regexp(int type_, regexp &&first, const regexp &second);
    friend bool match(const regexp &regexp_, std::string &&s);
};

#endif  // FL3_MY_REGEXP_H
