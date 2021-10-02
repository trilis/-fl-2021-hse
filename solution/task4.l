%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum token {
    T_SYMB = 1,
    T_STATE_NUMB = 2,
    T_START_NODE = 3,
    T_TERM_NODE = 4,
    T_SRC_NODE = 5,
    T_RSLT_NODE = 6,
    T_ARG = 7
};

/* Константы  номеров строк в описании автомата */

const int ALPHABET_LINE = 1;
const int NUMBER_OF_STATES_LINE = 2;
const int START_NODE_LINE = 3;
const int TERM_NODES_LINE = 4;
const int FUNC_START_LINE = 5;

/* Переменные */

int char_number = 1;
int argument_block = 0;
int source_block = 0;

%}

%option yylineno
%option noyywrap

INT    (0|[1-9][0-9]*)
FUNC   ->
SYMB   ([^,\n:{}>-]+)

%%
\n {char_number = 1; source_block = (yylineno >= FUNC_START_LINE) ? 1 : 0;}

[,]         {
printf("(comma-delimiter %s %u %u)\n", yytext, yylineno, char_number);
++char_number;
}

[:]         {
printf("(colon %s %u %u)\n", yytext, yylineno, char_number);
++char_number;
source_block = 0;
}

[{]         {
printf("(left-brace %s %u %u)\n", yytext, yylineno, char_number);
argument_block = 1;
++char_number;
}

[}]         {
printf("(right-brace %s %u %u)\n", yytext, yylineno, char_number);
argument_block = 0;
++char_number;
}

{FUNC}      {
printf("(arrow -> %u %u)\n", yylineno, char_number);
char_number += 2;
} 

{INT}       {
if (yylineno == ALPHABET_LINE) {
    return T_SYMB;
}
if (yylineno == NUMBER_OF_STATES_LINE) {
    return T_STATE_NUMB;
}
if (yylineno == START_NODE_LINE) {
    return T_START_NODE;
}
if (yylineno == TERM_NODES_LINE) {
    return T_TERM_NODE;
}
if (yylineno >= FUNC_START_LINE) {
    if (source_block == 1) {
        return T_SRC_NODE;
    }
    if (argument_block == 1) {
        return  T_ARG;
    }
    else if (argument_block == 0) {
        return T_RSLT_NODE;
    }
}
}

<<EOF>>     {printf("(eof %u)\n", yylineno); return 0;}

{SYMB}      {
    if (yylineno == 1) {
        return T_SYMB;
    }
    if (argument_block == 1 && yylineno >= FUNC_START_LINE) {
        return T_ARG;
    }
    else {
        printf("ERROR in line %u, char %u: unexpected symbol %s\n", yylineno, char_number, yytext);
        exit(1);
    }
}

. {
    printf("ERROR in line %u, char %u: unexpected symbol %s\n", yylineno, char_number, yytext);
        exit(1);
} 

%%

int main(int argc, char* argv[]) {
    while (1) {
    enum token t = yylex();
    if (t == 0) {
      break;
    }
    if (t == T_SYMB) {
        printf("(alphabet_symbol %s %u %u)\n", yytext, yylineno, char_number);
        char_number += strlen(yytext);
    } else if (t == T_STATE_NUMB) {
        printf("(states_number %s %u %u)\n", yytext, yylineno, char_number);
        char_number += strlen(yytext);
    } else if (t == T_START_NODE) {
        printf("(start_node_id %s %u %u)\n", yytext, yylineno, char_number);
        char_number += strlen(yytext); 
    } else if (t == T_TERM_NODE) {
        printf("(terminate_node_id %s %u %u)\n", yytext, yylineno, char_number);
        char_number += strlen(yytext);
    } else if (t == T_ARG) {
        printf("(symbol_argument %s %u %u)\n", yytext, yylineno, char_number);
        char_number += strlen(yytext);
    } else if (t == T_SRC_NODE) {
        printf("(source_node %s %u %u)\n", yytext, yylineno, char_number);
        char_number += strlen(yytext);
    } else if (t == T_RSLT_NODE) {
        printf("(result_node %s %u %u)\n", yytext, yylineno, char_number);
        char_number += strlen(yytext);
    }
  };
    return EXIT_SUCCESS ;
}