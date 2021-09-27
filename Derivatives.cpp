#include <list>
#include <iostream>
#include <memory>
#include <ctime>
#include <cassert>

using namespace std;

struct Regexp {
    string type;
    virtual ~Regexp() {};
};

struct Epsilon : Regexp {
    Epsilon() { type = "Epsilon"; }
};

struct Empty : Regexp {
    Empty() { type = "Empty"; }
};

struct Char : Regexp {
    char h;
    Char(char h_) : h(h_) { type = "Char"; };
};

struct Star : Regexp {
    shared_ptr<Regexp> r;
    Star(shared_ptr<Regexp> r_){r=r_; type="Star";};
};

struct Alt : Regexp {
    shared_ptr<Regexp> l, r;
    Alt(shared_ptr<Regexp> l_, shared_ptr<Regexp> r_) {
        l = l_;
        r = r_;
        type="Alt";
    };
    virtual void t() {
    };
    Alt(Alt &&c) {
        l = c.l;
        r = c.r;
        type="Alt";
    };
};

struct Concat : Regexp {
    shared_ptr<Regexp> l, r;
    Concat(shared_ptr<Regexp> l_, shared_ptr<Regexp> r_) {
        l = l_;
        r = r_;
        type="Concat";
    };
    virtual void t() {
    };
};

////
shared_ptr<Regexp> uni(shared_ptr<Regexp> e1, shared_ptr<Regexp> e2) {
    if (e1->type == "Epsilon" || e2->type == "Epsilon") {
        Epsilon ee;
        return make_shared<Epsilon>(ee);
    }
    if (e1->type == "Empty" && e2->type == "Empty") {
        Empty ee;
        return make_shared<Empty>(ee);
    }
}
//

shared_ptr<Regexp> intersect(shared_ptr<Regexp> e1, shared_ptr<Regexp> e2){
    if (e1->type == "Empty" || e2->type == "Empty") {
        Empty ee;
        return make_shared<Empty>(ee);
    }
    if (e1->type == "Epsilon" && e2->type == "Epsilon") {
        Epsilon ee;
        return make_shared<Epsilon>(ee);
    }
}

bool intersect(bool e1,bool e2){
    if (e1== 1 && e2 == 1) {
        return 1;
    }
    return 0;
}

bool uni(bool e1,bool e2){
    if (e1== 0 && e2 == 0) {
        return 0;
    }
    return 1;
}
/*
shared_ptr<Regexp> nullable(shared_ptr<Regexp> r) {
    if (r->type == "Empty") {
        Empty e;
        return make_shared<Empty>(e);
    }
    if (r->type == "Epsilon") {
        Epsilon e;
        return make_shared<Epsilon>(e);
    }
    if (r->type == "Char") {
        Empty e;
        return make_shared<Empty>(e);
    }
    if (r->type == "Concat") {
        Concat *c = dynamic_cast<Concat *>(r.get());
        return intersect(nullable(c->l),nullable(c->r));
    }
    if (r->type == "Alt") {
        Alt *a = dynamic_cast<Alt *>(r.get());
        return uni(nullable(a->l),nullable(a->r));
    }
    if (r->type == "Star") {
        Epsilon e;
        return make_shared<Epsilon>(e);
    }
}*/

bool nullable(shared_ptr<Regexp> r) {
    if (r->type == "Empty") {
        return 0;
    }
    if (r->type == "Epsilon") {
        return 1;
    }
    if (r->type == "Char") {
        return 0;
    }
    if (r->type == "Concat") {
        Concat *c = dynamic_cast<Concat *>(r.get());
        return intersect(nullable(c->l),nullable(c->r));
    }
    if (r->type == "Alt") {
        Alt *a = dynamic_cast<Alt *>(r.get());
        return uni(nullable(a->l),nullable(a->r));
    }
    if (r->type == "Star") {
        return 1;
    }
}

///

shared_ptr<Regexp> constrStar(shared_ptr<Regexp> r){
    if(r->type=="Empty"){
        return make_shared<Empty>();
    }
    if(r->type=="Epsilon"){
        return make_shared<Empty>();
    }
    if(r->type=="Star"){
        return r;
    }
    return make_shared<Star>(r);
}

