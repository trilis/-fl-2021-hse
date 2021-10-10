%{
#include <stdio.h>
#include <stdlib.h>
#include "parser.tab.hpp"

int num_chars = 0;
%}

%option yylineno
%option noyywrap

%x INLINK

%%


\/\/[^\n]*      {}
[\r\n]      {num_chars = 0;}
[ ]         {++num_chars;}

\t       { num_chars += yyleng; yylval.sym = yytext[0]; return TAB; }
:     { num_chars += yyleng; yylval.sym = yytext[0]; return COLON; }
states       { num_chars += yyleng; yylval.word = yytext; return KEY_STATES; }
links       { num_chars += yyleng; yylval.word = yytext; return KEY_LINKS; }

beginning      { num_chars += yyleng; yylval.word = yytext; return SPEC_BEGIN; }
terminal        { num_chars += yyleng; yylval.word = yytext; return SPEC_TERMINAL; }

[A-Za-z_][A-Za-z_0-9]*        { num_chars += yyleng; yylval.sym = yytext[0]; return ID; }

<INITIAL>"[" { BEGIN(INLINK); num_chars += yyleng; yylval.word = yytext; return BRACKET_OPEN; }
<INLINK>"]>" { BEGIN(INITIAL); num_chars += yyleng; yylval.word = yytext; return BRACKET_CLOSE; }
<INLINK>[A-Za-z0-9\-] { num_chars += yyleng; yylval.sym = yytext[0]; return CHAR; };
<INLINK>. {
    printf("ERROR in line %u, position %d, symbol \"%s\" doesn't contain in allowed alphabet\n", yylineno, num_chars, yytext);
    exit(1);
}

<<EOF>>     {return 0;}

. {
    printf("ERROR in line %u, position %d, symbol %s\n", yylineno, num_chars, yytext);
    exit(1);
}

%%
