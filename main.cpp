#include <iostream>
#include "parser.h"
#include <string>
#include <vector>

struct test {
    std::shared_ptr<Regexp> regx;
    std::string regx_normal;
    std::vector<std::string> inputs;
    std::vector<bool> expected_output;
    std::string start() {
        int test_passed = 0;
        for (int i = 0; i < inputs.size(); i++) {
            if (match(inputs[i], regx) == expected_output[i]) {
                test_passed++;
            }
        }
        return "Passed " + std::to_string(test_passed) + " out of " + std::to_string(inputs.size()) + "\n";
    }
};

int main() {
    std::cout << "Test only one char: \n";
    {
        test t1;
        t1.regx_normal = "a";
        t1.regx = std::make_shared<Regexp>('a');
        t1.inputs = {"a", "aaa", "b", "ab"};
        t1.expected_output = {true, false, false, false};
        std::cout << t1.start();
    }
    std::cout << "Test star: \n";
    {
        test t1;
        t1.regx_normal = "a*";
        t1.regx = std::make_shared<Regexp>(STAR, std::make_shared<Regexp>('a'));
        t1.inputs = {"a", "aaaaaaa", "aaab", "b", "bbb"};
        t1.expected_output = {true, true, false, false, false};
        std::cout << t1.start();
    }

    std::cout << "Test alt: \n";
    {
        test t1;
        t1.regx_normal = "a|b";
        t1.regx = std::make_shared<Regexp>(ALT, std::make_shared<Regexp>('a'), std::make_shared<Regexp>('b'));
        t1.inputs = {"a", "b", "ab", "aaaaaaa", "aaab", "bbb"};
        t1.expected_output = {true, true, false, false, false, false};
        std::cout << t1.start();
    }
    std::cout << "Test alt + star: \n";
    {
        test t1;
        t1.regx_normal = "a|b";
        t1.regx = std::make_shared<Regexp>(ALT,
                                           std::make_shared<Regexp>(STAR, std::make_shared<Regexp>('a')),
                                           std::make_shared<Regexp>(STAR, std::make_shared<Regexp>('b')));
        t1.inputs = {"a", "b", "aaaaaaa", "bbb", "ab"};
        t1.expected_output = {true, true, true, true, false};
        std::cout << t1.start();

        test t2;
        t2.regx_normal = "(a|b)*";
        t2.regx = std::make_shared<Regexp>(STAR,
                                           std::make_shared<Regexp>(ALT,
                                                                    std::make_shared<Regexp>('a'),
                                                                    std::make_shared<Regexp>('b')));
        t2.inputs = {"ab", "ababab", "abbabaab", "aaaaaaab", "b", "bbb", "a", "baa", "abaca", "c"};
        t2.expected_output = {true, true, true, true, true, true, true, true, false, false};
        std::cout << t2.start();
    }
    std::cout << "Test concat: \n";
    {
        test t1;
        t1.regx_normal = "ab";
        t1.regx = std::make_shared<Regexp>(CONCAT,
                                           std::make_shared<Regexp>('a'),
                                           std::make_shared<Regexp>('b'));
        t1.inputs = {"ab", "ba", "aaaaaaa", "bbb", "a", "b"};
        t1.expected_output = {true, false, false, false, false, false};
        std::cout << t1.start();
    }
    std::cout << "Test concat + star: \n";
    {
        test t1;
        t1.regx_normal = "a*b";
        t1.regx = std::make_shared<Regexp>(CONCAT,
                                           std::make_shared<Regexp>(STAR, std::make_shared<Regexp>('a')),
                                           std::make_shared<Regexp>('b'));
        t1.inputs = {"ab", "aaaaaaab", "b", "bbb", "a", "baa"};
        t1.expected_output = {true, true, true, false, false, false};
        std::cout << t1.start();

        test t2;
        t2.regx_normal = "(ab)*";
        t2.regx = std::make_shared<Regexp>(STAR,
                                           std::make_shared<Regexp>(CONCAT,
                                                                    std::make_shared<Regexp>('a'),
                                                                    std::make_shared<Regexp>('b')));
        t2.inputs = {"ab", "ababab", "abbabaab", "aaaaaaab", "b", "bbb", "a", "baa"};
        t2.expected_output = {true, true, false, false, false, false, false, false};
        std::cout << t2.start();
    }
    std::cout << "Test all: \n";
    {
        test t1;
        t1.regx_normal = "a*(d|c)";
        t1.regx = std::make_shared<Regexp>(CONCAT,
                                           std::make_shared<Regexp>(STAR, std::make_shared<Regexp>('a')),
                                                                    std::make_shared<Regexp>(ALT, std::make_shared<Regexp>('d'), std::make_shared<Regexp>('c')));
        t1.inputs = {"aad", "ac", "c", "d", "a", "b", "abc"};
        t1.expected_output = {true, true, true, true, false, false, false};
        std::cout << t1.start();
    }
}
