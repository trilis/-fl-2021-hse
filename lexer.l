%{
#include <stdio.h>
#include <stdlib.h>
int pos = 1;
%}

%option yylineno
%option noyywrap

VERTEX       q[0-9]+
CONDITION    s|n|t 
EDGE         e
SYMBOL      symb[0-9,a-z,A-Z]+

%%

[{]                  {printf("left-parenthesis %s %u %d\n",yytext, yylineno, pos);
                      pos+=yyleng; }
[}]                  {printf("right-parenthesis %s %u %d\n",yytext, yylineno, pos);
                      pos+=yyleng; }
[,]                 {printf("comma %s %u %d\n",yytext, yylineno, pos);
                      pos+=yyleng; }

[\t]        {}
[ ]        {pos++; }
[\n]       { pos = 1; }

 /* Print identifiers, integers and operators. */
{VERTEX}             {printf("vertex %s %u %d\n",yytext, yylineno, pos);
                      pos+=yyleng; }
{CONDITION}          {printf("condition %s %u %d\n",yytext, yylineno, pos);
                      pos+=yyleng; }
{EDGE}              {printf("edge %s %u %d\n",yytext, yylineno, pos);
                      pos+=yyleng; }
{SYMBOL}             {printf("symbol %s %u %d\n",yytext, yylineno, pos);
                      pos+=yyleng; }


#[^\n]*    {}

<<EOF>>     { printf("(eof %u)\n", yylineno); return 0; }

. {
  printf("error %u %d %s\n", yylineno, pos, yytext);
  exit(1);
}

%%

int main(int argc, char* argv[]) {
  yylex() ;
  return EXIT_SUCCESS ;
}