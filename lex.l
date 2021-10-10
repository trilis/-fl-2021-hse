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

    printf("T_STATE %s\n", yylval.val);
    return STATE;
}

[A-Za-pr-z0-9][A-Za-z0-9]* {
    char *  buf = new char[strlen(yytext) + 1];
    strcpy(buf, yytext);
    yylval.val = buf;

    printf("T_ALPHABET %s\n", yylval.val);
    return ALPHABET;
}

[ \t\n]+ { }
"("      { yylval.sym = '('; printf("T_LP\n"); return LP; }
")"      { yylval.sym = ')'; printf("T_RP\n"); return RP; }
"["      { yylval.sym = '['; printf("T_LB\n"); return LB; }
"]"      { yylval.sym = ']'; printf("T_RB\n"); return RB; }
","      { yylval.sym = ','; printf("T_COMMA\n"); return COMMA; }
<<EOF>>  { yylval.sym = 'e'; return 0; }

%%