%{
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "parser.tab.hpp"
#include <iostream>
//#include <string.h>

enum Token {
    T_UNAVAIL,
};

enum { MAX_TOKEN_SIZE = 4 * (1 << 10) - 1};

/* one-based line enumeration */
const int TOTAL_STATES_LINE = 1;
const int ALPHABET_LINE = 2;
const int TOTAL_LINKS_LINE = 3;

int cur_char = 1;
int total_states = 0;
int total_links = 0;
int commas_skipped = 0;
char buffer[MAX_TOKEN_SIZE] = "\0";

%}

%option yylineno
%option noyywrap

UINT  (0|[1-9][0-9]*)
ALPHA ([^,\n]+)

%%


 /* Print delimiters. */
[,]         {
    commas_skipped++;
    cur_char += strlen(yytext);
    yylval.str_val = ",";
    return T_COMMA_SEPARATOR;
}

 /* Print identifiers, integers and operators. */

{UINT}       {
    int response = 0;
    if (yylineno == TOTAL_STATES_LINE) {
        total_states = atoi(yytext);
        yylval.num_val = total_states;
        response = T_STATE_QUANTITY;
    }
    else if (yylineno == TOTAL_LINKS_LINE) {
        total_links = atoi(yytext);
        yylval.num_val = total_links;
        response = T_LINK_QUANTITY;
    }
    else if (yylineno == ALPHABET_LINE) {
        strcpy(buffer, yytext);
        char * tmp = new char[strlen(buffer) + 1];
        strcpy(tmp, buffer);
        yylval.str_val = tmp;
        response =  T_ALPHABET_INIT_CHAR;
        // std::cerr << "ALPHABET INIT\n";
    }
    else if (yylineno > TOTAL_LINKS_LINE && yylineno <= TOTAL_LINKS_LINE + total_links) {
        strcpy(buffer, yytext);
        if (commas_skipped < 2) {
            yylval.num_val = atoi(buffer);
            response = T_STATE;
        }
        else {
            char * tmp = new char[strlen(buffer) + 1];
            strcpy(tmp, buffer);
            yylval.str_val = tmp;
            response = T_ALPHABET_CHAR;
        }
    }
    else {
        strcpy(buffer, yytext);
        yylval.num_val = atoi(buffer);
        response = T_STATE;
    }
    cur_char += strlen(yytext);
    return response;
}

 /* Ignore comments and whitespace. */
[\n] {
    cur_char = 1;
    commas_skipped = 0;
}

{ALPHA} {
    strcpy(buffer, yytext);
    cur_char += strlen(yytext);
    int response = 0;
    if (yylineno > TOTAL_LINKS_LINE && yylineno <= TOTAL_LINKS_LINE + total_links) {
        if (commas_skipped == 2) {
            response = T_ALPHABET_CHAR;
        }
        else {
            response = T_INVALID_TOKEN;
        }
    }
    else if (yylineno == ALPHABET_LINE) {
        response = T_ALPHABET_INIT_CHAR;
    }
    else {
        response = T_INVALID_TOKEN;
    }
    char * tmp = new char[strlen(buffer) + 1];
    strcpy(tmp, buffer);
    yylval.str_val = tmp;
    return response;
}

%%