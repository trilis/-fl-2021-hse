#include <bits/stdc++.h>

using namespace std;

struct Empty {
	virtual bool nullable() {
		return false;
	}

	virtual Empty *der(char c) {
		return new Empty();
	}

	virtual void print() {
	}
};

struct Epsilon: virtual Empty {
	bool nullable() override {
		return true;
	}
	
	Empty *der(char c) {
		return new Empty();
	}

	void print() {
		cout << "eps";
	}
};

bool intersect(bool r1, bool r2) {
	if(r1 == false || r2 == false) {
		return false;
	}

	if(r1 == true && r2 == true) {
		return true;
	}

	cout << "wtf?";exit(0);
}

bool uni(bool r1, bool r2) {
	if(r1 == true || r2 == true) {
		return true;
	}

	if(r1 == false && r2 == false) {
		return false;
	}
	cout << "wtf?";exit(0);
}

struct Char: virtual Empty {
	char c;
	explicit Char(char c_) : c(c_) {
	}

	bool nullable() {
		return false;
	}

	Empty *der(char c2) {
		if(c == c2) 
			return new Epsilon();
		return new Empty();
	}

	void print() {
		cout << c;
	}
};

struct Alt: virtual Empty {
	Empty *regex1, *regex2;
	explicit Alt(Empty *r, Empty *r2) : 
		regex1(r), regex2(r2) {
	}

	bool nullable() {
		return uni(regex1->nullable(), regex2->nullable());
	}

	Empty *der(char c) {
		return new Alt(regex1->der(c), regex2->der(c));
	}

	void print() {
		cout << "(";
		regex1->print();
		cout << "|";
		regex2->print();
		cout << ")";
	}
};

struct Concat: virtual Empty {
	Empty *regex1, *regex2;
	explicit Concat(Empty *r, Empty *r2) : 
		regex1(r), regex2(r2) {
	}

	bool nullable() {
		return intersect(regex1->nullable(), 
				regex2->nullable());
	}

	Empty *der(char c) {
		return new Alt(new Concat(regex1->der(c), regex2),
				new Concat(regex1->nullable() ? new Epsilon() : new Empty(), regex2->der(c)));
	}

	void print() {
		cout << "(";
		regex1->print();
		regex2->print();
		cout << ")";
	}
};

struct Star: virtual Empty {
	Empty *regex;
	explicit Star(Empty *r) : regex(r) {
	}

	bool nullable() {
		return true;
	}

	Empty *der(char c) {
		return new Concat(regex->der(c), new Star(regex));
	}

	void print() {
		cout << "(";
		regex->print();
		cout << ")*";
	}
};

string decl_type(Empty *r) {
	if(auto reg = dynamic_cast<Epsilon*> (r); reg) {
		return "Epsilon";
	}
	if(auto reg = dynamic_cast<Star*> (r); reg) {
		return "Star";
	}
	if(auto reg = dynamic_cast<Concat*> (r); reg) {
		return "Concat";
	}
	if(auto reg = dynamic_cast<Char*> (r); reg) {
		return "Char";
	}
	if(auto reg = dynamic_cast<Alt*> (r); reg) {
		return "Alt";
	}
	return "Empty";
}

Empty *fit(Empty *r) {
	if(decl_type(r) == "Concat") {
		auto reg = dynamic_cast<Concat*> (r);
		Empty *r1 = reg->regex1, *r2 = reg->regex2;
		string t1 = decl_type(r1), t2 = decl_type(r2);

		if(t1 == "Empty") return fit(r2);
		if(t2 == "Empty") return fit(r1);

		if(t1 == "Epsilon") return fit(r2);
		if(t2 == "Epsilon") return fit(r1);
		// return new Concat(fit(r2), fit(r2));
		// optimization
	}

	if(decl_type(r) == "Alt") {
		auto reg = dynamic_cast<Alt*> (r);
		Empty *r1 = reg->regex1, *r2 = reg->regex2;
		string t1 = decl_type(r1), t2 = decl_type(r2);

		if(t1 == "Empty") return fit(r2);
		if(t2 == "Empty") return fit(r1);

		if(t1 == "Epsilon") return r2->nullable() ? fit(r2) : r;
		if(t2 == "Epsilon") return r1->nullable() ? fit(r1) : r;
		// return new Alt(fit(r1), fit(r2));
		// optimization
	}

	if(decl_type(r) == "Star") {
		auto reg = dynamic_cast<Star*> (r);
		Empty *rg = reg->regex;
		string type = decl_type(rg);

		if(type == "Empty") return new Empty();
		if(type == "Epsilon") return new Epsilon();
		if(type == "Star") return fit(rg);
		// return new Star(fit(rg));
		// optimization
	}
	return r;
}

Empty *derivative(string s, Empty *r) {
	if(s.size() == 0) {
		return r;
	}
	char c = s[0];
	s.erase(0, 1);
	auto tmp = r->der(c);
	auto tmp2 = fit(tmp);
	return derivative(s, tmp2);
}

bool match(Empty *r, string s) {
	return derivative(s, r)->nullable();
}

// (ab)*a
Empty *r1 = new Concat(new Star(new Concat(new Char('a'), new Char('b'))),
	   	new Char('a'));
//a*b
Empty *r2 = new Concat(new Star(new Char('a')), new Char('b'));

// (b|c)*
Empty *r3 = new Star(new Alt(new Char('b'), new Char('c')));

// (ab)*a(ab)*
Empty *r4 = new Concat(new Concat(new Star(new Concat(new Char('a'), new Char('b'))), new Char('a')),
	   	new Star(new Concat(new Char('a'), new Char('b'))));

// empty
Empty *r5 = new Empty();

// epsilon
Empty *r6 = new Epsilon();

vector<Empty *> regs = {r1,r2,r3,r4,r5,r6};

void tests() {
	assert(match(r1, "aaaaa") == false);
	assert(match(r1, "a") == true);
	assert(match(r1, "ababa") == true);

	assert(match(r2, "b") == true);
	assert(match(r2, "a") == false);
	assert(match(r2, "aaaab") == true);

	assert(match(r3, "cbcccbcb") == true);
	assert(match(r3, "cbcbacbcb") == false);
	assert(match(r3, "ccccccccc") == true);

	assert(match(r4, "a") == true);
	assert(match(r4, "ababa") == true);
	assert(match(r4, "aabab") == true);

	assert(match(r5, "") == false);
	assert(match(r5, "\n") == false);
	assert(match(r5, "f") == false);

	assert(match(r6, "") == true);
	assert(match(r6, "\t") == false);
	assert(match(r6, "aaaa") == false);
}

int main() {
	tests();
	int id;
	cout << "choose id of regex:\n";
	for(int i = 0; i < regs.size(); i++) {
		cout << "regex number " << (i+1) << ": ";
		regs[i]->print();
		cout << '\n';
	}

	cin >> id;
	id--;
	string s;
	cout << "test it ulimited:\n";
	getline(cin, s);
	while(getline(cin, s)) {
		if(match(regs[id], s)) {
			cout << "matched\n";
		} else {
			cout << "not matched\n";
		}
	}
}
