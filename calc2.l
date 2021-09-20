%{
#include <stdio.h>
#include <stdlib.h>

enum token {
  T_ID = 1, T_INT = 2, T_OP = 3
};
int n;
char c;
%}

%option yylineno
%option noyywrap

ID     [A-Za-z_][A-Za-z_0-9]*
INT    -?[1-9][0-9]*
OP     [-+*/^=]

%%

 /* Print identifiers, integers and operators. */
{INT}       {n = atoi(yytext); return T_INT; }
{ID}        {return T_ID; }
{OP}        {c = yytext[0]; return T_OP;}

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
    if (t == T_INT) {
      printf("T_INT %u\n", n);
    } else if (t == T_ID) {
      printf("T_ID %s\n", yytext);
    } else if (t == T_OP) {
      printf("T_OP %c\n", c);
    }
  };
  return EXIT_SUCCESS ;
}