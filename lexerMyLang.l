%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum token {
  T_VER = 1, T_WORD = 2, T_OP = 3, T_KW = 4
};
int str_n, symb_n, pointer;
int flag;
%}

%option yylineno
%option noyywrap


KW     (points|start|terminal)
ID     [A-Za-z_0-9]+
OP     (->|[,:";])

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
              if (flag) { return T_WORD; }
              else { return T_VER; }
            }
{OP}        {
              if (yytext[0] == '"'){ flag ^= 1; }
              symb_n = pointer;
              pointer += strlen(yytext);
              str_n = yylineno;
              return T_OP;
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
  flag = 0;
  while (1) {
    enum token t = yylex();
    if (t == 0) {
      break;
    }
    if (t == T_VER) {
      printf("T_VER %s %u %u\n", yytext, str_n, symb_n);
    } else if (t == T_WORD) {
      printf("T_WORD %s %u %u\n", yytext, str_n, symb_n);
    } else if (t == T_OP) {
      printf("T_OP %s %u %u\n", yytext, str_n, symb_n);
    } else if (t == T_KW){
      printf("T_KW %s %u %u\n", yytext, str_n, symb_n);
    }  
  };
  return EXIT_SUCCESS ;
}