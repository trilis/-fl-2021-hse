%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum token {
  T_ID = 1, T_WORD = 2, T_OP = 3, 
  T_KW = 4, T_SYMB = 5
};
int str_n, symb_n, pointer;
char c;
char buf[10];
%}

%option yylineno
%option noyywrap


KW     (points|start|terminal)
ID     [A-Za-z_][A-Za-z_0-9]*
WORD   [A-Za-z0-9]
OP     (->|[,:"])
SYMB   ;

%%

 /* Print identifiers, integers and operators. */
{KW}        {
              symb_n = pointer;
              pointer += strlen(yytext); 
              str_n = yylineno; 
              return T_KW;
            }
{ID}        {
              symb_n = pointer;
              pointer += strlen(yytext); 
              str_n = yylineno; 
              return T_ID;
            }
{WORD}      {
              symb_n = pointer;
              c = yytext[0];
              ++pointer;
              str_n = yylineno; 
              return T_WORD; 
            }
{OP}        {
              symb_n = pointer;
              strncpy(buf, yytext, 2);
              pointer += strlen(yytext);
              str_n = yylineno;
              return T_OP;
              }
{SYMB}      {
              symb_n = pointer;
              c = yytext[0]; 
              ++pointer;
              str_n = yylineno; 
              return T_SYMB;
            }

 /* Ignore comments and whitespace. */
#[^\n]*     {}
[ \t\r]     {++pointer;}
[\n]        {pointer = 1;}

<<EOF>>     {printf("(eof %u)\n", yylineno); return 0;}

. {
  printf("ERROR in line %u, number symbol %u, symbol %s\n", yylineno, pointer, yytext);
  exit(1);
}

%%

int main(int argc, char* argv[]) {
  pointer = 1;
  while (1) {
    enum token t = yylex();
    if (t == 0) {
      break;
    }
    if (t == T_WORD) {
      printf("T_WORD %c %u %u\n", c, str_n, symb_n);
    } else if (t == T_ID) {
      printf("T_ID %s %u %u\n", yytext, str_n, symb_n);
    } else if (t == T_OP) {
      printf("T_OP %s %u %u\n", yytext, str_n, symb_n);
    } else if (t == T_KW){
      printf("T_KW %s %u %u\n", yytext, str_n, symb_n);
    } else if (t == T_SYMB){
      printf("T_OP %c %u %u\n", c, str_n, symb_n);
    } else if (t == T_WORD){
      printf("T_WORD %c %u %u\n", c, str_n, symb_n);
    }
  };
  return EXIT_SUCCESS ;
}