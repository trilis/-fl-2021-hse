#include "parser.h"
#include <cassert>
#include <iostream>

int main(int argc, char *argv[]) {

  std::shared_ptr<Regex> r1(new Concat(new Char('a'), new Char('b')));
  std::shared_ptr<Regex> r2(new Star(new Char('a')));
  std::shared_ptr<Regex> r3(new Star(new Epsilon()));
  std::shared_ptr<Regex> r4(new Concat(new Star(new Concat(new Char('a'), new Char('b'))), new Alt(new Star(new Concat(new Char('c'), new Char('d'))), new Star(new Concat(new Char('e'), new Char('f'))))));
  std::shared_ptr<Regex> r5(new Star(new Alt(new Concat(new Char('0'), new Char('0')), new Concat(new Char('1'), new Char('1')))));

  std::shared_ptr<Regex> samples[] = {r1, r2, r3, r4, r5};
  assert(sizeof(samples) > 0);

  for (int i = 0; i < sizeof(samples) / sizeof(samples[0]); ++i) {
    if (Regex::match(argc > 1 ? argv[1] : "", samples[i])) {
      std::cout << "Valid\n";
    } else {
      std::cout << "Invalid\n";
    }
  }

  return 0;
}