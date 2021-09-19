%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum token {
  T_ID = 1, T_WORD = 2, T_OP = 3, 
  T_KW = 4, T_SYMB = 5
};
char c;
char buf[10];
%}

%option yylineno
%option noyywrap


KW     (points|start|terminal)
ID     [A-Za-z_][A-Za-z_0-9]*
WORD    [A-Za-z0-9]
OP     (->|[,:"])
SYMB   ;

%%

 /* Print identifiers, integers and operators. */
{KW}        {strncpy(buf, yytext, 10); return T_KW;}
{ID}        {return T_ID;}
{WORD}      {c = yytext[0]; return T_WORD; }
{OP}        {strncpy(buf, yytext, 2); return T_OP;}
{SYMB}      {c = yytext[0]; return T_SYMB;}

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
    if (t == T_WORD) {
      printf("T_WORD %c\n", c);
    } else if (t == T_ID) {
      printf("T_ID %s\n", yytext);
    } else if (t == T_OP) {
      //printf("T_OP %s\n", buf);
      printf("T_OP %s\n", yytext);
    } else if (t == T_KW){
      //printf("T_KW %s\n", buf);
      printf("T_KW %s\n", yytext);
    } else if (t == T_SYMB){
      printf("T_OP %c\n", c);
    } else if (t == T_WORD){
      printf("T_WORD %c\n", c);
    }
  };
  return EXIT_SUCCESS ;
}