#!/usr/bin/env bash
bison -d gram.ypp
flex lex.l
g++ gram.tab.cpp lex.yy.c -lfl
./a.out dfa.txt word.txt