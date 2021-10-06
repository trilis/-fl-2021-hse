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
    yylval.word = buf;
    position += strlen(buf);
    return VERTEX_TYPE;
}
[0-9]+   {
    std::cout << yytext << "\n";
    yylval.val = std::atoi(yytext);
    position += strlen(yytext);
    return NUM;
}
[^\)\(\[\],\n->]   {
    std::cout << yytext << "\n";
    yylval.sym = yytext[0];
    position++;
    return SYM;
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
