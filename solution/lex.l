%{
#include <iostream>
#include "gram.tab.hpp"


int n;
char c;
char s[128];
%}

%option yylineno
%option noyywrap


CHAR \'.\'
%%
 /* Print identifiers, integers and operators. */

(cond) {strcpy(yylval.str, yytext); return cond_var;}
(alph) {strcpy(yylval.str, yytext); return alph_var;}
(edge) {strcpy(yylval.str, yytext); return edge_var;}
((start)|(terminate)|(regular)) {strcpy(yylval.str, yytext); return simple_cond;}
[A-Za-z_0-9]* {strcpy(yylval.str, yytext); return var;}
"=" {yylval.sym = yytext[0]; return op;}

{CHAR} {strcpy(yylval.str, yytext); return ch;}

"{" {yylval.sym = yytext[0]; return LFP;}
"}" {yylval.sym = yytext[0]; return RFP;}
"," {yylval.sym = yytext[0]; return COMMA;}
"(" {yylval.sym = yytext[0]; return LP;}
")" {yylval.sym = yytext[0]; return RP;}

 /* Ignore comments and whitespace. */
[ \t\r\n]*  {}
\/\/[^\n]* {}

<<EOF>>     {return 0;}

. {
  printf("ERROR in line %u, symbol %s\n", yylineno, yytext);
  exit(1);
}

%%
    