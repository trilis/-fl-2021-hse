%{
#include <stdio.h>
#include <stdlib.h>

enum token {
  T_DELIM = 1, T_STATE_ID = 2, T_OP = 3, T_TYPE = 4, T_JUMP_ARG = 5 
};
int n;
char c;
int arg;
%}

%option yylineno
%option noyywrap

DELIM    ___
STATE_ID [1-9][0-9]*
OP       :|->|,|\(|\)
TYPE     s|q|t|d
JUMP_ARG ([\(|,]\K[0-9]+)

%%

 /* Print identifiers, integers and operators. */
{DELIM}     {return T_DELIM;}
{STATE_ID}  {n = atoi(yytext); return T_STATE_ID;}
{OP}        {return T_OP;}
{TYPE}      {return T_TYPE;}
{JUMP_ARG}  {arg = atoi(yytext); return T_JUMP_ARG;}


 /* Ignore comments and whitespace. */
#[^\n]*     {}
[ \t\r\n]   {}

<<EOF>>     {printf("(eof %u)\n", yylineno); return 0;}

. {
  printf("ERROR in line %u, symbol %s\n", yylineno, yytext);
  exit(1);
}

%%

int main(int argc, char* argv[]) {
  while (1) {
    enum token t = yylex();
    if (t == 0) {
      break;
    }
    if (t == T_DELIM) {
      printf("T_DELIM %s\n", "___");
    } else if (t == T_STATE_ID) {
      printf("T_STATE_ID %u\n", n);
    } else if (t == T_OP) {
      printf("T_OP %s\n", yytext);
    } else if (t == T_TYPE) {
      printf("T_TYPE %s\n", yytext);
    } else if (t == T_JUMP_ARG) {
      printf("T_JUMP_ARG %u\n", arg);
    }
  };
  return EXIT_SUCCESS ;
}