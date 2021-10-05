#!/bin/bash
bison gram.ypp
mv gram.tab.cpp gram.tab.hpp
flex lex.l
g++ lex.yy.c my.cpp
rm gram.tab.hpp
rm lex.yy.c
./a.out test str
rm a.out
