%{
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int num_chars = 0, open_cnt = 0, close_cnt = 0;

void recalc_num_chars(char * s) {
    num_chars += strlen(s);
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

ARROW_START --

ARROW_END -->

ARROW --[{]([a-zA-Z0-9\*],)*[a-zA-Z0-9,\*][}]-->

NUM 0|([1-9][0-9]*)

%%

 /* Print delimiters. */
[{] {
    ++open_cnt;
    //if (open_cnt - close_cnt > 2) {
    //    printf("ERROR: unmatched opening bracket in line %d in symbol %d\n", yylineno, num_chars);
    //    return 1;
    //}
    //if (bracket_state != EXPECT_OPEN) {
    //    printf("ERROR: unexpected opening bracket in line %d in symbol %d\n", yylineno, num_chars);
    //    return 1;
    //}
    printf("OPEN_BRACKET { %d %d\n", yylineno, num_chars);
    ++num_chars;
    //bracket_state = EXPECT_CLOSE;
}

[}] {
    // TODO: make this piece of code do transitions between different automata lex states (DEPRECATED)
    ++close_cnt;
    //if (close_cnt > open_cnt) {
    //    printf("ERROR: unmatched closing bracket in line %d in symbol %d\n", yylineno, num_chars);
    //    return 1;
    //}
    //if (bracket_state != EXPECT_CLOSE) {
    //    printf("ERROR: unmatched closing bracket in line %d in symbol %d\n", yylineno, num_chars);
    //    return 1;
    //}
    printf("CLOSE_BRACKET } %d %d\n", yylineno, num_chars);
    ++num_chars;
}

{ARROW_START} {
    printf("TRANSITION_DESCRIPTION_START %s %d %d\n", yytext, yylineno, num_chars);
    num_chars += 2;
}

{ARROW_END} {
    printf("TRANSITION_DESCRIPTION_END %s %d %d\n", yytext, yylineno, num_chars);
    num_chars += 3;
}

{ARROW} {
    printf("TRANSITION_DESCRIPTION %s %d %d\n", yytext, yylineno, num_chars);
    recalc_num_chars(yytext);
}

{KW} {
    printf("KW %s %d %d\n", yytext, yylineno, num_chars);
    // could use yyleng instead
    recalc_num_chars(yytext);
}

{NUM} {
    printf("NUMBER %s %d %d\n", yytext, yylineno, num_chars);
    recalc_num_chars(yytext);
}

[,] {
    printf("COMMA ',' %d %d\n", yylineno, num_chars);
    ++num_chars;
}

\n num_chars = 0;

{WORD} {
    printf("ID %s %d %d\n", yytext, yylineno, num_chars);
	recalc_num_chars(yytext);
}

 /* Ignore comments and whitespace. */
#[^\n]*     {}
[ \t\r\n]   { ++num_chars; }

[^ \t\r\n]+ {
    printf("Error: unknown token %s in line %d in symbol %d\n", yytext, yylineno, num_chars);
    return 1;
}

<<EOF>>     {printf("(eof %u)\n", yylineno); return 0;}

%%

int main(int argc, char* argv[]) {
  yylex();
  return EXIT_SUCCESS ;
}