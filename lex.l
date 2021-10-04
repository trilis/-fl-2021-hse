%{
#include <iostream>
#include "gram.tab.hpp"
%}

%option yylineno
%option noyywrap

%%
[0-9]+   { yylval.val = atoi(yytext); return NUM; }
[\+|\-]  { yylval.sym = yytext[0]; return OPA; }
[\*|/]   { yylval.sym = yytext[0]; return OPM; }
"("      { return LP; }
")"      { return RP; }
<<EOF>>  { return 0; }
[ \t\n]+ { }


%%