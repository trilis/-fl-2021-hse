#pragma once

namespace Reg {
    struct Regexp {
        virtual ~Regexp();
    };

    struct Empty : Regexp {
        ~Empty() override;
    };

    struct Epsilon : Regexp {
        ~Epsilon() override;
    };

    struct Symb : Regexp {
        char symb;

        explicit Symb(char symb_);

        ~Symb() override;
    };

    struct Star : Regexp {
        std::shared_ptr<Regexp> arg;

        explicit Star(std::shared_ptr<Regexp> arg_);
    };

    struct Alt : Regexp {
        std::shared_ptr<Regexp> arg1;
        std::shared_ptr<Regexp> arg2;

        explicit Alt(std::shared_ptr<Regexp> arg1_, std::shared_ptr<Regexp> arg2_);

        ~Alt() override;
    };

    struct Concat : Regexp {
        std::shared_ptr<Regexp> arg1;
        std::shared_ptr<Regexp> arg2;

        ~Concat() override;

        explicit Concat(std::shared_ptr<Regexp> arg1_, std::shared_ptr<Regexp> arg2_);

    };

    template<class type>
    bool verify_type(std::shared_ptr<Regexp> r);

    bool nullable(std::shared_ptr<Regexp> r);

    std::shared_ptr<Regexp> der(char c, std::shared_ptr<Regexp> lang);

    std::shared_ptr<Regexp> derivative(const std::string &s, std::shared_ptr<Regexp> lang);

    bool match(std::shared_ptr<Regexp> reg, const std::string &s);

    std::shared_ptr<Regexp> concat(std::shared_ptr<Regexp> a, std::shared_ptr<Regexp> b);

    std::shared_ptr<Regexp> alt(std::shared_ptr<Regexp> a, std::shared_ptr<Regexp> b);

    std::shared_ptr<Regexp> star(std::shared_ptr<Regexp> a);

    std::shared_ptr<Epsilon> epsilon();

    std::shared_ptr<Empty> empty();

    std::shared_ptr<Symb> symb(char c);
}