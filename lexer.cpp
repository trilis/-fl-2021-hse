%{
#include <stdio.h>
#include <stdlib.h>
int id = 1;
%}

%option yylineno
%option noyywrap

T_NUMBER [0-9]+

%%

[!]         {printf("Exclamation mark %u %d\n", yylineno, id); id++;}
[@]         {printf("At sign in line %u %d\n", yylineno, id); id++;}
[;]         {printf("Semicolon at %u %d\n", yylineno, id); id++;}
[,]         {printf("Comma at %u %d\n", yylineno, id); id++;}
{T_NUMBER} {printf("Number %s %u %d\n", yytext, yylineno, id); id += strlen(yytext);}

. {
printf("Error at id %d\n", id);
exit(1);
}

%%

int main(int argc, char* argv[]) {
    yylex() ;
    return EXIT_SUCCESS ;
}
