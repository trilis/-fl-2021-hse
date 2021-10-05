#include <chrono>
#include <iomanip>
#include <iostream>
#include <memory>
#include <vector>

enum type {
  empty,
  epsilon,
  char_,
  star,
  concat,
  alt
};

struct Regexpr {
  type type;
  virtual ~Regexpr() = default;
};

struct Alt : Regexpr {
  std::shared_ptr<Regexpr> fst, snd;
  Alt(std::shared_ptr<Regexpr> fst, std::shared_ptr<Regexpr> snd)
      : fst(std::move(fst)), snd(std::move(snd)) {
    type = type::alt;
  }

  ~Alt() override = default;
};

struct Star : Regexpr {
  std::shared_ptr<Regexpr> elem;
  Star(std::shared_ptr<Regexpr> elem) : elem(std::move(elem)) {
    type = type::star;
  }

  ~Star() override = default;
};

struct Char : Regexpr {
  char c;
  Char(char c) : c(c) {
    type = type::char_;
  }
  ~Char() override = default;
};

struct Concat : Regexpr {
  std::shared_ptr<Regexpr> fst, snd;
  Concat(std::shared_ptr<Regexpr> fst, std::shared_ptr<Regexpr> snd)
      : fst(std::move(fst)), snd(std::move(snd)) {
    type = type::concat;
  }
  ~Concat() override = default;
};

struct Epsilon : Regexpr {
  Epsilon() {
    type = type::epsilon;
  }
  ~Epsilon() override = default;
};

struct Empty : Regexpr {
  Empty() {
    type = type::empty;
  }
  ~Empty() override = default;
};

bool union_(const bool &fst, const bool &snd) {
  if (fst == snd == 0) {
    return false;
  }
  return true;
}

std::shared_ptr<Regexpr> union_(std::shared_ptr<Regexpr> e1, std::shared_ptr<Regexpr> e2) {
  if (e1->type == type::epsilon || e2->type == type::epsilon) {
    Epsilon ee;
    return std::make_shared<Epsilon>(ee);
  }
  if (e1->type == e2->type == type::empty) {
    Empty ee;
    return std::make_shared<Empty>(ee);
  }
}

bool intersect(const bool &fst, const bool &snd) {
  if (fst == snd == 1) {
    return true;
  }
  return false;
}

std::shared_ptr<Regexpr> intersect(const std::shared_ptr<Regexpr> &fst,
                                   const std::shared_ptr<Regexpr> &snd) {
  if (fst->type == type::empty or snd->type == type::empty) {
    Empty empty;
    return std::make_shared<Empty>(empty);
  }
  if (fst->type == snd->type == type::epsilon) {
    Epsilon epsilon;
    return std::make_shared<Epsilon>(epsilon);
  }
}

bool nullable(const std::shared_ptr<Regexpr> &elem) {
  if (elem->type == type::empty or elem->type == type::char_) {
    return false;
  }

  if (elem->type == type::epsilon or elem->type == type::star) {
    return true;
  }

  if (elem->type == type::concat) {
    Concat *concat = dynamic_cast<Concat *>(elem.get());
    return intersect(nullable(concat->fst), nullable(concat->snd));
  }
  if (elem->type == type::alt) {
    Alt *alt = dynamic_cast<Alt *>(elem.get());
    return union_(nullable(alt->fst), nullable(alt->snd));
  }
}

std::shared_ptr<Regexpr> clever_star(std::shared_ptr<Regexpr> regexpr) {
  if (regexpr->type == type::empty) {
    return std::make_shared<Empty>();
  }
  if (regexpr->type == type::epsilon) {
    return std::make_shared<Epsilon>();
  }
  if (regexpr->type == type::star) {
    return regexpr;
  }
  return std::make_shared<Star>(regexpr);
}

std::shared_ptr<Regexpr> clever_alt(std::shared_ptr<Regexpr> fst, std::shared_ptr<Regexpr> snd) {
  if (fst->type == type::empty or snd->type == type::empty) {
    return (fst->type == type::empty ? snd : fst);
  }
  if(fst->type == type::epsilon and nullable(snd)){
    return snd;
  }
  if(snd->type == type::epsilon and nullable(fst)){
    return  fst;
  }
  if(fst == snd) return fst;
  return std::make_shared<Alt>(fst, snd);
}

