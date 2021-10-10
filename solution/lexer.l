%{
#include <stdio.h>
#include <stdlib.h>
#include "parser.tab.hpp"
%}

%option yylineno
%option noyywrap

STATES      states:
START       start:
TERMINALS   terminals:
ALPHABET    alphabet:
TRANSITIONS transitions:
LETTER      \'[A-Za-z_0-9]\'
STATE_NAME       [A-Za-z_0-9]+
SEMICOLON   ;

%%

{STATES}          {yylval.val = strdup(yytext); return STATES; }
{START}           {yylval.val = strdup(yytext); return START; }
{TERMINALS}       {yylval.val = strdup(yytext); return TERMINALS; }
{ALPHABET}        {yylval.val = strdup(yytext); return ALPHABET; }
{TRANSITIONS}     {yylval.val = strdup(yytext); return TRANSITIONS; }
{SEMICOLON}       {yylval.sym = strdup(yytext)[0]; return SEMICOLON; }
{LETTER}          {yylval.sym = strdup(yytext)[1]; return LETTER; }
{STATE_NAME}      {yylval.val = strdup(yytext); return STATE_NAME; }

[ \t\r]           {}
[\n]              {}

.                 {
                    printf("ERROR in line %u, symbol %s\n", yylineno, yytext);
                    exit(1);
                  }

%%