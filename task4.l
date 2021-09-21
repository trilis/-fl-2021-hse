%{
#include <stdio.h>
#include <stdlib.h>

enum token {
  T_ALPHABET = 1, T_STATES = 2, T_INITIAL_STATE = 3, T_FINAL_STATES = 4, T_TRANSITION = 5
};
int n;
char c;
unsigned int yycolumnno = 0;

#define COL {yycolumnno += strlen(yytext);}
%}

%option yylineno
%option noyywrap

ALPHABET      \(\ *[^q ][A-Za-z0-9]*(\ *,\ *[^q ][A-Za-z0-9]*)*\ *\)
STATES        \(\ *q_[A-Za-z0-9]+(\ *,\ *q_[A-Za-z0-9]+)*\ *\)
INITIAL_STATE q_[A-Za-z0-9]+
FINAL_STATES  f\(\ *q_[A-Za-z0-9]+(\ *,\ *q_[A-Za-z0-9]+)*\ *\)
TRANSITION    \(\ *q_[A-Za-z0-9]+\ *,\ *q_[A-Za-z0-9]+\ *,\ *\([^q ][A-Za-z0-9]*(\ *,\ *[^q ][A-Za-z0-9]*)*\ *\)\)

%%

 /* Print identifiers. */
{ALPHABET}  {printf("T_ALPHABET %s %u %u\n", yytext, yylineno, yycolumnno); COL; return T_ALPHABET; }
{STATES}    {printf("T_STATES %s %u %u\n", yytext, yylineno, yycolumnno); COL; return T_STATES; }
{INITIAL_STATE} {printf("T_INITIAL %s %u %u\n", yytext, yylineno, yycolumnno); COL; return T_INITIAL_STATE; }
{FINAL_STATES}    {printf("T_FINAL_STATES %s %u %u\n", yytext, yylineno, yycolumnno); COL; return T_FINAL_STATES; }
{TRANSITION}    {printf("T_TRANSITION %s %u %u\n", yytext, yylineno, yycolumnno); COL; return T_TRANSITION; }

 /* Print delimiters. */
[[]         {printf("left-bracket %s %u %u\n", yytext, yylineno, yycolumnno); COL; }
[]]         {printf("right-bracket %s %u %u\n", yytext, yylineno, yycolumnno); COL; }
[(]         {printf("left-parenthesis %s %u %u\n", yytext, yylineno, yycolumnno); COL; }
[)]         {printf("right-parenthesis %s %u %u\n", yytext, yylineno, yycolumnno); COL; }
[,]         {printf("comma %s %u %u\n", yytext, yylineno, yycolumnno); COL; }
[;]         {printf("semicolon %s %u %u\n", yytext, yylineno, yycolumnno); COL; }

 /* Ignore comments and whitespace. */
#[^\n]*     {yycolumnno = 0; }
[ \t\r\n]   {yycolumnno += strlen(yytext); }

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