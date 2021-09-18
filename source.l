%{
#include <stdio.h>
#include <stdlib.h>
int pos = 1;
%}

%option yylineno
%option noyywrap

START-VERTEX q0
VERTEX       q[0-9]+
CONDITION    I|T 
SYMBOL       n[0-9]+
COMMA        ,
OTHER        [^() \t\r\n]*

%%

 /* Print identifiers, integers and operators. */
{START-VERTEX}       {printf("start-vertex %s %u %d\n",yytext, yylineno, pos);
                      pos+=yyleng; }
{VERTEX}             {printf("vertex %s %u %d\n",yytext, yylineno, pos);
                      pos+=yyleng; }
{CONDITION}          {printf("condition %s %u %d\n",yytext, yylineno, pos);
                      pos+=yyleng; }
{SYMBOL}             {printf("symbol %s %u %d\n",yytext, yylineno, pos);
                      pos+=yyleng; }
{COMMA}              {printf("comma %s %u %d\n",yytext, yylineno, pos);
                      pos+=yyleng; }
[(]                  {printf("left-parenthesis %s %u %d\n",yytext, yylineno, pos);
                      pos+=yyleng; }
[)]                  {printf("right-parenthesis %s %u %d\n",yytext, yylineno, pos);
                      pos+=yyleng; }
{OTHER}              {printf("invalid parameter %s %u %d\n",yytext, yylineno, pos);
                      pos+=yyleng; }

 /* Ignore comments and whitespace. */
#[^\n]*    {}
[\t\r]     {}
[ ]        {pos+=1; }
[\n]       { pos = 1; }


%%

int main(int argc, char* argv[]) {
  yylex() ;
  return EXIT_SUCCESS ;
}