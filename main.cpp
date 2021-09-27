#include "parser.h"
#include <iostream>

int main() {
    std::shared_ptr<Regex> r1(new Concat(new Char('a'), new Char('b')));
    std::shared_ptr<Regex> r2(new Star(new Concat(new Char('a'), new Char('b'))));
    
    std::shared_ptr<Regex> samples[] = {r1, r2};

    for (int i = 0; i < sizeof(samples); ++i) {
        if (Regex::match("ab", samples[i])) {
            std::cout << "Valid\n";
        }
        else {
            std::cout << "Invalid\n";
        }
    }

    if (Regex::match("ab", r1)) {
        std::cout << "Valid\n";
    }
    else {
        std::cout << "Invalid\n";
    }


    return 0;
}