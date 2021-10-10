%{
#include <iostream>
#include "gram.tab.hpp"
unsigned int pos = 1;
using namespace std;
%}

%option yylineno
%option noyywrap

T_KEYWORD   Terminal
T_EDGE      ->
T_STATE     \([A-Za-z_0-9]+\)|@
T_WORD    [A-Za-z0-9]
T_N       [\n]

%%

 /*operators. */
{T_KEYWORD}      {pos += yyleng; yylval.word = yytext; return T_KEYWORD;}
{T_EDGE}         {pos += yyleng; yylval.word = yytext; return T_EDGE;}
{T_STATE}        {pos += yyleng; yylval.word = yytext; return T_STATE;}
{T_WORD}         {pos += yyleng; yylval.sym = yytext[0]; return T_WORD;}

 /* Ignore comments and whitespace. */
[\t]        {pos += 4;}
[ ]         {pos += 1;}
[\n]        {pos = 0; return T_N;}

<<EOF>>     {return 0;}

. {
  printf("LEXER ERROR: \"%s\" %u %u \n", yytext, yylineno, pos);
  exit(1);
}

%%