std::shared_ptr<Regexpr> clever_concat(std::shared_ptr<Regexpr> fst, std::shared_ptr<Regexpr> snd) {
  if (fst->type == type::empty or snd->type == type::empty) {
    return std::make_shared<Empty>();
  }
  if (fst->type == type::epsilon or snd->type == type::epsilon) {
    return (fst->type == type::epsilon ? snd : fst);
  }
  return std::make_shared<Concat>(fst, snd);
}

std::shared_ptr<Regexpr> sec_der(const char &c, const std::shared_ptr<Regexpr> &regexpr) {
  if (regexpr->type == type::empty or regexpr->type == type::epsilon) {
    Empty empty;
    return std::make_shared<Empty>(empty);
  }
  if (regexpr->type == type::char_) {
    Char *char_ = dynamic_cast<Char *>(regexpr.get());
    if (c == char_->c) {
      Epsilon e;
      return std::make_shared<Epsilon>(e);
    }
    Empty e;
    return std::make_shared<Empty>(e);
  }

  if (regexpr->type == type::alt) {
    Alt *alt = dynamic_cast<Alt *>(regexpr.get());
    return clever_alt(sec_der(c, alt->fst), sec_der(c, alt->snd));
  }
  if (regexpr->type == type::star) {
    Star *star = dynamic_cast<Star *>(regexpr.get());
    return clever_concat(sec_der(c, star->elem), clever_star(star->elem));
  }
  if (regexpr->type == type::concat) {
    auto *concat = dynamic_cast<Concat *>(regexpr.get());

    if (nullable(concat->fst))
      return clever_alt(clever_concat(sec_der(c, concat->fst), concat->snd), sec_der(c, concat->snd));
    else
      return clever_alt(clever_concat(sec_der(c, concat->fst), concat->snd), std::make_shared<Empty>());
  }
}

std::shared_ptr<Regexpr> fst_der(const std::string &s, const std::shared_ptr<Regexpr> &regexpr) {
  if (s.empty()) {
    return regexpr;
  } else {
    return fst_der(s.substr(1), sec_der(s[0], regexpr));
  }
}

bool check(const std::shared_ptr<Regexpr> &regexpr, const std::string &str) {
  return nullable(fst_der(str, regexpr));
}

void test(const std::shared_ptr<Regexpr> &regexpr, const std::string &str, bool b) {
  std::cout << "string --- " << str << '\n';
  auto start = std::chrono::system_clock::now();
  if (check(regexpr, str) != b) {
    std::cerr << "IT IS NOT, LOSER, string eq:  " << str << '\n';
  }
  auto end = std::chrono::system_clock::now();
  std::chrono::duration<double> diff = end - start;
  std::cout << "time   --- " << diff.count() << " s\n";
}

#define Char_ std::make_shared<Char>
#define Alt_ std::make_shared<Alt>
#define Concat_ std::make_shared<Concat>
#define Star_ std::make_shared<Star>

int main() {
  std::cout << std::fixed << std::setprecision(9) << std::left;

  test(Char_('a'), "a", 1);
  test(Star_(Char_('a')), "aaaaaa", 1);
  test(Alt_(Char_('a'), Star_(Char_('b'))), "abbbbbbb", 0);
  test(Alt_(Char_('a'), Star_(Char_('b'))), "abbbbbbbaaa", 0);
  test(Star_(Char_('a')), "ba", 0);
  test(Star_(Alt_(Char_('a'), (Char_('b')))), "ababababababa", 1);
  test(Alt_(Alt_(Concat_(Char_('a'), Char_('a')), Star_(Char_('b'))), Concat_(Char_('b'), Star_(Char_('a')))), "aabaa", 0);
  test(Alt_(Alt_(Concat_(Char_('a'), Char_('a')), Star_(Char_('b'))), Concat_(Char_('b'), Star_(Char_('a')))), "baaa", 1);
  
  return 0;
}

//    string --- a
//    time   --- 0.000031000 s
//    string --- aaaaaa
//    time   --- 0.000008000 s
//    string --- abbbbbbb
//    time   --- 0.000007000 s
//    string --- abbbbbbbaaa
//    time   --- 0.000008000 s
//    string --- ba
//    time   --- 0.000003000 s
//    string --- ababababababa
//    time   --- 0.000021000 s
//    string --- aabaa
//    time   --- 0.000013000 s
//    string --- baaa
//    time   --- 0.000010000 s
