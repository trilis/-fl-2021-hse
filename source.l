%{
#include <stdio.h>
#include <stdlib.h>
#include "automata.tab.hpp"
int pos = 1;
%}

%option yylineno
%option noyywrap

START-VERTEX q0
VERTEX       q[0-9]+
VERTEX_N     Q
ALPHABET_N   A
CONDITION    I|T 
COMMA        ,
LETTER       [a-zA-Z0-9]+
EQUEL        =
COLON        :
OTHER        [^() \t\r\n]*

%%

 /* Print identifiers, integers and operators. */
{START-VERTEX}       {  yylval.val = 0; return SVERTEX; }
{VERTEX}             {  yylval.val = atoi(yytext+1); return VERTEX;}
{CONDITION}          {  yylval.sym = yytext[0]; return CONDITION; }
{VERTEX_N}           {  return Q; }
{ALPHABET_N}         {  return A; }
{COLON}              {  return COL; }
{LETTER}             {  yylval.string = strndup(yytext, 100); return LETTER; }
{COMMA}              {  return COMMA; }
[(]                  {  return LP; }
[)]                  {  return RP; }
{OTHER}              {  return OTHER; }

 /* Ignore comments and whitespace. */
#[^\n]*    {}
[\r\n]     {pos = 1; }
[ \t]      {pos+=1; }


%%
