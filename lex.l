%{
#include <stdio.h>
#include <stdlib.h>
#include "gram.tab.hpp"

int string_pos = 1;
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

{STATES}          {string_pos+=strlen(yytext); yylval.word = strdup(yytext); return STATES; }
{START}           {string_pos+=strlen(yytext); yylval.word = strdup(yytext); return START; }
{TERMINALS}       {string_pos+=strlen(yytext); yylval.word = strdup(yytext); return TERMINALS; }
{ALPHABET}        {string_pos+=strlen(yytext); yylval.word = strdup(yytext); return ALPHABET; }
{TRANSITIONS}     {string_pos+=strlen(yytext); yylval.word = strdup(yytext); return TRANSITIONS; }
{SEPARATOR}       {string_pos+=strlen(yytext); yylval.sym = strdup(yytext)[0]; return SEPARATOR; }
{SYMBOL}          {string_pos+=strlen(yytext); yylval.sym = strdup(yytext)[1]; return SYMBOL; }
{STATE}           {string_pos+=strlen(yytext); yylval.word = strdup(yytext); return STATE; }

[ \t\r]           {string_pos++;}
[\n]              {string_pos = 1;}

.                 {
                    printf("ERROR in line %u, pos %u, symbol %s\n", yylineno, string_pos, yytext);
                    exit(1);
                  }

%%