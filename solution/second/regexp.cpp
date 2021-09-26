#include "regexp.hpp"
#include <memory>
#include <type_traits>
#include <cassert>
#include <sstream>
#include <vector>

namespace NRegexp
{

    RegPtr Intersect(RegPtr lhs, RegPtr rhs)
    {
        if (PatternMatch<Empty>(lhs) || PatternMatch<Empty>(rhs))
        {
            return make_shared<Empty>();
        }
        if (PatternMatch<Epsilon>(lhs) && PatternMatch<Epsilon>(rhs))
        {
            return make_shared<Epsilon>();
        }
        std::stringstream ss;
        ss << __FILE__ << ":" << __LINE__ << " Impossible to intersect these regexes\n";
        throw std::runtime_error(ss.str());
    }

    RegPtr Union(RegPtr lhs, RegPtr rhs)
    {
        if (PatternMatch<Epsilon>(lhs) || PatternMatch<Epsilon>(rhs))
        {
            return make_shared<Epsilon>();
        }
        if (PatternMatch<Empty>(lhs) && PatternMatch<Empty>(rhs))
        {
            return make_shared<Empty>();
        }
        std::stringstream ss;
        ss << __FILE__ << ":" << __LINE__ << " Impossible to unite these regexes\n";
        throw std::runtime_error(ss.str());
    }
    bool Nullable(RegPtr reg_ptr) // does reg_ptr match epsilon?
    {
        if (PatternMatch<Empty>(reg_ptr))
        {
            return false;
        }
        if (PatternMatch<Epsilon>(reg_ptr))
        {
            return true;
        }
        if (PatternMatch<Char>(reg_ptr))
        {
            return false;
        }
        if (auto matched = PatternMatch<Concat>(reg_ptr))
        {
            return Nullable(matched.ptr->left) && Nullable(matched.ptr->right);
        }
        if (auto matched = PatternMatch<Alt>(reg_ptr))
        {
            return Nullable(matched.ptr->left) || Nullable(matched.ptr->right);
        }
        if (PatternMatch<Star>(reg_ptr))
        {
            return true;
        }
        std::stringstream ss;
        ss << __FILE__ << ":" << __LINE__ << " Regex didn't match any pattern\n";
        throw std::runtime_error(ss.str());
    }

    RegPtr Derivative(char ch, RegPtr regexp_ptr)
    {
        if (PatternMatch<Empty>(regexp_ptr)) // Empty
        {
            return make_shared<Empty>();
        }
        if (PatternMatch<Epsilon>(regexp_ptr)) // Epsilon
        {
            return make_shared<Empty>();
        }
        if (auto matched = PatternMatch<Char>(regexp_ptr)) // Char
        {
            if (ch == matched.ptr->value)
            {
                return make_shared<Epsilon>();
            }
            return make_shared<Empty>();
        }
        if (auto matched = PatternMatch<Star>(regexp_ptr)) // Star
        {
            auto casted_ptr = matched.ptr;
            return make_shared<Concat>(Derivative(ch, casted_ptr->inside), casted_ptr);
        }
        if (auto matched = PatternMatch<Alt>(regexp_ptr)) // Alt
        {
            auto casted_ptr = matched.ptr;
            return make_shared<Alt>(Derivative(ch, casted_ptr->left), Derivative(ch, casted_ptr->right));
        }
        if (auto matched = PatternMatch<Concat>(regexp_ptr)) // Concat
        {
            auto casted_ptr = matched.ptr;
            auto left = make_shared<Concat>(Derivative(ch, casted_ptr->left), casted_ptr->right);
            RegPtr right = nullptr;

            if (Nullable(casted_ptr->left))
            {
                right = Derivative(ch, casted_ptr->right);
            }
            else
            {
                right = make_shared<Empty>();
            }

            return make_shared<Alt>(left, right);
        }
        std::stringstream ss;
        ss << __FILE__ << ":" << __LINE__ << " Regex didn't match any pattern\n";
        throw std::runtime_error(ss.str());
    }

    RegPtr Derivative(std::string str, RegPtr reg_ptr)
    {
        if (str.empty())
        {
            return reg_ptr;
        }
        char head = str[0];
        auto tail = std::string(str.begin() + 1, str.end());
        return Derivative(tail, Derivative(head, reg_ptr));
    }

    bool Match(RegPtr reg_ptr, std::string str)
    {
        return Nullable(Derivative(str, reg_ptr));
    }
} // namespace NRegexp