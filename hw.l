%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int symbol=0;
%}

%option yylineno
%option noyywrap

ALPH         ([a-zA-Z0-9])
STATE        \"[a-zA-Z0-9_><=]*\"
TERMINAL     (fls|tru)
RIGHTBRACE   \} 
LEFTBRACE    \{
DOTS         \.\.
COMMA        \,
SEP          \|   

%%

 /* Print identifiers, integers and operators. */

{ALPH}         {printf("(alph \"%s\" %u %d)\n",yytext, yylineno,symbol);symbol+=strlen(yytext);}
{TERMINAL}     {printf("(terminal \"%s\" %u %d)\n", yytext, yylineno,symbol);symbol+=strlen(yytext);}
{STATE}        {printf("(state \"%s\" %u %d)\n", yytext, yylineno,symbol);symbol+=strlen(yytext);}
{RIGHTBRACE}   {printf("(right_brace \"%s\" %u %d)\n", yytext, yylineno,symbol);symbol+=strlen(yytext);}
{LEFTBRACE}    {printf("(left_brace \"%s\" %u %d)\n", yytext, yylineno,symbol);symbol+=strlen(yytext);}
{COMMA}        {printf("(comma \"%s\" %u %d)\n", yytext, yylineno,symbol);symbol+=strlen(yytext);}
{SEP}          {printf("(sep \"%s\" %u %d)\n", yytext, yylineno,symbol);symbol+=strlen(yytext);}
{DOTS}         {printf("(dots \"%s\" %u %d)\n", yytext, yylineno,symbol);symbol+=strlen(yytext);}


[\n]                    {symbol=0;}
[ \t\r]                 {symbol++;}



<<EOF>>     {printf("(eof %u)\n", yylineno); return 0;}

. {
  printf("ERROR in line %u, symbol %s %d\n", yylineno, yytext, symbol);
  exit(1);
}

%%

int main(int argc, char* argv[]) {
  yylex() ;
  return EXIT_SUCCESS ;
}