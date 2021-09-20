%{
#include <stdio.h>
#include <stdlib.h>

enum token {
  WORD = 1, T_INT = 2, T_OP = 3
};
int current_char = 0;
%}

%option yylineno
%option noyywrap

WORD [0-9a-zA-Z]*


%%

 /* Print identifiers, integers and operators. */
{INT} {n = atoi(yytext); return T_INT; }
{WORD} {

  return T_ID; 
}

 /* Ignore comments and whitespace. */
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