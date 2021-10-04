%{
#include <stdio.h>
#include <stdlib.h>
#include "gram.tab.hpp"
%}

%option yylineno
%option noyywrap

STATES      states
START       start
TERMINALS   terminals
ALPHABET    alphabet
TRANSITIONS transitions
SYMBOL      \'[A-Za-z_0-9]+\'
STATE       [A-Za-z_0-9]+
SEPARATOR   :|;

%%

{STATES}          {yylval.word = strdup(yytext); return STATES; }
{START}           {yylval.word = strdup(yytext); return START; }
{TERMINALS}       {yylval.word = strdup(yytext); return TERMINALS; }
{ALPHABET}        {yylval.word = strdup(yytext); return ALPHABET; }
{TRANSITIONS}     {yylval.word = strdup(yytext); return TRANSITIONS; }
{SEPARATOR}       {yylval.sym = strdup(yytext)[0]; return SEPARATOR; }
{SYMBOL}          {yylval.sym = strdup(yytext)[1]; return SYMBOL; }
{STATE}           {yylval.word = strdup(yytext); return STATE; }

[ \t\r\n]+   { }

%%