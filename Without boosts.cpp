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
        if (type == other.type && ch == other.ch)
            return 1;
        return 0;
    }
};
regexp Empty = { "Empty", ' ', nullptr, nullptr };
regexp Epsilon = { "Epsilon", ' ', nullptr, nullptr };
//vector<regexp> save;


std::unique_ptr<regexp> derivative1(string str, regexp r);
std::unique_ptr<regexp> nullable(regexp r);

bool match(regexp r, string str) { // Done
    //regexp ans = *nullable(*derivative1(str, r));
    //cout << ans.type << '\n';
    //cout << (*(ans.r1)).type << '\n';
    //cout << (*(*(ans.r1)).r1).type << '\n';
    //cout << (*(*(ans.r1)).r2).type << '\n';
    //cout << (*(ans.r2)).type << '\n';
    //cout << (*(*(ans.r2)).r1).type << '\n';
    //cout << (*(*(ans.r2)).r2).type << '\n';
    //if ((*(ans.r1)).type == "Epsilon" && (*(ans.r2)).type == "Epsilon")
    //    return 1;
    //return 0;
    return (*nullable(*derivative1(str, r)) == Epsilon);
}

std::unique_ptr<regexp> derivative(char ch, regexp r) { // Done
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
        right.r1 = nullable(*r.r1);
        right.r2 = derivative(ch, *r.r2);
        x.r1 = make_shared<regexp>(left);
        x.r2 = make_shared<regexp>(right); // possibly wrong? dont know why
        x.type = "Alt";
        return make_unique<regexp>(x);
    }
    return make_unique<regexp>(Empty);
}

std::unique_ptr<regexp> intersect(regexp r1, regexp r2) { // Done
    if (r1 == Empty)
        return make_unique<regexp>(Empty);
    if (r2 == Empty)
        return make_unique<regexp>(Empty);
    if (r1 == Epsilon && r2 == Epsilon)
        return make_unique<regexp>(Epsilon);
    return make_unique<regexp>(Empty);
}

std::unique_ptr<regexp> Union(regexp r1, regexp r2) { // Done
    if (r1 == Epsilon)
        return make_unique<regexp>(Epsilon);
    if (r2 == Epsilon)
        return make_unique<regexp>(Epsilon);
    if (r1 == Empty && r2 == Empty)
        return make_unique<regexp>(Empty);
    return make_unique<regexp>(Empty);
}

std::unique_ptr<regexp> nullable(regexp r) { // Done
    if (r == Empty)
        return make_unique<regexp>(Empty);
    if (r == Epsilon)
        return make_unique<regexp>(Epsilon);
    if (r.type == "Char")
        return make_unique<regexp>(Empty);
    if (r.type == "Concat") {
        regexp x;
        x = *intersect(*nullable(*r.r1), *nullable(*r.r2));
        return make_unique<regexp>(x);
    }
    if (r.type == "Alt") {
        regexp x;
        x = *Union(*nullable(*r.r1), *nullable(*r.r2));
        return make_unique<regexp>(x);
    }
    if (r.type == "Star") {
        return make_unique<regexp>(Epsilon);
    }
    return make_unique<regexp>(Empty);
}

std::unique_ptr<regexp> derivative1(string str, regexp r) { //Done
    if (str.empty())
        return make_unique<regexp>(r);
    char first = str[0];
    string new_str = str;
    new_str.erase(0, 1); // able to make faster
    return std::move(derivative1(new_str, *derivative(first, r)));
}

void test1(regexp &r) { // (ab)*a
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

void test2(regexp &r) { // a*
    regexp second;
    second.type = "Star";
    second.r1 = make_shared<regexp>(Empty);
    (*(second.r1)).type = "Char";
    (*(second.r1)).ch = 'a';
    r = second;
}

void test3(regexp &r) { // (a|b)*
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

void test4(regexp &r) { // empty
    r = Empty;
}

void test5(regexp &r) { // epsilon
    r = Epsilon;
}

void test6(regexp &r) { // a(c|d)*(a|b)x
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

int main() {
    regexp r;

    test6(r);
    std::string str;
    //cin >> str; // TODO in the end
    str = "accbx";
    if (match(r, str)) {
        std::cout << "It's OK";
    }
    else {
        std::cout << "Something gone wrong";
    }
}