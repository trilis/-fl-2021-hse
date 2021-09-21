%{
#include <stdio.h>
#include <stdlib.h>
int id = 1;
%}

%option yylineno
%option noyywrap

T_NUM_OF_VERTEX [0-9]+
T_VERTEX_TYPE [0-9]+,[01][01][01][01]
T_VERTEX_FUNC [0-9]+(,[0-9]+)*


%%

[!]         {printf("Exclamation mark %u %d\n", yylineno, id); id++;}
[@]         {printf("At sign in line %u %d\n", yylineno, id); id++;}
[;]         {printf("Semicolon at %u %d\n", yylineno, id); id++;}
{T_NUM_OF_VERTEX} {printf("Num of vertex %s %u %d\n", yytext, yylineno, id); id += strlen(yytext);}
{T_VERTEX_TYPE} {printf("Type of edge %s %u %d\n", yytext, yylineno, id); id += strlen(yytext);}
{T_VERTEX_FUNC} {printf("Edge function %s %u %d\n", yytext, yylineno, id); id += strlen(yytext);}


. {
printf("Error at id %d\n", id);
exit(1);
}

%%

int main(int argc, char* argv[]) {
    yylex() ;
    return EXIT_SUCCESS ;
}
