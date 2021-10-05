#ifndef MY__H__
#define MY__H__

#include <vector>
#include <map>
#include <string>
#include <iostream>
using std::vector, std::map, std::string, std::endl, std::cout;

struct Station {
	int id;
	char type;
	Station(int a = 0, char b = 0);
};

struct Edges {
	int id1, id2;
	vector<char> edges;
	Edges(int a, int b, vector<char> &c);
};


void resave(const vector<char> &symbs, vector<Station> &sts, const vector<Edges> &eds);

bool check(const string &s);
#endif
