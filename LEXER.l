%{
#include <stdio.h>
#include <stdlib.h>
unsigned int pos = 0;
%}

%option yylineno
%option noyywrap

KEYWORD   (Sigma|Terminal)
EDGE      ->
STATE     \([A-Za-z_0-9]+\)|@
WORD    [A-Za-z_0-9]+

%%

 /*operators. */
{KEYWORD}      {printf("KEYWORD %s %u %u\n", yytext, yylineno, pos); pos += yyleng;}
{EDGE}         {printf("EDGE %s %u %u\n", yytext, yylineno, pos); pos = pos + yyleng;}
{STATE}        {printf("STATE %s %u %u\n", yytext, yylineno, pos); pos += yyleng;}
{WORD}       {printf("WORD %s %u %u\n", yytext, yylineno, pos); pos += yyleng;}

 /* Ignore comments and whitespace. */
[\t]        {pos += 4;}
[ ]         {pos += 1;}
[\n]        {pos = 0;}

<<EOF>>     {printf("(EOF %u)\n", yylineno); return 0;}

. {
  printf("ERROR (INVALID SYMBOL): \"%s\" %u %u \n", yytext, yylineno, pos);
  pos += yyleng;
}

%%

int main(int argc, char* argv[]) {
  yylex() ;
  return EXIT_SUCCESS ;
}