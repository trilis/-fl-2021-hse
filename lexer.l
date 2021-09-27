%{
#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
enum token {
    T_SPEC = 1, T_KEY = 2, T_ID = 3, T_CHAR = 4, T_COLON = 5, T_TAB = 6, T_BRACKET = 7
};
int num_chars = 0;
%}

%option yylineno
%option noyywrap

%x INLINK

SPEC    beginning|terminal
KEY       states|links
ID      [A-Za-z_][A-Za-z_0-9]*
COLON   :
TAB     \t


%%


\/\/[^\n]*      {}
[\r\n]      {num_chars = 0;}
[ ]         {++num_chars;}

{TAB}       {return T_TAB;}
{COLON}     {return T_COLON;}
{KEY}       {return T_KEY;}
{SPEC}      {return T_SPEC;}
{ID}        {return T_ID;}

<INITIAL>"[" { BEGIN(INLINK); return T_BRACKET; }
<INLINK>"]>" { BEGIN(INITIAL); return T_BRACKET; }
<INLINK>[A-Za-z0-9\-] { return T_CHAR; };
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

int main(int argc, char* argv[]) {
    while(1) {
        enum token t = yylex();
        if (t == 0) {
            break;
        }
        if (t == T_SPEC) {
            printf("T_SPEC %s %u %d\n", yytext, yylineno, num_chars);
        } else if (t == T_KEY) {
            printf("T_KEY %s %u %d\n", yytext, yylineno, num_chars);
        } else if (t == T_ID) {
            printf("T_ID %s %u %d\n", yytext, yylineno, num_chars);
        } else if (t == T_COLON) {
            printf("T_COLON %s %u %d\n", yytext, yylineno, num_chars);
        } else if (t == T_TAB) {
            printf("T_TAB \"\\t\" %u %d\n", yylineno, num_chars);
        } else if (t == T_BRACKET) {
            printf("T_BRACKET %s %u %d\n", yytext, yylineno, num_chars);
        } else if (t == T_CHAR) {
            printf("T_CHAR %s %u %d\n", yytext, yylineno, num_chars);
        }

        num_chars += yyleng;
    }
    return EXIT_SUCCESS ;
}