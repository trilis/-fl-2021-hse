%{
#include <stdio.h>
#include <stdlib.h>
#include "gram.tab.hpp"

%}

%option yylineno
%option noyywrap

DELIM     ___
NODE      [1-9][0-9]*
COLON     :
COMMA     ,
ARROW     ->
NODE_TYPE_ST   s
NODE_TYPE_US   q
NODE_TYPE_TERM t
NODE_TYPE_DR   d
ARGS      \(.*\)

%%

 /* Print identifiers, integers and operators. */
{NODE}           {yylval.num = atoi(yytext); return NODE;}
{DELIM}          {strcpy(yylval.str, yytext); return DELIM;}
{COLON}          {yylval.c = yytext[0]; return COLON;}
{COMMA}          {yylval.c = yytext[0]; return COMMA;}
{ARROW}          {strcpy(yylval.str, yytext); return ARROW;}
{NODE_TYPE_ST}   {yylval.c = yytext[0]; return NODE_TYPE_ST;}
{NODE_TYPE_TERM} {yylval.c = yytext[0]; return NODE_TYPE_TERM;}
{NODE_TYPE_US}   {yylval.c = yytext[0]; return NODE_TYPE_US;}
{NODE_TYPE_DR}   {yylval.c = yytext[0]; return NODE_TYPE_DR;}
{ARGS}           {strcpy(yylval.str, yytext); return ARGS;}



 /* Ignore comments and whitespace. */
#[^\n]*     {}
[ \t\r\n]   {}

<<EOF>>     {return 0;}

. {
  printf("ERROR in line %u, symbol %s\n", yylineno, yytext);
  exit(1);
}

%%
