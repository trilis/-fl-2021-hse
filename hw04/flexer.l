%{
#include "parser.tab.hpp"
unsigned char_pos = 0;
%}

%option yylineno
%option noyywrap

KEYWORD terminal
NUMBER  [+-]?(0|[1-9][0-9]*)
SYMBOL  [a-zA-Z_][a-zA-Z_0-9]*

%%
{KEYWORD} {
  char_pos += strlen(yytext);
  yylval.str = yytext;
  return T_KEYWORD;
}

{NUMBER} {
  char_pos += strlen(yytext);
  yylval.val = std::atoi(yytext);
  return T_NUMBER;
}

{SYMBOL} {
  char_pos += strlen(yytext);
  yylval.symb = yytext[0];
  return T_SYMBOL;
}

\n {
  char_pos = 0;
  yylval.symb = yytext[0];
  return T_END_LINE;
}

[ \t\r] {
  ++char_pos;
}

. {
  printf("ERROR in [%u; %u], symbol %s\n", yylineno, char_pos, yytext);
  exit(1);
}
%%
