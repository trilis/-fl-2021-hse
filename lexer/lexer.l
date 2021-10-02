%{
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int num_chars = 0, open_cnt = 0, close_cnt = 0;

void recalc_num_chars() {
    num_chars += yyleng;
}

%}

%option yylineno
%option noyywrap

TRANSITIONS_KW transitions

START_KW start

STATES_KW states

TERMINAL_KW terminal

KW (start|terminal|transitions|states)

WORD [a-zA-Z][a-zA-Z0-9]*

SYMBOL [a-zA-Z\*]

ARROW_START --[{]

ARROW_END [}]-->

ARROW --[{]([a-zA-Z0-9\*],)*[a-zA-Z0-9,\*][}]-->

NUM 0|([1-9][0-9]*)

%%

 /* Print delimiters. */
[{] {
    ++open_cnt;
    printf("OPEN_BRACKET { %d %d\n", yylineno, num_chars);
    recalc_num_chars();
}

[}] {
    ++close_cnt;
    printf("CLOSE_BRACKET } %d %d\n", yylineno, num_chars);
    ++num_chars;
}

{ARROW_START} {
    printf("TRANSITION_DESCRIPTION_START %s %d %d\n", yytext, yylineno, num_chars);
    recalc_num_chars();
}

{ARROW_END} {
    printf("TRANSITION_DESCRIPTION_END %s %d %d\n", yytext, yylineno, num_chars);
    recalc_num_chars();
}

{KW} {
    printf("KW %s %d %d\n", yytext, yylineno, num_chars);
    recalc_num_chars();
}

{NUM} {
    printf("NUMBER %s %d %d\n", yytext, yylineno, num_chars);
    recalc_num_chars();
}

[,] {
    printf("COMMA ',' %d %d\n", yylineno, num_chars);
    recalc_num_chars();
}

\n num_chars = 0;

{SYMBOL} {
    printf("SYMBOL %s %d %d\n", yytext, yylineno, num_chars);
	recalc_num_chars();
}

{WORD} {
    printf("WORD %s %d %d\n", yytext, yylineno, num_chars);
	recalc_num_chars();
}

[ \t\r\n]   { recalc_num_chars();; }

[.] {
    printf("Error: unknown token in line %d in symbol %d\n", yylineno, num_chars);
    return 1;
}

<<EOF>>     {printf("(eof %u)\n", yylineno); return 0;}

%%

int main(int argc, char* argv[]) {
  yylex();
  return EXIT_SUCCESS ;
}