%{
#include <stdio.h>
#include <stdlib.h>

enum token {
  T_STATE = 1, T_FINAL_STATES = 2, T_ALPHABET = 3
};
int n;
char c;
unsigned int yycolumnno = 0;

#define COL {yycolumnno += strlen(yytext);}
%}

%option yylineno
%option noyywrap

STATE         q_[A-Za-z0-9]+
ALPHABET      [A-Za-eg-pr-z0-9][A-Za-z0-9]*
FINAL_STATES  f

%%

 /* Print identifiers. */
{STATE}    {printf("T_STATE %s %u %u\n", yytext, yylineno, yycolumnno); COL; return T_STATE; }
{ALPHABET}    {printf("T_ALPHABET %s %u %u\n", yytext, yylineno, yycolumnno); COL; return T_ALPHABET; }
{FINAL_STATES}    {printf("T_FINAL_STATES %s %u %u\n", yytext, yylineno, yycolumnno); COL; return T_FINAL_STATES; }

 /* Print delimiters. */
[[]         {printf("left-bracket %s %u %u\n", yytext, yylineno, yycolumnno); COL; }
[]]         {printf("right-bracket %s %u %u\n", yytext, yylineno, yycolumnno); COL; }
[(]         {printf("left-parenthesis %s %u %u\n", yytext, yylineno, yycolumnno); COL; }
[)]         {printf("right-parenthesis %s %u %u\n", yytext, yylineno, yycolumnno); COL; }
[,]         {printf("comma %s %u %u\n", yytext, yylineno, yycolumnno); COL; }
[;]         {printf("semicolon %s %u %u\n", yytext, yylineno, yycolumnno); COL; }

 /* Ignore comments and whitespace. */
#[^\n]*     {yycolumnno += strlen(yytext); }
[ \t\r\n]   {yycolumnno = 0; }

<<EOF>>     {printf("(eof %u)\n", yylineno); return 0;}

. {
  printf("ERROR in line %u, column %u, symbol %s\n", yylineno, yycolumnno, yytext);
  exit(1);
}

%%

int main(int argc, char* argv[]) {
  while (1) {
    enum token t = yylex();
    if (t == 0) {
      break;
    }
  };
  return EXIT_SUCCESS ;
}