#include "parser.h"
#include <cassert>
#include <iostream>

int main(int argc, char* argv[]) {
  if (argc != 2) {
    return 1;
  }

  std::shared_ptr<Regex> r1(new Concat(new Char('a'), new Char('b')));
  std::shared_ptr<Regex> r2(new Star(new Concat(new Char('a'), new Char('b'))));
  std::shared_ptr<Regex> r3(new Star(new Epsilon()));
  std::shared_ptr<Regex> r4(new Star(new Concat(new Char('a'), new Char('b'))));
  std::shared_ptr<Regex> r5(new Star(new Concat(new Char('a'), new Char('b'))));

  std::shared_ptr<Regex> samples[] = {r1, r2, r3, r4, r5};
  assert(sizeof(samples) > 0);

  for (int i = 0; i < sizeof(samples) / sizeof(samples[0]); ++i) {
    if (Regex::match("ab", samples[i])) {
      std::cout << "Valid\n";
    } else {
      std::cout << "Invalid\n";
    }
  }

  return 0;
}