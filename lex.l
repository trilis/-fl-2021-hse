%{
#include <stdio.h>
#include <stdlib.h>
#include "gram.tab.hpp"

int n, position = 1, pos_before, t;
char c;
%}

%option yylineno
%option noyywrap

NEW_EDGE   ->
NUM        -?[1-9][0-9]*|0
SYMBOL     [A-Za-z]
VISUAL     [(),.]
BEGIN_EDGE [\[]
END_EDGE   [\]]

%%

 /* Print identifiers, integers and operators. */
{NUM}       {n = atoi(yytext);
	     pos_before = position; 
	     position += yyleng;
	     yylval.val = n;
	     printf("readed NUM\n");
	     return NUM; }
{NEW_EDGE}  {pos_before = position; 
	     position ++; 
	     yylval.sym = '>';
	     printf("readed NEW EDGE\n");
	     return NEW_EDGE;}
{SYMBOL}    {c = yytext[0]; 
	     pos_before = position;
	     position++;
	     yylval.sym = c;
	     printf("readed SYMBOL\n");
	     return SYMBOL;}     
{VISUAL}    {c = yytext[0]; 
	     pos_before = position; 
	     position++;
	     yylval.sym = c; 
	     printf("readed VISUAL\n");}
{BEGIN_EDGE} {pos_before = position; 
	      position++;
	      yylval.sym = '['; 
	      printf("readed BEGIN_EDGE\n");
	      return BEGINING; }
{END_EDGE}  {pos_before = position; 
	     position++;
	     yylval.sym = ']';
	     printf("readed END_EDGE\n");
	     return ENDING; }

 /* Ignore comments and whitespace. */
#[^\n]*     {printf("readed something\n");}
[ \t]       {position++;printf("readed t\n");}
[\r\n]      {position = 1;printf("readed r\n");}

<<EOF>>     {printf("(eof %u)\n", yylineno); return 0;}

. {
  printf("ERROR in line %u, symbol %s position %u\n", yylineno, yytext, position);
  exit(1);
}

%%
