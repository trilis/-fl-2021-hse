%{
#include <stdio.h>
#include <stdlib.h>
int position = 1;
int square_br_opened = 0;
int arrow = 0;
int vertex = 0;
%}

%option yylineno
%option noyywrap

T_VERTEX_START start
T_EDGE    ->
T_VERTEX_TERM term
T_VERTEX_DEF def
T_VERTEX_TERM_AND_START start_term
T_OPEN_PAREN \(
T_CLOSE_PAREN \)
T_OPEN_SQUARE_BR \[
T_CLOSE_SQUARE_BR \]
T_VERTEX_NUMBER [0-9]+
T_ARGS [^->\[\]\(\),\n ]+

%%

 /* Print delimiters. */
[,]         {printf("comma in line: %u, pos: %d\n", yylineno, position); position++;}
[\n]        {position = 1; arrow = 0; square_br_opened = 0; vertex = 0;}
[ ]        {position++;}
[\t]        {position++;}
 /* Print tokens */
{T_VERTEX_START} {
    printf("start vertex \"%s\" line: %u pos: %d\n", yytext, yylineno, position);
    position += strlen(yytext);
    vertex = 1;
}
{T_EDGE}        {
    arrow = 1;
    printf("edge \"%s\" line: %u pos: %d\n", yytext, yylineno, position);
    position += strlen(yytext);
}
{T_VERTEX_TERM}  {
    printf("terminal vertex \"%s\" line: %u pos: %d\n", yytext, yylineno, position);
    position += strlen(yytext);
    vertex = 1;
}
{T_VERTEX_DEF} {
    printf("default vertex \"%s\" line: %u pos: %d\n", yytext, yylineno, position);
    position += strlen(yytext);
    vertex = 1;
}
{T_VERTEX_TERM_AND_START}   {
    printf("start and terminal vertex \"%s\" line: %u pos: %d\n", yytext, yylineno, position);
    position += strlen(yytext);
    vertex = 1;
}
{T_OPEN_PAREN} {
    printf("open parenthesis line: %u pos: %d\n", yylineno, position);
    position += strlen(yytext);
}
{T_CLOSE_PAREN} {
    printf("close parenthesis line: %u pos: %d\n", yylineno, position);
    position += strlen(yytext);
}
{T_OPEN_SQUARE_BR} {
    square_br_opened = 1;
    printf("open square bracket line: %u pos: %d\n", yylineno, position);
    position += strlen(yytext);
}
{T_CLOSE_SQUARE_BR} {
     square_br_opened = 0;
     printf("close square bracket line : %u pos: %d\n", yylineno, position);
     position += strlen(yytext);
}
{T_ARGS} {
    if (arrow && !vertex && square_br_opened && yylineno != 1){
         printf("arrow arg \"%s\" line: %u pos: %d\n", yytext, yylineno, position); 
         position += strlen(yytext);
    }
    else if (!arrow && vertex && square_br_opened && yylineno != 1){
        printf("arg of begin of edge \"%s\" line: %u pos: %d\n", yytext, yylineno, position);
        position += strlen(yytext);
        vertex = 0;
    }
    else if (arrow && vertex && square_br_opened && yylineno != 1){
        printf("arg of end of edge \"%s\" line: %u pos: %d\n", yytext, yylineno, position);
        position += strlen(yytext);
        vertex = 0;
        arrow = 0;
    }
    else if (yylineno == 1){
        printf("alphabet symbol \"%s\" in line: %u pos: %d\n", yytext, yylineno, position);
        position += strlen(yytext);
    }
    else {
        printf("ERROR in line %u, position %d, symbol \"%s\"\n", yylineno, position, yytext);
        exit(1); 
    }
}


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
