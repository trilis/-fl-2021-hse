#!/bin/sh
flex -o ./build/lex.yy.c ./build/lexer.l
gcc ./build/lex.yy.c -o ./build/lexer

ls ./tests/ -1 | while read file 
do
    ./build/lexer <./tests/$file >./testsout/$file.out
done
