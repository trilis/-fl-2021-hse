#include "regexp.cpp"
#include <chrono>
#include <functional>
#include <iostream>

#define Char Char::create
#define Star Star::create
#define Alt Alt::create
#define Concat Concat::create

void TEST(const std::shared_ptr<BaseRegExp> &ptr, const std::string &s,
          bool answer) {
  static unsigned test_number = 0;
  std::chrono::steady_clock::time_point begin =
      std::chrono::steady_clock::now();
  bool result = match(ptr, s);
//  bool result = false;
  std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

  auto duration =
      std::chrono::duration_cast<std::chrono::duration<double>>(end - begin);

  if (test_number) {
    std::cout << "\n\n";
  }
  std::cout << "===TEST #" << ++test_number << "===\n";
  std::cout << "INPUT:    " << (s.size() <= 100 ? s : s.substr(0, 100) + "...") << '\n';
  std::cout << "REG EXP:  " << *ptr << '\n';
  std::cout << "EXPECTED: " << std::boolalpha << answer << '\n';
  std::cout << "GOT:      " << std::boolalpha << result << '\n';
  std::cout << "===" << duration.count() * 1000 << " ms===\n";
}

int main() {
  TEST(Concat(Star(Concat(Char('a'), Char('b'))), Char('a')), "abababa", true);
  TEST(Concat(Star(Concat(Char('a'), Char('b'))), Char('a')), "ababab", false);
  TEST(Concat(Star(Concat(Char('a'), Char('b'))), Char('a')), "abacaba", false);
  const int N = 50'000;
  std::string long_test_1;
  long_test_1.reserve(N);
  for (int i = 0; i < N; ++i) {
    long_test_1.push_back('a' + i % 2);
  }
  TEST(Concat(Char('a'), Char('b')), long_test_1, false);
  TEST(Star(Alt(Concat(Char('a'), Star(Char('b'))), Star(Alt(Concat(Char('a'), Star(Char('b'))), Concat(Char('a'), Star(Char('b'))))))), "aaaaaaaaaaaaaaaaaaaa", true);

  /*std::string long_test_2;
  long_test_2.reserve(N);
  for (int i = 0; i < N; ++i) {
    long_test_2.push_back(rand() % 26 + 'a');
  }
  std::function<std::shared_ptr<BaseRegExp>(char)> create_large_regexp = [&](char c) {
    return c == 'z' ? Char(c) : Alt(Char(c), create_large_regexp(c + 1));
  };
  TEST([&]() { return Star(create_large_regexp('a')); }(), long_test_2, true);*/

  /*std::string long_test_3;
  long_test_3.reserve(N);
  for (int i = 0; i < N; ++i) {
    long_test_3.push_back((i & 1) ^ 1);
  }
  std::function<std::shared_ptr<BaseRegExp>(int)> create_large_regexp_3 = [&](int i) {
    return i == N ? Char(i & 1) : Concat(Char(i & 1), create_large_regexp_3(i + 1));
  };
  TEST(create_large_regexp_3(0), long_test_2, false);*/
  return 0;
}
