#pragma once

#include <iostream>
#include <memory>
#include <type_traits>
#include <cassert>
#include <sstream>
#include <vector>

namespace NRegexp
{
    struct Regexp
    {
        virtual ~Regexp() {}
    };

    using RegPtr = std::shared_ptr<Regexp>;
    using std::dynamic_pointer_cast;
    using std::make_shared;

    struct Empty : Regexp
    {
    };

    struct Epsilon : Regexp
    {
    };

    struct Char : Regexp
    {
        Char(char x) : value(x) {}
        char value;
    };

    struct Star : Regexp
    {
        Star(RegPtr r) : inside(r) {}
        RegPtr inside;
    };

    struct Alt : Regexp
    {
        Alt(RegPtr l, RegPtr r) : left(l), right(r) {}
        RegPtr left;
        RegPtr right;
    };

    struct Concat : Regexp
    {
        Concat(RegPtr l, RegPtr r) : left(l), right(r) {}
        RegPtr left;
        RegPtr right;
    };

    template <class To>
    struct PatternMatchingResult
    {
        static_assert(std::is_base_of<Regexp, To>());
        bool res;
        std::shared_ptr<To> ptr;
        operator bool()
        {
            return res;
        }
    };

    template <class Reg>
    PatternMatchingResult<Reg> PatternMatch(RegPtr reg_ptr)
    {
        auto res = dynamic_pointer_cast<Reg>(reg_ptr);
        return {!!res, res};
    }

    RegPtr Intersect(RegPtr lhs, RegPtr rhs);

    RegPtr Union(RegPtr lhs, RegPtr rhs);

    RegPtr Nullable(RegPtr reg_ptr);

    RegPtr Derivative(char ch, RegPtr regexp_ptr);

    RegPtr Derivative(std::string str, RegPtr reg_ptr);

    bool Match(RegPtr reg_ptr, std::string str);
} // namespace NRegexp