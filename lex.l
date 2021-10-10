%{
#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include "./gram_types.h"

#include "gram.tab.hpp"

int num_chars = 0;

void recalc_num_chars() {
    num_chars += yyleng;
}

char * new_token() {
    char *dest = new char[yyleng + 1];
    strcpy(dest, yytext);
    return dest;
}

%}

%option yylineno
%option noyywrap

TRANSITIONS_KW transitions

START_KW start

STATES_KW states

TERMINAL_KW terminal

WORD [a-zA-Z][a-zA-Z0-9]*

SYMBOL \'[A-Za-z_0-9]+\'

STAR \*

ARROW_START --[{]

ARROW_END [}]-->

ARROW --[{]([a-zA-Z0-9\*],)*[a-zA-Z0-9,\*][}]-->

NUM 0|([1-9][0-9]*)

%%

[{] {
    yylval.sym = '{';
    return LEFT_PAREN;
}

[}] {
    yylval.sym = '}';
    return RIGHT_PAREN;
}

{ARROW_START} {
    yylval.word = new_token();
    return ARROW_START;
}

{ARROW_END} {
    yylval.word = new_token();
    return ARROW_END;
}

{TRANSITIONS_KW} {
    yylval.word = new_token();
    return TRANSITIONS_KW;
}

{START_KW} {
    yylval.word = new_token();
    return START_KW;
}

{STATES_KW} {
    yylval.word = new_token();
    return STATES_KW;
}

{TERMINAL_KW} {
    yylval.word = new_token();
    return TERMINAL_KW;
}

{STAR} {
    yylval.sym = '*';
    return STAR;
}

[,] {
    yylval.sym = ',';
    return COMMA;
}

\n num_chars = 0;

{SYMBOL} {
    yylval.sym = yytext[1];
    return SYMBOL;
}

{WORD} {
    yylval.word = new_token();
    return ID;
}

[ \t\r\n]   { recalc_num_chars();; }

[.] {
    printf("Error: unknown token in line %d in symbol %d\n", yylineno, num_chars);
    return 1;
}

<<EOF>>     {return 0;}
