#ifndef FL_2021_HSE_REGEXP_CPP
#define FL_2021_HSE_REGEXP_CPP

#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <utility>

enum class Type { Empty, Epsilon, Char, Star, Alt, Concat };

#ifdef OPTIMIZED
struct BaseRegExp;
bool nullable(const std::shared_ptr<BaseRegExp> &ptr);
#endif

struct BaseRegExp {
  Type type;
  explicit BaseRegExp(Type type) : type(type) {}
  virtual void print(std::ostream &os) const = 0;
  friend std::ostream &operator<<(std::ostream &os, const BaseRegExp &r) {
    r.print(os);
    return os;
  }
  virtual ~BaseRegExp() = default;
};

struct Empty : BaseRegExp {
  Empty() : BaseRegExp(Type::Empty) {}
  void print(std::ostream &os) const override {}
  static std::shared_ptr<BaseRegExp> create() {
    return std::make_shared<Empty>();
  }
};

struct Epsilon : BaseRegExp {
  Epsilon() : BaseRegExp(Type::Epsilon) {}
  void print(std::ostream &os) const override {}
  static std::shared_ptr<BaseRegExp> create() {
    return std::make_shared<Epsilon>();
  }
};

struct Char : BaseRegExp {
  char c;
  void print(std::ostream &os) const override { std::cout << c; }
  explicit Char(char c) : BaseRegExp(Type::Char), c(c) {}
  static std::shared_ptr<BaseRegExp> create(char c) {
    return std::make_shared<Char>(c);
  }
};

struct Star : BaseRegExp {
  std::shared_ptr<BaseRegExp> p;
  explicit Star(std::shared_ptr<BaseRegExp> p)
      : BaseRegExp(Type::Star), p(std::move(p)) {}
  void print(std::ostream &os) const override {
    std::cout << "(";
    p->print(os);
    std::cout << ")*";
  }
  static std::shared_ptr<BaseRegExp>
  create(const std::shared_ptr<BaseRegExp> &ptr) {
#ifdef OPTIMIZED
    if (ptr->type == Type::Empty) {
      return Empty::create();
    } else if (ptr->type == Type::Epsilon) {
      return Epsilon::create();
    } else if (ptr->type == Type::Star) {
      return ptr;
    } else {
      return std::make_shared<Star>(ptr);
    }
#else
    return std::make_shared<Star>(ptr);
#endif
  }
};

struct Alt : BaseRegExp {
  std::shared_ptr<BaseRegExp> l, r;
  Alt(std::shared_ptr<BaseRegExp> l, std::shared_ptr<BaseRegExp> r)
      : BaseRegExp(Type::Alt), l(std::move(l)), r(std::move(r)) {}
  void print(std::ostream &os) const override {
    std::cout << "(";
    l->print(os);
    std::cout << "|";
    r->print(os);
    std::cout << ")";
  }
  static std::shared_ptr<BaseRegExp>
  create(const std::shared_ptr<BaseRegExp> &lhs,
         const std::shared_ptr<BaseRegExp> &rhs) {
#ifdef OPTIMIZED
    if (rhs->type == Type::Empty ||
        rhs->type == Type::Epsilon && nullable(lhs)) {
      return lhs;
    } else if (lhs->type == Type::Empty ||
               lhs->type == Type::Epsilon && nullable(rhs)) {
      return rhs;
    } else {
      return std::make_shared<Alt>(lhs, rhs);
    }
#else
    return std::make_shared<Alt>(lhs, rhs);
#endif
  }
};

struct Concat : BaseRegExp {
  std::shared_ptr<BaseRegExp> l, r;
  Concat(std::shared_ptr<BaseRegExp> l, std::shared_ptr<BaseRegExp> r)
      : BaseRegExp(Type::Concat), l(std::move(l)), r(std::move(r)) {}
  void print(std::ostream &os) const override {
    l->print(os);
    r->print(os);
  }
  static std::shared_ptr<BaseRegExp>
  create(const std::shared_ptr<BaseRegExp> &lhs,
         const std::shared_ptr<BaseRegExp> &rhs) {
#ifdef OPTIMIZED
    if (rhs->type == Type::Empty || lhs->type == Type::Empty) {
      return Empty::create();
    } else if (rhs->type == Type::Epsilon) {
      return lhs;
    } else if (lhs->type == Type::Epsilon) {
      return rhs;
    } else {
      return std::make_shared<Concat>(lhs, rhs);
    }
#else
    return std::make_shared<Concat>(lhs, rhs);
#endif
  }
};

