%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gram.tab.hpp"

unsigned int char_number = 1;

%}

%option yylineno
%option noyywrap

ID      (Start|Stock|Edge|Alphabet|Accept)
NODE    q(0|[1-9][0-9]*)
LETTER  [A-Za-z_0-9]

%%

\n {char_number = 1;
//return LINE;
}

[ \t\r] {char_number++;}

[,] {
//printf("comma-delimiter: %s %u %u\n", yytext, yylineno, char_number);
char_number++;
return COM;
}

[:] {
//printf("colon: %s %u %u\n", yytext, yylineno, char_number);
char_number++;
return COL;
}

[{] {
//printf("left-brace: %s %u %u\n", yytext, yylineno, char_number);
char_number++;
return LB;
}

[}] {
//printf("right-brace: %s %u %u\n", yytext, yylineno, char_number);
char_number++;
return RB;
}

[(] {
//printf("left-parenthesis: %s %u %u\n", yytext, yylineno, char_number);
char_number++;
return LP;
}

[)] {
//printf("right-parenthesis: %s %u %u\n", yytext, yylineno, char_number);
char_number++;
return RP;
}
[.] {
//printf("dot: %s %u %u\n", yytext, yylineno, char_number);
char_number++;
return DOT;
}


{ID} {
    char *  buf = new char[strlen(yytext) + 1];
    strcpy(buf, yytext);
    yylval.word = buf;
    char_number += strlen(buf);
return ID;
}

{NODE} {
    char * str = yytext;
    str++;
    yylval.val = atoi(str);
    return NODE;
}

{LETTER} {
    yylval.sym = yytext[0];
    return SYM;
}


<<EOF>> {
    return 0;
}

. {
  printf("ERROR in line %u, in symbol %u, symbol %s\n", yylineno, char_number, yytext);
  exit(1);
}

%%
