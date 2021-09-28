#include "parser_fast/parser.h"
#include <cassert>
#include <iostream>

int main(int argc, char *argv[]) {

  std::shared_ptr<Regex> r1(new Concat(new Char('a'), new Char('b')));
  std::shared_ptr<Regex> r2(new Star(new Char('a')));
  std::shared_ptr<Regex> r3(new Star(new Epsilon()));
  std::shared_ptr<Regex> r4(new Concat(new Star(new Concat(new Char('a'), new Char('b'))), new Alt(new Star(new Concat(new Char('c'), new Char('d'))), new Star(new Concat(new Char('e'), new Char('f'))))));
  std::shared_ptr<Regex> r5(new Star(new Alt(new Concat(new Char('0'), new Char('0')), new Concat(new Char('1'), new Char('1')))));
  
  std::shared_ptr<Regex> r_slow(    
                                    new Star(new Concat(
                                    new Star(new Char('0')),
                                    new Star(new Concat(
                                    new Star(new Char('9')),
                                    new Star(new Concat(
                                    new Star(new Char('8')),
                                    new Star(new Concat(
                                    new Star(new Char('7')),
                                    new Star(new Concat(
                                    new Star(new Char('6')),
                                    new Star(new Concat(
                                    new Star(new Char('5')),
                                    new Star(new Concat(
                                    new Star(new Char('4')),
                                    new Star(new Concat(
                                    new Star(new Char('3')),
                                    new Star(new Concat(
                                    new Star(new Char('2')),
                                    new Star(new Concat(
                                    new Star(new Char('1')),
                                    new Star(new Concat(
                                    new Star(new Char('z')),
                                    new Star(new Concat(
                                    new Star(new Char('y')),
                                    new Star(new Concat(
                                    new Star(new Char('x')),
                                    new Star(new Concat(
                                    new Star(new Char('w')),
                                    new Star(new Concat(
                                    new Star(new Char('v')),
                                    new Star(new Concat(
                                    new Star(new Char('u')),
                                    new Star(new Concat(
                                    new Star(new Char('t')),
                                    new Star(new Concat(
                                    new Star(new Char('s')),
                                    new Star(new Concat(
                                    new Star(new Char('r')),
                                    new Star(new Concat(
                                    new Star(new Char('q')),
                                    new Star(new Concat(
                                    new Star(new Char('p')),
                                    new Star(new Concat(
                                    new Star(new Char('o')),
                                    new Star(new Concat(
                                    new Star(new Char('n')),
                                    new Star(new Concat(
                                    new Star(new Char('m')),
                                    new Star(new Concat(
                                    new Star(new Char('l')),
                                    new Star(new Concat(
                                    new Star(new Char('k')),
                                    new Star(new Concat(
                                    new Star(new Char('j')),
                                    new Star(new Concat(
                                    new Star(new Char('i')),
                                    new Star(new Concat(
                                    new Star(new Char('h')),
                                    new Star(new Concat(
                                    new Star(new Char('g')),
                                    new Star(new Concat(
                                    new Star(new Char('f')),
                                    new Star(new Concat(
                                    new Star(new Char('e')), 
                                    new Star(new Concat
                                    (new Star(new Char('d')), new Star(new Concat(new Star(new Char('c')),
                                     new Star(new Concat
                                    (new Star(new Char('b')), new Star(new Char('a')))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))));
  
  std::shared_ptr<Regex> samples[] = {r1, r2, r3, r4, r5};
  std::shared_ptr<Regex> long_samples[] = {r_slow};

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