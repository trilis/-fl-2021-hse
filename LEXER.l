%{
#include <stdio.h>
#include <stdlib.h>
unsigned int pos = 1;
%}

%option yylineno
%option noyywrap

T_KEYWORD   (Sigma|Terminal)
T_EDGE      ->
T_STATE     \([A-Za-z_0-9]+\)|@
T_WORD    [A-Za-z_0-9]+

%%

 /*operators. */
{T_KEYWORD}      {printf("T_KEYWORD %s %u %u\n", yytext, yylineno, pos); pos += yyleng;}
{T_EDGE}         {printf("T_EDGE %s %u %u\n", yytext, yylineno, pos); pos = pos + yyleng;}
{T_STATE}        {printf("T_STATE %s %u %u\n", yytext, yylineno, pos); pos += yyleng;}
{T_WORD}       {printf("T_WORD %s %u %u\n", yytext, yylineno, pos); pos += yyleng;}

 /* Ignore comments and whitespace. */
[\t]        {pos += 4;}
[ ]         {pos += 1;}
[\n]        {pos = 0;}

<<EOF>>     {printf("(EOF %u)\n", yylineno); return 0;}

. {
  printf("ERROR: \"%s\" %u %u \n", yytext, yylineno, pos);
  exit(1);
}

%%

int main(int argc, char* argv[]) {
  yylex() ;
  return EXIT_SUCCESS ;
}