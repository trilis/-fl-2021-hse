flex lex.l
bison -d gram.ypp
g++ -std=c++17 gram.tab.cpp lex.yy.c -o run