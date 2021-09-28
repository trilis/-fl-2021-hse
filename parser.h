#ifndef HW_PARSER_H
#define HW_PARSER_H

#endif //HW_PARSER_H

enum Type {
    EMPTY,
    EPSILON,
    CHAR,
    STAR,
    ALT,
    CONCAT
};

struct Regexp {
    Regexp(Type type1): type(type1) {}
    Regexp(Type type1, std::shared_ptr<Regexp> fst, std::shared_ptr<Regexp> snd): type(type1), first(fst), second(snd) {}
    Regexp(Type type1, std::shared_ptr<Regexp> fst): type(type1), first(fst) {}
    Regexp(char ch): type(CHAR), Char(ch) {}
    Type type;
    char Char;
    std::shared_ptr<Regexp> first;
    std::shared_ptr<Regexp> second;
};

bool match(const std::string& str, const std::shared_ptr<Regexp>& regx);