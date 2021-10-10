#!/usr/bin/env bash
bison -d parser.ypp && \
flex flexer.l && \
g++ parser.tab.cpp lex.yy.c -o parser