#!/usr/bin/bash
bison -d parser.ypp
flex -o lexer.c lexer.l
g++ -std=c++17 parser.tab.cpp lexer.c -o executable.out