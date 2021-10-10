%{
#include <iostream>
#include "gram.tab.hpp"
%}

%option yylineno
%option noyywrap

%%
q_[A-Za-z0-9]+ { 
    char *  buf = new char[strlen(yytext) + 1];
    strcpy(buf, yytext);
    yylval.val = buf;
    return STATE;
}

[A-Za-pr-z0-9][A-Za-z0-9]* {
    char *  buf = new char[strlen(yytext) + 1];
    strcpy(buf, yytext);
    yylval.val = buf;
    return ALPHABET;
}

[ \t\n]+ { }
"("      { yylval.sym = '('; return LP; }
")"      { yylval.sym = ')'; return RP; }
"["      { yylval.sym = '['; return LB; }
"]"      { yylval.sym = ']'; return RB; }
","      { yylval.sym = ','; return COMMA; }
<<EOF>>  { yylval.sym = 'e'; return 0; }

%%