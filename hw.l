%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int symbol=0;
%}

%option yylineno
%option noyywrap

ALPH       (([a-zA-Z0-9]|\.\.)+,)+
TERMINAL   (fls|tru)
STATE      \"[a-zA-Z0-9_><=]*\"

%%

 /* Print delimiters. */
[{]         {printf("{\n");symbol++;}
[}]         {printf("}\n");symbol++;}
[|]         {printf("|\n");symbol++;}

 /* Print identifiers, integers and operators. */

{ALPH}       {printf("(alph \"%s\" %u %d)\n",yytext, yylineno,symbol);symbol+=strlen(yytext);}
{TERMINAL}   {printf("(terminal \"%s\" %u %d)\n", yytext, yylineno,symbol);symbol+=strlen(yytext);}
{STATE}      {printf("(state \"%s\" %u %d)\n", yytext, yylineno,symbol);symbol+=strlen(yytext);}

 /* Ignore comments and whitespace. */
[\n]       {symbol=0;}
[ \t\r]    {symbol++;}
[///.*\n]  {}


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