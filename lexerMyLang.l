%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include "gramMyLang.tab.hpp"

int flag = 0, pointer = 1;
%}

%option yylineno
%option noyywrap


KW     (points|start|terminal)
ID     [A-Za-z_0-9]+
OP     (->|[,:";])

%%

 /* Print identifiers, integers and operators. */
{KW}        {
              pointer += strlen(yytext); 
              yylval.kw = yytext;
              return KW;
            }
{ID}        {
              pointer += strlen(yytext); 
              if (flag) { 
                yylval.word = yytext;
                return WORD; 
              }
              else { 
                yylval.ver = yytext;
                return VER; 
              }
            }
{OP}        {
              pointer += strlen(yytext); 
              if (yytext[0] == '"'){ flag ^= 1; }
              yylval.op = yytext;
              switch(yytext[0]){
                case ',': return COMMA;
                case ';': return SEMICOLON;
                case ':': return COLON;
                case '"': return QUOTE;
                default : return ARROW;
              }
            }

 /* Ignore comments and whitespace. */
#[^\n]*       {}
[ \t\r]     {++pointer;}
[\n]        {pointer = 1;}

<<EOF>>     {return 0;}

. {
  printf("ERROR in line %u, number symbol %u, symbol %s\n", yylineno, pointer, yytext);
  exit(1);
}

%%
