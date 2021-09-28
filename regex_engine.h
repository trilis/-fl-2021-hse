//
// Created by egor on 27.09.2021.
//

#ifndef FL_2021_HSE_REGEX_ENGINE_H
#define FL_2021_HSE_REGEX_ENGINE_H

namespace regex_engine {

    struct empty_label_t {
    };
    struct epsilon_label_t {
    };

    struct abstract_regex {
        virtual std::shared_ptr<abstract_regex> nullable() = 0;

        virtual std::shared_ptr<abstract_regex> derivative(char c) = 0;

        virtual bool is_empty() const { return false; }

        virtual bool is_epsilon() const { return false; }
    };

    struct empty_regex : abstract_regex {
        // In case of SFINAE workarounds
        empty_label_t empty_label;

        std::shared_ptr<abstract_regex> nullable() override;

        std::shared_ptr<abstract_regex> derivative(char c) override;

        bool is_empty() const override;
    };

    struct epsilon_regex : abstract_regex {
        // In case of SFINAE workarounds
        epsilon_label_t epsilon_label;

        std::shared_ptr<abstract_regex> nullable() override;

        std::shared_ptr<abstract_regex> derivative(char c) override;

        bool is_epsilon() const override;
    };

    std::shared_ptr<abstract_regex>
    intersect(std::shared_ptr<abstract_regex> const &lhs, std::shared_ptr<abstract_regex> const &rhs);

    std::shared_ptr<abstract_regex>
    unite(std::shared_ptr<abstract_regex> const &lhs, std::shared_ptr<abstract_regex> const &rhs);

    struct concat_regex : abstract_regex {
        std::shared_ptr<abstract_regex> concat1, concat2;

        concat_regex(std::shared_ptr<abstract_regex> concat1, std::shared_ptr<abstract_regex> concat2);

        std::shared_ptr<abstract_regex> nullable() override;

        std::shared_ptr<abstract_regex> derivative(char c) override;
    };

    struct star_regex : abstract_regex {
        std::shared_ptr<abstract_regex> regex;

        explicit star_regex(std::shared_ptr<abstract_regex> regex);

        std::shared_ptr<abstract_regex> nullable() override;

        std::shared_ptr<abstract_regex> derivative(char c) override;
    };

    struct alt_regex : abstract_regex {
        std::shared_ptr<abstract_regex> alt1, alt2;

        alt_regex(std::shared_ptr<abstract_regex> alt1, std::shared_ptr<abstract_regex> alt2);

        std::shared_ptr<abstract_regex> nullable() override;

        std::shared_ptr<abstract_regex> derivative(char c) override;
    };

    struct char_regex : abstract_regex {
        char value = '\0';

        explicit char_regex(char c);

        std::shared_ptr<abstract_regex> nullable() override;

        std::shared_ptr<abstract_regex> derivative(char c) override;
    };

    std::shared_ptr<abstract_regex> derivative_strike(std::string &pattern, std::shared_ptr<abstract_regex> regex) {
        if (pattern.empty()) {
            return regex;
        }
        char front_char = pattern.back();
        pattern.pop_back();
        return derivative_strike(pattern, regex->derivative(front_char));
    }

    std::shared_ptr<abstract_regex> match_internal(std::string &pattern, std::shared_ptr<abstract_regex> regex) {
        return derivative_strike(pattern, std::move(regex))->nullable();
    }

    bool match(std::string pattern, std::shared_ptr<abstract_regex> regex) {
        std::reverse(std::begin(pattern), std::end(pattern));
        return match_internal(pattern, std::move(regex))->is_epsilon();
    }

    std::shared_ptr<abstract_regex>
    concat(std::shared_ptr<abstract_regex> const &lhs, std::shared_ptr<abstract_regex> const &rhs);

    std::shared_ptr<abstract_regex>
    either(std::shared_ptr<abstract_regex> const &lhs, std::shared_ptr<abstract_regex> const &rhs);

    std::shared_ptr<abstract_regex> many(std::shared_ptr<abstract_regex> const &regex);

    std::shared_ptr<abstract_regex> symb(char c);

    std::shared_ptr<abstract_regex> empty_set();

    std::shared_ptr<abstract_regex> epsilon();

}

#endif //FL_2021_HSE_REGEX_ENGINE_H
