%{
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>

int letters_counter = 0;
%}

%option yylineno
%option noyywrap

ALPH         [a-zA-Z0-9]*
DASH         \-
ARROW        \-\>
RIGHTBRACE   \}
LEFTBRACE    \{
COMMA        \,
ELLIPSIS     \.\.\.
COLON        \:

%%

{ALPH}         {printf("alphabet \"%s\" %u %d\n", yytext, yylineno, letters_counter);
                letters_counter += yyleng; }
{DASH}         {printf("dash \"%s\" %u %d\n", yytext, yylineno, letters_counter);
                letters_counter += yyleng; };
{ARROW}        {printf("arrow \"%s\" %u %d\n", yytext, yylineno, letters_counter);
                letters_counter += yyleng; };
{RIGHTBRACE}   {printf("rbrace \"%s\" %u %d\n", yytext, yylineno, letters_counter);
                letters_counter += yyleng; };
{LEFTBRACE}    {printf("lbrace \"%s\" %u %d\n", yytext, yylineno, letters_counter);
                letters_counter += yyleng; };
{COMMA}        {printf("comma \"%s\" %u %d\n", yytext, yylineno, letters_counter);
                letters_counter += yyleng; };
{ELLIPSIS}     {printf("ellipsis \"%s\" %u %d\n", yytext, yylineno, letters_counter);
                letters_counter += yyleng; };
{COLON}        {printf("colon \"%s\" %u %d\n", yytext, yylineno, letters_counter);
                letters_counter += yyleng; };

[\n]           {letters_counter = 0;}
[ \t\r]        {letters_counter++;}

<<EOF>>     {printf("(eof %u)\n", yylineno); return 0;}

. {
  printf("ERROR in line %u, symbol %s %d\n", yylineno, yytext, letters_counter);
  exit(1);
}

%%


int main(int argc, char* argv[]) {
  yylex() ;
  return EXIT_SUCCESS ;
}