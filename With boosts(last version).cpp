#include <cassert>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

using namespace std;

struct regexp {
    string type = "empty"; // Empty, Epsilon, Char, Star, Alt, Concat
    char ch;
    shared_ptr<regexp> r1;
    shared_ptr<regexp> r2;
    regexp() : type(), ch(' '), r1(nullptr), r2(nullptr) {};
    regexp(string type_, char ch_, shared_ptr<regexp> r1_, shared_ptr<regexp> r2_) : type(std::move(type_)), ch(ch_), r1(r1_), r2(r2_) {};

    bool operator==(regexp other) { // working only for non recursive regexp
        if (type == "Epsilon" && other.type == "Epsilon")
            return 1;
        if (type == "Empty" && other.type == "Empty")
            return 1;
        if (type == other.type && ch == other.ch)
            return (*r1 == *other.r1 && *r2 == *other.r2);
        return 0;
    }
};
regexp Empty = { "Empty", ' ', nullptr, nullptr };
regexp Epsilon = { "Epsilon", ' ', nullptr, nullptr };
//vector<regexp> save;


std::unique_ptr<regexp> derivative1(string str, regexp r);

bool nullable(regexp r);

void simplify(regexp& r) {
    regexp t;
    if (r.r1 == nullptr)
        r.r1 = make_shared<regexp>(Empty);
    if (r.r2 == nullptr)
        r.r2 = make_shared<regexp>(Empty);
    if (r.type == "Concat" && (*(r.r1)).type == "Empty") {
        r = Empty;
    }
    else if (r.type == "Concat" && (*(r.r2)).type == "Empty") {
        r = Empty;
    }
    else if (r.type == "Concat" && (*(r.r1)).type == "Epsilon") {
        t = *r.r2; r = t;
    }
    else if (r.type == "Concat" && (*(r.r2)).type == "Epsilon") {
        t = *r.r1; r = t;
    }
    else if (r.type == "Alt" && (*(r.r1)).type == "Empty") {
        t = *r.r2; r = t;
    }
    else if (r.type == "Alt" && (*(r.r2)).type == "Empty") {
        t = *r.r1; r = t;
    }
    else if (r.type == "Alt" && (*(r.r1)).type == "Epsilon" && nullable(*r.r2)) {
        t = *r.r2; r = t;
    }
    else if (r.type == "Alt" && (*(r.r2)).type == "Epsilon" && nullable(*r.r1)) {
        t = *r.r1; r = t;
    }
    else if (r.type == "Alt" && *(r.r1) == *(r.r2)) {
        t = *r.r1; r = t;
    }
    else if (r.type == "Star" && (*(r.r1)).type == "Empty") {
        t = *r.r1; r = t;
    }
    else if (r.type == "Star" && (*(r.r1)).type == "Epsilon") {
        t = *r.r1; r = t;
    }
    else if (r.type == "Star" && (*(r.r1)).type == "Star") {
        t = *r.r1; r = t;
    }
}

bool match(regexp r, string str) { // Done
    simplify(r);
    return (nullable(*derivative1(str, r)));
}

std::unique_ptr<regexp> derivative(char ch, regexp r) { // Done
    simplify(r);
    if (r == Empty)
        return make_unique<regexp>(Empty);
    if (r == Epsilon)
        return make_unique<regexp>(Empty);
    if (r.type == "Char")
        if (ch == r.ch)
            return make_unique<regexp>(Epsilon);
        else
            return make_unique<regexp>(Empty);
    if (r.type == "Alt") {
        regexp x;
        x.r1 = derivative(ch, *(r.r1));
        x.r2 = derivative(ch, *(r.r2));
        x.type = "Alt";
        return make_unique<regexp>(x);
    }
    if (r.type == "Star") {
        regexp x;
        x.r1 = derivative(ch, *(r.r1));
        x.r2 = make_shared<regexp>(*(new regexp));
        (*(x.r2)).type = "Star";
        (*(x.r2)).r1 = r.r1;
        x.type = "Concat";
        return make_unique<regexp>(x);
    }
    if (r.type == "Concat") {
        regexp x, left, right;
        left.type = "Concat";
        left.r1 = derivative(ch, *r.r1);
        left.r2 = r.r2;
        right.type = "Concat";
        if (nullable(*r.r1))
            right.r1 = make_unique<regexp>(Epsilon);
        else
            right.r1 = make_unique<regexp>(Empty);
        //right.r1 = nullable(*r.r1);
        right.r2 = derivative(ch, *r.r2);
        x.r1 = make_shared<regexp>(left);
        x.r2 = make_shared<regexp>(right);
        x.type = "Alt";
        return make_unique<regexp>(x);
    }
    return make_unique<regexp>(Empty);
}

std::unique_ptr<regexp> intersect(regexp r1, regexp r2) { // Done
    simplify(r1); simplify(r2);
    if (r1 == Empty)
        return make_unique<regexp>(Empty);
    if (r2 == Empty)
        return make_unique<regexp>(Empty);
    if (r1 == Epsilon && r2 == Epsilon)
        return make_unique<regexp>(Epsilon);
    return make_unique<regexp>(Empty);
}