shared_ptr<Regexp> constrAlt(shared_ptr<Regexp> l,shared_ptr<Regexp> r){
    if(r->type=="Empty"){
        return l;
    }
    if(l->type=="Empty"){
        return r;
    }
    return make_shared<Alt>(l,r);
}

shared_ptr<Regexp> constrConcat(shared_ptr<Regexp> l,shared_ptr<Regexp> r){
    if(r->type=="Empty" || l->type=="Empty"){
        return make_shared<Empty>();
    }
    if(r->type=="Epsilon"){
        return l;
    }
    if(l->type=="Epsilon"){
        return r;
    }
    return make_shared<Concat>(l,r);
}


shared_ptr<Regexp> der(char h, shared_ptr<Regexp> r) {
    if (r->type == "Char") {
        Char *c = dynamic_cast<Char *>(r.get());
        if (h == c->h) {
            Epsilon e;
            return make_shared<Epsilon>(e);
        }
        Empty e;
        return make_shared<Empty>(e);
    }
    if (r->type == "Epsilon"){
        Empty e;
        return make_shared<Empty>(e);
    }
    if (r->type == "Empty"){
        Empty e;
        return make_shared<Empty>(e);
    }
    if (r->type == "Alt"){
        Alt *c = dynamic_cast<Alt *>(r.get());
        return constrAlt(der(h,c->l),der(h,c->r));
    }
    if (r->type == "Concat"){
        Concat *c = dynamic_cast<Concat *>(r.get());
        if(nullable(c->l))
        return make_shared<Alt>(constrConcat(der(h,c->l),c->r),der(h,c->r));
        else
            return make_shared<Alt>(constrConcat(der(h,c->l),c->r),make_shared<Empty>());
    }
    if (r->type == "Star"){
        Star *c = dynamic_cast<Star *>(r.get());
        return constrConcat(der(h,c->r),constrStar(c->r));
    }

}

shared_ptr<Regexp> der_s(string s, shared_ptr<Regexp> r) {
    if (s.empty()) {
        return r;
    } else {
        return der_s(s.substr(1), der(s[0], r));
    }
}

bool match(shared_ptr<Regexp> r, string s) {
    return nullable(der_s(s, r));
    //Epsilon==true
    //Empty==false
}

void testing(shared_ptr<Regexp> r, string s, bool b){
    double start_time =  clock(); // начальное время
    assert(match(r,s)==b);
    double end_time = clock(); // конечное время
    cout << "time: "<<(end_time - start_time) /1000 << endl;
}

#define Char make_shared<Char>
#define Alt make_shared<Alt>
#define Concat make_shared<Concat>
#define Star make_shared<Star>

int main() {
    testing(Alt (Concat (Char ('a'),Char ('b')),Char ('b')),"abacaba",0);
    testing(Star (Char ('a')), "a",1);
    testing(Star (Char ('a')), "aaaaaa",1);
    testing(Star (Char ('a')), "ba",0);
    testing( Concat ( Star ( Concat ( Char ('a'), Char ('b'))) , Char ('a')),"sigbk",0);

    testing( Alt ( Star ( Concat ( Char ('a'), Char ('b'))) , Concat (Char ('a'),Star(Char('b')))),"sigbk",0);
    testing( Alt ( Star ( Concat ( Char ('a'), Char ('b'))) , Concat (Char ('a'),Star(Char('b')))),"abab",1);
    testing( Alt ( Star ( Concat ( Char ('a'), Char ('b'))) , Concat (Char ('a'),Star(Char('b')))),"abbbb",1);

    testing(Concat(Char('a'),Concat(Char('a'),Char('a'))),"aaaaaaaa",0);
    testing(Concat(Char('a'),Concat(Char('a'),Concat(Char('a'),Char('a')))),"aaaaaaaa",0);

    testing(Star(Alt(Concat(Char('a'),Star(Char('x'))),Alt(Concat(Concat(Char('a'),Star(Char('x'))),Star(Char('x'))),Alt(Concat(Concat(Char('a'),Star(Char('x'))),Star(Char('x'))),Alt(Char('d'),Alt(Concat(Char('a'),Star(Char('x'))),Alt(Char('d'),Char('d')))))))),"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaajshvckbbllsbdvlbjkljbaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaajshvckbbllsbdvlbjkljbaaaaaaaaaaaaaaaaaa",0);

}