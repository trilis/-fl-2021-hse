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
    std::cout << yytext << "\n";
    char *  buf = new char[strlen(yytext) + 1];
    strcpy(buf, yytext);
    vertex= 1;
    yylval.word = buf;
    position += strlen(buf);
    return VERTEX_TYPE;
}
[^->\[\]\(\),\n ] {
    std::cout << yytext << "\n";
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
        printf("vertex %d square_br_opened %d arrow %d\n", vertex, square_br_opened, arrow);
        exit(1);
    }
}
\(      {
    std::cout << yytext << "\n";
    yylval.sym = '(';
    position++;
    return LP;
}
\)      {
    std::cout << yytext << "\n";
    yylval.sym = ')';
    position++;
    return RP;
}
"["      {
    std::cout << yytext << "\n";
    yylval.sym = '[';
    square_br_opened = 1;
    position++;
    return SQ_BR_OP;
}
\]      {
    std::cout << yytext << "\n";
    yylval.sym = ']';
    square_br_opened = 0;
    position++;
    return SQ_BR_CL;
}
,      {
    std::cout << yytext << "\n";
    yylval.sym = ',';
    position++;
    return COMMA;
}
->    {
    std::cout << yytext << "\n";
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
