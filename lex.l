%{
#include <stdio.h>
#include <stdlib.h>
#include "gram.tab.hpp"
%}

%option yylineno
%option noyywrap

VERTEX       q[0-9]+
CONDITION    s|n|t|u
EDGE         e
SYMBOL      [0-9a-zA-Z]

%%

[{]                  { return LP; }
[}]                  { return RP; }
[,]                  { return COMMA; }

[ \t\n]+        {}

 /* Print identifiers, integers and operators. */
{VERTEX}             { yylval.str =  strdup(yytext); return VERTEX; }
{CONDITION}          { yylval.sym =  yytext[0]; return CONDITION; }
{EDGE}               { yylval.sym =  yytext[0]; return EDGE; }
{SYMBOL}             { yylval.sym =  yytext[0]; return SYMBOL; }


<<EOF>>     { return 0; }

. {
  printf("error %u %s\n", yylineno, yytext);
  exit(1);
}

%%