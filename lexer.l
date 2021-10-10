%{
#include <stdio.h>
#include <stdlib.h>
#include "parser.tab.hpp"

int num_chars = 0;
char* copy_yytext() {
    char *dest = new char[yyleng + 1];
    strcpy(dest, yytext);
    return dest;
}

%}

%option yylineno
%option noyywrap

%x INLINK

%%



\/\/[^\n]*      {}
[\r\n]      {num_chars = 0;}
[ ]         {++num_chars;}
\t       { num_chars += yyleng; return TAB; }
:     { num_chars += yyleng; return COLON; }
states       { num_chars += yyleng; return KEY_STATES; }
links       { num_chars += yyleng; return KEY_LINKS; }
beginning      { num_chars += yyleng; return SPEC_BEGIN; }
terminal        { num_chars += yyleng; return SPEC_TERMINAL; }
[A-Za-z_][A-Za-z_0-9]*        { num_chars += yyleng; yylval.word = copy_yytext(); return ID; }

<INITIAL>"[" { BEGIN(INLINK); num_chars += yyleng; return BRACKET_OPEN; }
<INLINK>"]>" { BEGIN(INITIAL); num_chars += yyleng; return BRACKET_CLOSE; }
<INLINK>[A-Za-z0-9\-] { num_chars += yyleng; yylval.sym = yytext[0]; return CHAR; };
<INLINK>. {
    printf("ERROR in line %u, position %d, symbol \"%s\" doesn't contain in allowed alphabet\n", yylineno, num_chars, yytext);
    exit(1);
}

. {
    printf("ERROR in line %u, position %d, symbol %s\n", yylineno, num_chars, yytext);
    exit(1);
}

%%
