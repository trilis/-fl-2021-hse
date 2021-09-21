%{
#include <stdio.h>
#include <stdlib.h>

enum token {
  T_ERR = 0, T_ID = 1, T_INT = 2, T_OP = 3, T_NEW_EDGE = 4, T_VISUAL = 5, T_BEGIN = 6, T_END = 7
};
int n, position = 1, pos_before, strnum, t;
char c;
%}

%option yylineno
%option noyywrap

NEW_EDGE   ->
INT        -?[1-9][0-9]*
VISUAL       [(),]
BEGIN_EDGE [\[]
END_EDGE   [\]]

%%

 /* Print identifiers, integers and operators. */
{INT}       {n = atoi(yytext);
	     pos_before = position; 
	     position += strlen(yytext); 
	     strnum = yylineno;
	     return T_INT; }
{NEW_EDGE}  {pos_before = position; 
	     position += 2; 
	     strnum = yylineno;
	     return T_NEW_EDGE;}
{VISUAL}    {c = yytext[0]; 
	     pos_before = position; 
	     position++; 
	     strnum = yylineno;
	     return T_VISUAL;}
{BEGIN_EDGE} {pos_before = position; 
	      position++; 
	      strnum = yylineno;
	      return T_BEGIN; }
{END_EDGE}  {pos_before = position;
	     position++;
	     strnum = yylineno; 
	     return T_END; }

 /* Ignore comments and whitespace. */
#[^\n]*     {}
[ \t]       {position++;}
[\r\n]      {position = 1;}

<<EOF>>     {printf("(eof %u)\n", yylineno); return 0;}

. {
  printf("ERROR in line %u, symbol %s position %u\n", yylineno, yytext, position);
  exit(1);
}

%%

int main(int argc, char* argv[]) {
  while (1) {
    t = yylex(); 
    if (t == T_ERR) {
      break;
    }   
    if (t == T_INT) {
      printf("T_INT %u %u %u\n", n, strnum, pos_before);
    } else if (t == T_OP) {
      printf("T_OP %c %u %u\n", c, strnum, pos_before);
    } else if (t == T_NEW_EDGE) {
      printf("T_NEW_EDGE -> %u %u\n", strnum, pos_before);
    } else if (t == T_VISUAL) {
      printf("T_VISUAL %c %u %u\n", c, strnum, pos_before);
    } else if (t == T_BEGIN) {
      printf("T_BEGIN [ %u %u\n", strnum, pos_before);
    } else if (t == T_END) {
      printf("T_END ] %u %u\n", strnum, pos_before);
    }
  };
  return EXIT_SUCCESS ;
}
