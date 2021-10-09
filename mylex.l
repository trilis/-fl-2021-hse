%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mygram.tab.hpp"

int symbol=0;
FILE *log=fopen("log.txt", "w");
%}

%option yylineno
%option noyywrap
 

%%

 /* Print identifiers, integers and operators. */

([a-zA-Z0-9])         {yylval.sym = yytext[0];fprintf(log,"(alph \"%s\" %u %d)\n",yytext, yylineno,symbol);symbol+=strlen(yytext);return ALPH;}
\"[a-zA-Z0-9_><=]*\"  {char*  buf = new char[strlen(yytext)+1];strcpy(buf, yytext);yylval.str = buf;fprintf(log,"(state \"%s\" %u %d)\n", yytext, yylineno,symbol);symbol+=strlen(yytext);return STATE;}
(fls|tru)             {char*  buf = new char[strlen(yytext)+1];strcpy(buf, yytext);yylval.str = buf;fprintf(log,"(terminal \"%s\" %u %d)\n", yytext, yylineno,symbol);symbol+=strlen(yytext);return TERMINAL;}
\}                    {fprintf(log,"(right_brace \"%s\" %u %d)\n", yytext, yylineno,symbol);symbol+=strlen(yytext);return RB;}
\{                    {fprintf(log,"(left_brace \"%s\" %u %d)\n", yytext, yylineno,symbol);symbol+=strlen(yytext);return LB;}
\,                    {fprintf(log,"(comma \"%s\" %u %d)\n", yytext, yylineno,symbol);symbol+=strlen(yytext);return COMMA;}
\|                    {fprintf(log,"(sep \"%s\" %u %d)\n", yytext, yylineno,symbol);symbol+=strlen(yytext);return SEP;}
\.\.                  {fprintf(log,"(dots \"%s\" %u %d)\n", yytext, yylineno,symbol);symbol+=strlen(yytext);return DOTS;}
[\n]                  {symbol=0;}
[ \t\r]+              {symbol++;}



<<EOF>>     {printf("(eof %u)\n", yylineno); return 0;}

. {
  fprintf(log,"ERROR in line %u, symbol %s %d\n", yylineno, yytext, symbol);
  exit(1);
}

%%
