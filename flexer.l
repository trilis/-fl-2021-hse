%{
#include <stdio.h>
#include <stdlib.h>

unsigned char_pos = 0;
enum token {
    T_KEYWORD = 1, T_NUMBER, T_ALPHABET_SYMBOL
};
%}

%option yylineno
%option noyywrap

KEYWORD         terminal
NUMBER          [+-]?(0|[1-9][0-9]*)
ALPHABET_SYMBOL [a-zA-Z_][a-zA-Z_0-9]*

%%

 /* Print identifiers, integers and operators. */
{KEYWORD}          return T_KEYWORD;
{NUMBER}           return T_NUMBER;
{ALPHABET_SYMBOL}  return T_ALPHABET_SYMBOL;
\n                 char_pos = 0;
[ \t\r]            ++char_pos;

. {
  printf("ERROR in [%u; %u], symbol %s\n", yylineno, char_pos, yytext);
  exit(1);
}

%%

int main(int argc, char* argv[]) {
  while (1) {
    enum token t = yylex();
    if (t == 0) {
      break;
    }
    switch (t) {
      case T_KEYWORD: printf("T_KEYWORD"); break;
      case T_NUMBER: printf("T_NUMBER"); break;
      case T_ALPHABET_SYMBOL: printf("T_ALPHABET_SYMBOL"); break;
    }
    printf(" %s %u %u\n", yytext, yylineno, char_pos);
    char_pos += strlen(yytext);
  }
  return EXIT_SUCCESS;
}
