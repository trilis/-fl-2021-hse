%{
#include <stdio.h>
#include <stdlib.h>
int position = 1;
%}

%option yylineno
%option noyywrap

T_VERTEX_START s\[[0-9]+\]
T_EDGE    ->\[[0-9]+\]
T_VERTEX_TERM t\[[0-9]+\]
T_VERTEX_DEF d\[[0-9]+\]
T_VERTEX_TERM_AND_START st\[[0-9]+\]

%%

 /* Print delimiters. */
[(]         {printf("left-parenthesis in line %u, pos: %d\n", yylineno, position); position++;}
[)]         {printf("right-parenthesis in line: %u, pos: %d\n", yylineno, position); position++;}
[,]         {printf("comma in line: %u, pos: %d\n", yylineno, position); position++;}
[\n]        {position = 1;}
[\s]        {position++;}
[\t]        {position++;}
 /* Print vertexes and edges */
{T_VERTEX_START}       {printf("start vertex \"%s\" line: %u pos: %d\n", yytext, yylineno, position); position += strlen(yytext);}
{T_EDGE}        {printf("edge \"%s\" line: %u pos: %d\n", yytext, yylineno, position); position += strlen(yytext);}
{T_VERTEX_TERM}        {printf("terminal vertex \"%s\" line: %u pos: %d\n", yytext, yylineno, position); position += strlen(yytext);}
{T_VERTEX_DEF}      {printf("default vertex \"%s\" line: %u pos: %d\n", yytext, yylineno, position); position += strlen(yytext);}
{T_VERTEX_TERM_AND_START}   {printf("start and terminal vertex \"%s\" line: %u pos: %d\n", yytext, yylineno, position); position += strlen(yytext);}
#[^\n]*     {}

<<EOF>>     {printf("(eof in line: %u, position: %d)\n", yylineno, position); return 0;}

. {
  printf("ERROR in line %u, position %d, symbol %s\n", yylineno,position, yytext);
  exit(1);
}

%%

int main(int argc, char* argv[]) {
  yylex() ;
  return EXIT_SUCCESS ;
}
