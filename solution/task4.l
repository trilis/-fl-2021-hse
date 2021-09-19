%{
#include <stdio.h>
#include <stdlib.h>

enum token {
    T_SYMB = 1, T_STATE_NUMB, T_START_NODE, T_TERM_NODE, T_SRC_NODE, T_RSLT_NODE, T_ARG
};

int char_number = 1;
int argument_block = 0;
int source_block = 1;
%}

%option yylineno
%option noyywrap

ID     [A-Za-z_][A-Za-z_0-9]*
INT    (-[1-9])?[0-9]*
OP     [-+*/^=]
FUNC   ->


%%
\n {char_number = 1; source_block = 1;}

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

{INT}       {
if (yylineno == 1) {
    return T_SYMB;
}
if (yylineno == 2) {
    return T_STATE_NUMB;
}
if (yylineno == 3) {
    return T_START_NODE;
}
if (yylineno == 4) {
    return T_TERM_NODE;
}
if (yylineno > 4) {
    if (argument_block == 1) {
        return T_ARG;
    }
    else if (source_block == 1) {
        return T_SRC_NODE;
    }
    else {
        return T_RSLT_NODE;
    }
}
}


{FUNC}      {
printf("(arrow -> %u %u)\n", yylineno, char_number);
char_number += 2;
}

#[^\n]*     {}
[ \t\r]   {++char_number;}

<<EOF>>     {printf("(eof %u)\n", yylineno); return 0;}

. {
  printf("ERROR in line %u, char %u\n unexpected symbol", yylineno, char_number);
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
    } else if (t == T_STATE_NUMB) {
        printf("(states_number %s %u %u)\n", yytext, yylineno, char_number);
    } else if (t == T_START_NODE) {
        printf("(start_node_id %s %u %u)\n", yytext, yylineno, char_number);
    } else if (t == T_TERM_NODE) {
        printf("(terminate_node_id %s %u %u)\n", yytext, yylineno, char_number);
    } else if (t == T_ARG) {
        printf("(symbol_argument %s %u %u)\n", yytext, yylineno, char_number);
    } else if (t == T_SRC_NODE) {
        printf("(source_node %s %u %u)\n", yytext, yylineno, char_number);
    } else if (t == T_RSLT_NODE) {
        printf("(result_node %s %u %u)\n", yytext, yylineno, char_number);
    }
    ++char_number;
  };
    return EXIT_SUCCESS ;
}