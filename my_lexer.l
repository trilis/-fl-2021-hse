%{
#include <stdio.h>
#include <stdlib.h>

enum token {
    T_TYPE = 1, T_LETTER = 2, T_INT = 3, T_OP = 4, T_OPEN_FIG_SCOB = 5, 
    T_CLOSE_FIG_SCOB = 6, T_OPEN_SCOB = 7, T_CLOSE_SCOB = 8, T_CONDS = 9
};
int n;
char c;
char s[128];
%}

%option yylineno
%option noyywrap

CONDS  (start)|(terminate)|(stock)
TYPE   (cond)|(alph)|(edge)
LETTER     [A-Za-z_0-9]*
INT    -?[1-9][0-9]*
OP     [-+*/^=,]


%%
 /* Print identifiers, integers and operators. */

{CONDS}     {return T_CONDS; }
{TYPE}      {return T_TYPE; }
{INT}       {n = atoi(yytext); return T_INT; }
{LETTER}        {return T_LETTER; }
{OP}        {c = yytext[0]; return T_OP; }

"}"     {return T_CLOSE_FIG_SCOB; }
"{"     {return T_OPEN_FIG_SCOB; }
")"     {return T_CLOSE_SCOB; }
"("     {return T_OPEN_SCOB; }


 /* Ignore comments and whitespace. */
#[^\n]*     {}
[ \t\r\n]*  {}
\/\/[^\n]* {}

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
    if (t == T_TYPE) {
      printf("T_TYPE %s line: %u\n", yytext, yylineno);
    } else if (t == T_INT) {
      printf("T_INT %u line: %u\n", n, yylineno);
    } else if (t == T_LETTER) {
      printf("T_LETTER %s line: %u\n", yytext, yylineno);
    } else if (t == T_OP) {
      printf("T_OP %c line: %u\n", c, yylineno);
    } else if (t == T_CLOSE_FIG_SCOB) {
      printf("T_CLOSE_FIG_SCOB %s line: %u\n", yytext, yylineno);
    } else if (t == T_OPEN_FIG_SCOB) {
      printf("T_OPEN_FIG_SCOB %s line: %u\n", yytext, yylineno);
    } else if (t == T_CLOSE_SCOB) {
      printf("T_CLOSE_SCOB %s line: %u\n", yytext, yylineno);
    } else if (t == T_OPEN_SCOB) {
      printf("T_OPEN_SCOB %s line: %u\n", yytext, yylineno);
    } else if (t == T_CONDS) {
      printf("T_CONDS %s line: %u\n", yytext, yylineno);
    }
  };
  return EXIT_SUCCESS ;
}