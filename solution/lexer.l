%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum token {
    T_ID = 1,
    T_LETTER = 2,
    T_NODE = 3
};

int char_number = 1;

%}

%option yylineno
%option noyywrap

ID      (Start|Stock|Edge|Alphabet|Accept)
NODE    q(0|[1-9][0-9]*)
LETTER  [A-Za-z_0-9]*

%%

\n {char_number = 1;}

[ \t\r] {char_number++;}

[,] {
printf("comma-delimiter: %s %u %u\n", yytext, yylineno, char_number);
char_number++;
}

[:] {
printf("colon: %s %u %u\n", yytext, yylineno, char_number);
char_number++;
}

[{] {
printf("left-brace: %s %u %u\n", yytext, yylineno, char_number);
char_number++;
}

[}] {
printf("right-brace: %s %u %u\n", yytext, yylineno, char_number);
char_number++;
}

[(] {
printf("left-parenthesis: %s %u %u\n", yytext, yylineno, char_number);
char_number++;
}

[)] {
printf("right-parenthesis: %s %u %u\n", yytext, yylineno, char_number);
char_number++;
}

[-] {
printf("dash: %s %u %u\n", yytext, yylineno, char_number);
char_number++;
}


{ID} {
return T_ID;
}

{NODE} {
    return T_NODE;
}

{LETTER} {
    return T_LETTER;
}


<<EOF>> {
    printf("(eof %u)\n", yylineno); 
    return 0;
}

. {
  printf("ERROR in line %u, symbol %s\n", yylineno, yytext);
  exit(1);
}

%%

int main(int argc, char* argv[]) {
    while (1) {
        enum token t = yylex();
        if (t == 0) {
          break;
        }
        if (t == T_LETTER) {
            printf("alphabet letter: %s %u %u\n", yytext, yylineno, char_number);
            char_number += strlen(yytext);
        } else if (t == T_ID) {
            printf("identifier: %s %u %u\n", yytext, yylineno, char_number);
            char_number += strlen(yytext);
        } else if (t == T_NODE) {
            printf("node: %s %u %u\n", yytext, yylineno, char_number);
            char_number += strlen(yytext); 
        }
    };
    return EXIT_SUCCESS ;
}
