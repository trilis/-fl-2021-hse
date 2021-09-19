#!/bin/bash
flex calc.l
gcc -o lexer lex.yy.c
#lexer 