std::unique_ptr<regexp> Union(regexp r1, regexp r2) { // Done
    simplify(r1); simplify(r2);
    if (r1 == Epsilon)
        return make_unique<regexp>(Epsilon);
    if (r2 == Epsilon)
        return make_unique<regexp>(Epsilon);
    if (r1 == Empty && r2 == Empty)
        return make_unique<regexp>(Empty);
    return make_unique<regexp>(Empty);
}

bool nullable(regexp r) { // Done
    simplify(r);
    if (r == Empty)
        return 0;
    if (r == Epsilon)
        return 1;
    if (r.type == "Char")
        return 0;
    if (r.type == "Concat") {
        //regexp x;
        //x = *intersect(*nullable(*r.r1), *nullable(*r.r2));
        return (nullable(*r.r1) && nullable(*r.r2));
    }
    if (r.type == "Alt") {
        //regexp x;
        //x = *Union(*nullable(*r.r1), *nullable(*r.r2));
        return (nullable(*r.r1) || nullable(*r.r2));
    }
    if (r.type == "Star") {
        return 1;
    }
    return 0;
}

std::unique_ptr<regexp> derivative1(string str, regexp r) { //Done
    simplify(r);
    if (str.empty())
        return make_unique<regexp>(r);
    char first = str[0];
    string new_str = str;
    new_str.erase(0, 1); // able to make faster
    return std::move(derivative1(new_str, *derivative(first, r)));
}

void test1(regexp& r) { // (ab)*a
    regexp one, second, left, right;
    left.type = "Star";
    left.r1 = make_shared<regexp>(Empty);
    (*(left.r1)).r1 = make_shared<regexp>(Empty);
    left.r2 = make_shared<regexp>(*(new regexp));
    (*(left.r1)).r2 = make_shared<regexp>(Empty);

    (*(left.r1)).type = "Concat";
    (*(*(left.r1)).r1).type = "Char";
    (*(*(left.r1)).r1).ch = 'a';
    (*(*(left.r1)).r2).type = "Char";
    (*(*(left.r1)).r2).ch = 'b';
    right.type = "Char";
    right.ch = 'a';

    one.type = "Concat";
    one.r1 = make_shared<regexp>(left);
    one.r2 = make_shared<regexp>(right);
    r = one;
}

void test2(regexp& r) { // a*
    regexp second;
    second.type = "Star";
    second.r1 = make_shared<regexp>(Empty);
    (*(second.r1)).type = "Char";
    (*(second.r1)).ch = 'a';
    r = second;
}

void test3(regexp& r) { // (a|b)*
    regexp third, left;
    third.type = "Star";
    left.r1 = make_shared<regexp>(Empty);
    left.r2 = make_shared<regexp>(Empty);
    (*(left.r1)).r1 = make_shared<regexp>(Empty);
    (*(left.r1)).r2 = make_shared<regexp>(Empty);
    left.type = "Alt";
    (*(left.r1)).type = "Char";
    (*(left.r2)).type = "Char";
    (*(left.r1)).ch = 'a';
    (*(left.r2)).ch = 'b';
    third.r1 = make_shared<regexp>(left);
    r = third;
}

void test4(regexp& r) { // empty
    r = Empty;
}

void test5(regexp& r) { // epsilon
    r = Epsilon;
}

void test6(regexp& r) { // a(c|d)*(a|b)x
    regexp x, y, z, t, j, f, g, h;
    x.type = "Char";
    x.ch = 'x';

    y.type = "Alt";
    y.r1 = make_shared<regexp>(Empty);
    y.r2 = make_shared<regexp>(Empty);
    (*(y.r1)).type = "Char";
    (*(y.r2)).type = "Char";
    (*(y.r1)).ch = 'a';
    (*(y.r2)).ch = 'b';

    z.type = "Alt";
    z.r1 = make_shared<regexp>(Empty);
    z.r2 = make_shared<regexp>(Empty);
    (*(z.r1)).type = "Char";
    (*(z.r2)).type = "Char";
    (*(z.r1)).ch = 'c';
    (*(z.r2)).ch = 'd';

    t.type = "Star";
    t.r1 = make_shared<regexp>(z);

    j.type = "Char";
    j.ch = 'a';

    h.type = "Concat";
    h.r1 = make_shared<regexp>(y);
    h.r2 = make_shared<regexp>(x);

    g.type = "Concat";
    g.r1 = make_shared<regexp>(t);
    g.r2 = make_shared<regexp>(h);

    f.type = "Concat";
    f.r1 = make_shared<regexp>(j);
    f.r2 = make_shared<regexp>(g);

    r = f;
}

void start_test(string str,int num) {
    regexp r;
    if (num == 1)
        test1(r);
    if (num == 2)
        test2(r);
    if (num == 3)
        test3(r);
    if (num == 4)
        test4(r);
    if (num == 5)
        test5(r);
    if (num == 6)
        test6(r);
    if (match(r, str)) {
        std::cout << "It's OK\n";
    }
    else {
        std::cout << "Something gone wrong\n";
    }
}

int main() {
    regexp r;
    //cin >> str; // TODO in the end
    start_test("ababa", 1);
    start_test("aaaaaa", 2);
    start_test("aabbaab", 3);
    start_test("", 5);
    start_test("accdbx", 6);
    std::string str;
}