%{
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

enum token {
	WORD = 1,
	EXIT = 2	
};

int alphabet_size = 0;
int states_number = 0;
int edges_number = 0;

%}

%option yylineno
%option noyywrap
%option stack

%x ENUMERATION IN_CURLY_BRACES NEW_BLOCK 

VARIABLE [a-z][a-zA-Z_0-9]*
ENUMERATION_BLOCK_NAME Alphabet|States|Edges 

%%
<INITIAL>{ENUMERATION_BLOCK_NAME}  {BEGIN(ENUMERATION); printf("BEGIN_ENUMERATION %s\n", yytext);}
<ENUMERATION>\s*\{  {yy_push_state(IN_CURLY_BRACES); printf("OPEN_CURLY_BRACE\n");}
<IN_CURLY_BRACES>{VARIABLE} {printf("VARIABLE %s\n", yytext);}
<IN_CURLY_BRACES>\s*\}  {BEGIN(INITIAL); printf("CLOSE_CURLY_BRACE\n");}
<IN_CURLY_BRACES>.* {printf("ERROR\n");}

<INITIAL>n



<<EOF>>     {printf("(eof %u)\n", yylineno); return 0;}

%%

int main(int argc, char* argv[]) {
  yylex() ;
  return EXIT_SUCCESS ;
}