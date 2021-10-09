%{
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

void error(){
  printf("ERROR");
  exit(1);
}


int line_pos = 1;

%}

%option yylineno
%option noyywrap
%option stack

KEYWORD new
INITIALIZATION_VARIABLE [_]Alphabet|[_]States|[_]Edges|[_]Q0
VARIABLE [a-z][a-z_0-9]*
STRUCT_NAME [A-Z][a-z_0-9]*
STRUCT_FIELD [_]{2}[a-z][a-z_0-9]*
BOOLEAN False|True
OPEN_CURLY_BRACKET "{"
STRING ['].+[']

CLOSE_CURLY_BRACKET "}"
OPEN_ROUND_BRACKET "("
CLOSE_ROUND_BRACKET ")"
COLON ":"
EMPTY_LINE "\n"
SPACES [\s]*
COMM ,
DOT "."

%%
{KEYWORD} { printf("KEYWORD %s %u  %d\n", yytext, yylineno, line_pos); line_pos+=yyleng; return T_KEYWORD; }
{INITIALIZATION_VARIABLE} { printf("INITIALIZATION_VARIABLE %s %u  %d\n", yytext, yylineno, line_pos); line_pos+=yyleng; }
{VARIABLE} { printf("VARIABLE %s %u  %d\n", yytext, yylineno, line_pos); line_pos+=yyleng; }
{STRING} { printf("STRING %s %u  %d\n", yytext, yylineno, line_pos); line_pos+=yyleng; }
{BOOLEAN} { printf("BOOLEAN %s %u  %d\n", yytext, yylineno, line_pos); line_pos+=yyleng; }
{STRUCT_NAME} { printf("STRUCT_NAME %s %u  %d\n", yytext, yylineno, line_pos); line_pos+=yyleng; }
{DOT} { printf("DOT %u  %d\n", yylineno, line_pos); line_pos+=yyleng; }
{STRUCT_FIELD} { printf("STRUCT_FIELD %s %u  %d\n", yytext, yylineno, line_pos); line_pos+=yyleng; }
{OPEN_CURLY_BRACKET} { printf("OPEN_CURLY_BRACKET %u  %d\n", yylineno, line_pos); line_pos+=yyleng; }
{CLOSE_CURLY_BRACKET} { printf("CLOSE_CURLY_BRACKET %u  %d\n", yylineno, line_pos); line_pos+=yyleng; }
{OPEN_ROUND_BRACKET} { printf("OPEN_ROUND_BRACKET %u  %d\n", yylineno, line_pos); line_pos+=yyleng; }
{CLOSE_ROUND_BRACKET} { printf("CLOSE_ROUND_BRACKET %u  %d\n", yylineno, line_pos); line_pos+=yyleng; }
{COLON} { printf("COLON %u  %d\n", yylineno, line_pos); line_pos+=yyleng; }
{COMM}  { printf("COMM %u  %d\n", yylineno, line_pos); line_pos+=yyleng; } 
{EMPTY_LINE}  { line_pos=1; }
[\r ]*  { line_pos+=1; }

. {printf("ERROR '%s' %u %d\n", yytext, yylineno, line_pos); exit(1);}
<<EOF>> {printf("EOF"); return 0;}

%%

int main(int argc, char* argv[]) {
  yylex() ;
  return EXIT_SUCCESS ;
}