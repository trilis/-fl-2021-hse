#!/bin/sh
flex -o ./build/lex.yy.c ./build/lexer.l
gcc ./build/lex.yy.c -o lexer

ls ./tests/ -1 | while read file 
do
    ./lexer <./tests/$file >./testsout/$file.out
done
