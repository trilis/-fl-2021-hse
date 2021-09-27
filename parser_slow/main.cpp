#include "parser.h"
#include <iostream>
#include <cassert>

int main() {
    std::shared_ptr<Regex> r1(new Concat(new Char('a'), new Char('b')));
    std::shared_ptr<Regex> r2(new Star(new Concat(new Char('a'), new Char('b'))));
    
    std::shared_ptr<Regex> samples[] = {r1, r2};
    assert(sizeof(samples) > 0);

    for (int i = 0; i < sizeof(samples) / sizeof(samples[0]); ++i) {
        if (Regex::match("", samples[i])) {
            std::cout << "Valid\n";
        }
        else {
            std::cout << "Invalid\n";
        }
    }

    return 0;
}