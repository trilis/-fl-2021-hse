%{
#include <stdio.h>
#include <stdlib.h>
#include "gram.tab.hpp"
int id = 1;
%}

%option yylineno
%option noyywrap

T_NUMBER [0-9]+

%%
[!]         {id++; return EXCLAMATION_MARK;}
[@]         {id++; return AT_SIGN;}
[;]         {id++; return SEMICOLON;}
[,]         {id++; return COMMA;}
{T_NUMBER}  {id += strlen(yytext); yylval.num = atoi(yytext); return T_NUMBER;}

. {
printf("Error at id %d\n", id);
exit(1);
}

%%
