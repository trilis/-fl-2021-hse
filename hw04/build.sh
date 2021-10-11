#!/usr/bin/env bash
bison -d parser.ypp && \
flex flexer.l && \
g++ -std=c++17 parser.tab.cpp lex.yy.c -o parser