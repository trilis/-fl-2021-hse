%{
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

enum automata_lex_state {
    INITIAL,
    READ_NAME,
    READ_STATES_KW,
    READ_STATES_DESCR,
    READ_START_KW,
    READ_START,
    READ_TRANSITIONS_KW,
    READ_TRANSITIONS_DESCR
};

enum global_bracket_state {
    EXPECT_OPEN, EXPECT_CLOSE, NONE
};

enum automata_lex_state state = INITIAL;
enum global_bracket_state bracket_state = NONE;
int num_chars = 0, open_cnt = 0, close_cnt = 0;

void recalc_num_chars(char * s) {
    num_chars += strlen(s);
}

%}

%option yylineno
%option noyywrap

LINE [ a-zA-Z0-9]*

WORD [a-zA-Z][a-zA-Z0-9]*

%%

 /* Print delimiters. */
[{] {
    ++open_cnt;
    if (open_cnt - close_cnt > 2) {
        printf("ERROR: unmatched opening bracket in line %d in symbol %d\n", yylineno, num_chars);
        return 1;
    }
    if (bracket_state != EXPECT_OPEN) {
        printf("ERROR: unexpected opening bracket in line %d in symbol %d\n", yylineno, num_chars);
        return 1;
    }
    printf("OPEN_BRACKET { %d %d\n", yylineno, num_chars);
    ++num_chars;
    bracket_state = EXPECT_CLOSE;
}

[}] {
    // TODO: make this piece of code do transitions between different automata lex states
    ++close_cnt;
    if (close_cnt > open_cnt) {
        printf("ERROR: unmatched closing bracket in line %d in symbol %d\n", yylineno, num_chars);
        return 1;
    }
    if (bracket_state != EXPECT_CLOSE) {
        printf("ERROR: unmatched closing bracket in line %d in symbol %d\n", yylineno, num_chars);
        return 1;
    }
    printf("CLOSE_BRACKET } %d %d\n", yylineno, num_chars);
    ++num_chars;
}

[,] {
    printf("COMMA ',' %d %d\n", yylineno, num_chars);
}

 /* Print identifiers, integers and operators. */

\n num_chars = 0;

. ++num_chars;

{WORD} {
	if (state == INITIAL) {
	    printf("ID %s %d %d\n", yytext, yylineno, num_chars);
	    state = READ_NAME;
	    bracket_state = EXPECT_OPEN;
	} else if (state == READ_NAME) {
	    if (strcmp(yytext, "states") != 0) {
	        printf("ERROR: expected 'states' keyword but got %s in line %d in symbol %d\n", yytext, yylineno, num_chars);
            return 1;
	    }
	    printf("KW %s %d %d\n", yytext, yylineno, num_chars);
	    state = READ_STATES_KW;
	    bracket_state = EXPECT_OPEN;
	} else if (state == READ_STATES_DESCR) {
	    if (strcmp(yytext, "start") != 0) {
	        printf("ERROR: expected 'start' keyword but got %s in line %d in symbol %d\n", yytext, yylineno, num_chars);
	        return 1;
	    }
	    printf("KW %s %d %d\n", yytext, yylineno, num_chars);
        state = READ_START_KW;
        bracket_state = EXPECT_OPEN;
	} else if (state == READ_START) {
        if (strcmp(yytext, "transitions") != 0) {
            printf("ERROR: expected 'transitions' keyword but got %s in line %d in symbol %d\n", yytext, yylineno, num_chars);
            return 1;
        }
        printf("KW %s %d %d\n", yytext, yylineno, num_chars);
        state = READ_TRANSITIONS_KW;
        bracket_state = EXPECT_OPEN;
    }
	recalc_num_chars(yytext);
}

<<EOF>>     {printf("(eof %u)\n", yylineno); return 0;}

%%

int main(int argc, char* argv[]) {
  yylex();
  return EXIT_SUCCESS ;
}