%{
#include <iostream>
#include "gram.tab.hpp"
%}

%option yylineno
%option noyywrap

%%
["e"]	{return E;}
["q"]		{return Q;}
[0-9]+	{yylval.val = atoi(yytext); return num;}
["t""s""n"]	{yylval.val = yytext[0]; 
return type_stat;}

<<EOF>>  {return 0;}

[{]	{return ls;}
[}]	{return rs;}
[,] {return comma;}
[ ] {return space;}
[\t\n]+ {}

. {yylval.val = *yytext; return chr;}

%%
