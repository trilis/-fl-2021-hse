%{
#include <iostream>
#include "gram.tab.hpp"
int position = 1;
int square_br_opened = 0;
int arrow = 0;
int vertex = 0;
%}

%option yylineno
%option noyywrap

%%
(start|def|term|start_term) {
    char *  buf = new char[strlen(yytext) + 1];
    strcpy(buf, yytext);
    vertex= 1;
    yylval.word = buf;
    position += strlen(buf);
    return VERTEX_TYPE;
}
[^->\[\]\(\),\n ] {
    if (arrow && !vertex && square_br_opened || yylineno == 1){
         if (strlen(yytext) > 1) {
             printf("ERROR! Got string instead of char in line: %u pos: %d\n", yylineno, position);
             exit(1);
         }
        position += strlen(yytext);
        yylval.sym = yytext[0];
        return SYM;
    }
    else if (!arrow && vertex && square_br_opened && yylineno != 1){
        yylval.val = std::atoi(yytext); 
        position += strlen(yytext);
        vertex = 0;
        return NUM;
    }
    else if (arrow && vertex && square_br_opened && yylineno != 1){
        yylval.val = std::atoi(yytext);
        position += strlen(yytext);
        vertex = 0;
        arrow = 0;
        return NUM;
    }
    else {
        printf("ERROR in line %u, position %d, symbol \"%s\"\n", yylineno, position, yytext);
        exit(1);
    }
}
\(      {
    yylval.sym = '(';
    position++;
    return LP;
}
\)      {
    yylval.sym = ')';
    position++;
    return RP;
}
"["      {
    yylval.sym = '[';
    square_br_opened = 1;
    position++;
    return SQ_BR_OP;
}
\]      {
    yylval.sym = ']';
    square_br_opened = 0;
    position++;
    return SQ_BR_CL;
}
,      {
    yylval.sym = ',';
    position++;
    return COMMA;
}
->    {
    char * buf = new char[strlen(yytext) + 1];
    strcpy(buf, yytext);
    yylval.word = buf;
    position += 2;
    arrow = 1;
    return EDGE;
}
[\n] {position = 1; arrow = 0; square_br_opened = 0; vertex = 0;}
[ \t] {position++;}

. {
      printf("ERROR in line %u, position %d, symbol %s\n", yylineno,position, yytext);
      exit(1);
}
%%
