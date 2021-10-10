%{
#include <iostream>
#include "gram.tab.hpp"

int lineno() {
	return yylineno;
}
%}

%option yylineno
%option noyywrap

EDGE EDGE
STATE STATE
ALPH_SIZE ALPH_SIZE
START_ST START_ST
ED_NUM ED_NUM
ST_NUM ST_NUM
NUM [0-9]+
STATE_DEF (term|notterm)
SEP [ \t\n,=]+
WORD \"[0-9a-zA-Z]+\"

%%


{EDGE}             { yylval.word = yytext; return EDGE; }
{ST_NUM}           { yylval.word = yytext; return ST_NUM; }
{STATE}            { yylval.word = yytext; return STATE; }
{ALPH_SIZE}        { yylval.word = yytext; return ALPH_SIZE; }
{START_ST}         { yylval.word = yytext; return START_ST; } 
{ED_NUM}           { yylval.word = yytext; return ED_NUM; }
{NUM}              { yylval.num = atoi(yytext); return NUM; }
{STATE_DEF}        { yylval.word = yytext; return STATE_DEF;}
{SEP}              { return SEP;}
{WORD}               { yylval.word = yytext; return WORD; }
<<EOF>>            { return 0; }

%%