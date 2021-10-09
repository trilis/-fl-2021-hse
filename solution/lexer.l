%{
#include <stdio.h>
#include <stdlib.h>
#include "parser.tab.hpp"
%}

%option yylineno
%option noyywrap


STATES       states:
START        start:
TERMINALS    terminals:
ALPHABET     alphabet:
TRANSITIONS  transitions:
STATE_NAME   [0-9a-zA-Z]+
LETTER      \'[0-9a-zA-Z]\' 
SEMICOLON    ;

%%


 /* Print identifiers, integers and operators. */

{STATES}              {yylval.val = strdup(yytext); return STATES; }
{START}               {yylval.val = strdup(yytext); return START;}
{TERMINALS}           {yylval.val = strdup(yytext); return TERMINALS;}
{ALPHABET}            {yylval.val = strdup(yytext); return ALPHABET;}
{TRANSITIONS}         {yylval.val = strdup(yytext); return TRANSITIONS;}
{STATE_NAME}          {yylval.val = strdup(yytext); return STATE_NAME;}
{LETTER}              {yylval.sym = strdup(yytext)[0]; return LETTER;}
{SEMICOLON}           {yylval.sym = strdup(yytext)[1]; return SEMICOLON;}


 /* Ignore comments and whitespace. */
#[^\n]*     {}
[ \t\r\n]   {}

<<EOF>>     {printf("(eof %u)\n", yylineno); return 0;}

%%
