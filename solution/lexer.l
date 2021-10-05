%{
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "parser.tab.hpp"
#include <iostream>
//#include <string.h>

enum Token {
    T_UNAVAIL,
    // T_INVALID_TOKEN,
    // T_STATE_QUANTITY,
    // T_LINK_QUANTITY,
    // T_ALPHABET_INIT_CHAR,
    // T_ALPHABET_CHAR,
    // T_STATE,
    // T_COMMA_SEPARATOR,
    // T_EOF
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
        // printf("Commas skipped: %d\n", commas_skipped);
        if (commas_skipped == 2) {
            response = T_ALPHABET_CHAR;
        }
        else {
            // std::cerr << "INVALID TOKEN\n";
            response = T_INVALID_TOKEN;
        }
    }
    else if (yylineno == ALPHABET_LINE) {
        response = T_ALPHABET_INIT_CHAR;
        // std::cerr << "ALPHABET INIT\n";
    }
    else {
        // std::cerr << "INVALID TOKEN\n";
        response = T_INVALID_TOKEN;
    }
    char * tmp = new char[strlen(buffer) + 1];
    strcpy(tmp, buffer);
    yylval.str_val = tmp;
    return response;
}

%%

// int main(int argc, char* argv[]) {
//     printf("in main\n");
//     for (;;) {
//         enum Token t = yylex();
//         switch (t) {
//             case T_STATE_QUANTITY:
//                 // printf("T_STATE_QUANTITY %d %u %d", total_states, yylineno, cur_char - (int)strlen(yytext));
//                 break;
//             case T_LINK_QUANTITY:
//                 printf("T_LINK_QUANTITY %d %u %d", total_links, yylineno, cur_char - (int)strlen(yytext));
//                 break;
//             case T_ALPHABET_CHAR:
//                 printf("T_ALPHABET_CHAR %s %u %d", buffer, yylineno, cur_char - (int)strlen(yytext));
//                 break;
//             case T_ALPHABET_INIT_CHAR:
//                 printf("T_ALPHABET_INIT_CHAR %s %u %d", buffer, yylineno, cur_char - (int)strlen(yytext));
//                 break;
//             case T_STATE:
//                 printf("T_STATE %s %u %d", buffer, yylineno, cur_char - (int)strlen(yytext));
//                 break;
//             case T_COMMA_SEPARATOR:
//                 printf("T_COMMA_SEPARATOR , %u %d", yylineno, cur_char - (int)strlen(yytext));
//                 break;
//             case T_EOF:
//                 return EXIT_SUCCESS;
//             case T_INVALID_TOKEN:
//                 printf("T_INVALID_TOKEN %s %u %d", buffer, yylineno, cur_char - (int)strlen(yytext));
//                 break;
//             default:
//                 printf("Unknown error!");
//                 return EXIT_FAILURE;
//         }
//         printf("\n"); 
//     }
//     return EXIT_SUCCESS;
// }