#ifdef OPTIMIZED
bool nullable(const std::shared_ptr<BaseRegExp> &ptr) {
  if (ptr->type == Type::Empty || ptr->type == Type::Char) {
    return false;
  } else if (ptr->type == Type::Epsilon || ptr->type == Type::Star) {
    return true;
  } else if (ptr->type == Type::Alt) {
    auto new_ptr = std::dynamic_pointer_cast<Alt>(ptr);
    return nullable(new_ptr->l) || nullable(new_ptr->r);
  } else if (ptr->type == Type::Concat) {
    auto new_ptr = std::dynamic_pointer_cast<Concat>(ptr);
    return nullable(new_ptr->l) && nullable(new_ptr->r);
  } else {
    assert(false);
  }
}
#else
std::shared_ptr<BaseRegExp> intersect(const std::shared_ptr<BaseRegExp> &lhs,
                                      const std::shared_ptr<BaseRegExp> &rhs) {
  if (lhs->type == Type::Empty || rhs->type == Type::Empty) {
    return Empty::create();
  } else if (lhs->type == Type::Epsilon && rhs->type == Type::Epsilon) {
    return Epsilon::create();
  } else {
    assert(false);
  }
}

std::shared_ptr<BaseRegExp> unite(const std::shared_ptr<BaseRegExp> &lhs,
                                  const std::shared_ptr<BaseRegExp> &rhs) {
  if (lhs->type == Type::Epsilon || rhs->type == Type::Epsilon) {
    return Epsilon::create();
  } else if (lhs->type == Type::Empty && rhs->type == Type::Empty) {
    return Empty::create();
  } else {
    assert(false);
  }
}

std::shared_ptr<BaseRegExp> nullable(const std::shared_ptr<BaseRegExp> &ptr) {
  if (ptr->type == Type::Empty || ptr->type == Type::Char) {
    return Empty::create();
  } else if (ptr->type == Type::Epsilon || ptr->type == Type::Star) {
    return Epsilon::create();
  } else if (ptr->type == Type::Alt) {
    auto new_ptr = std::dynamic_pointer_cast<Alt>(ptr);
    return unite(nullable(new_ptr->l), nullable(new_ptr->r));
  } else if (ptr->type == Type::Concat) {
    auto new_ptr = std::dynamic_pointer_cast<Concat>(ptr);
    return intersect(nullable(new_ptr->l), nullable(new_ptr->r));
  } else {
    assert(false);
  }
}
#endif

std::shared_ptr<BaseRegExp> der(char c,
                                const std::shared_ptr<BaseRegExp> &ptr) {
  if (ptr->type == Type::Empty || ptr->type == Type::Epsilon) {
    return Empty::create();
  } else if (ptr->type == Type::Char) {
    auto new_ptr = std::dynamic_pointer_cast<Char>(ptr);
    if (c == new_ptr->c) {
      return Epsilon::create();
    } else {
      return Empty::create();
    }
  } else if (ptr->type == Type::Star) {
    auto new_ptr = std::dynamic_pointer_cast<Star>(ptr);
    return Concat::create(der(c, new_ptr->p), Star::create(ptr));
  } else if (ptr->type == Type::Alt) {
    auto new_ptr = std::dynamic_pointer_cast<Alt>(ptr);
    return Alt::create(der(c, new_ptr->l), der(c, new_ptr->r));
  } else if (ptr->type == Type::Concat) {
    auto new_ptr = std::dynamic_pointer_cast<Concat>(ptr);
#ifdef OPTIMIZED
    return nullable(new_ptr->l)
               ? Alt::create(Concat::create(der(c, new_ptr->l), new_ptr->r),
                             der(c, new_ptr->r))
               : Alt::create(Concat::create(der(c, new_ptr->l), new_ptr->r),
                             Empty::create());
#else
    return Alt::create(
        Concat::create(der(c, new_ptr->l), new_ptr->r),
        Concat::create(nullable(new_ptr->l), der(c, new_ptr->r)));
#endif
  } else {
    assert(false);
  }
}

std::shared_ptr<BaseRegExp> der_(std::string s,
                                 std::shared_ptr<BaseRegExp> ptr) {
  if (s.empty()) {
    return ptr;
  } else {
    char c = s[0];
    s.erase(s.begin());
    return der_(s, der(c, ptr));
  }
}

bool match(std::shared_ptr<BaseRegExp> ptr, std::string s) {
  return nullable(der_(std::move(s), std::move(ptr)))
#ifndef OPTIMIZED
             ->type == Type::Epsilon
#endif
      ;
}

#endif // FL_2021_HSE_REGEXP_CPP
