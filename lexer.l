%{
#include <stdio.h>
#include <stdlib.h>
int tokenno = 0;

enum token {
  T_NULL, T_ID = 1, T_OP = 2, T_LB = 3, T_RB = 4, T_AL = 5, T_NO = 6, T_ST = 7, T_EN = 8, T_SE = 9, T_NL = 10, T_CO = 11
};

%}

%option yylineno
%option noyywrap



ID      [A-Za-z_][A-Za-z_0-9]*
INT     -?[1-9][0-9]*
OP      [-+*/^=]
AL      ALPHABET
NO      (NODES)
ST      (START)
EN      END

%%

 /* Print delimiters. */
[\{]        {printf("(int %s %u)\n",yytext, yylineno); return T_LB;}
[\}]         {printf("(int %s %u)\n",yytext, yylineno); return T_RB;}
[\n]        {tokenno = 0; return T_NL;}
[:]         {printf("(semicolon %u)\n", yylineno); return T_CO;}


 /* Print identifiers, integers and operators. */
{ID}        {printf("(ID %s %u)\n",yytext, yylineno); return T_ID;}
{OP}        {printf("(OP %s %u)\n",yytext, yylineno); return T_OP;}

{AL}        {printf("(ALPHABET %s %u %d)\n",yytext, yylineno, tokenno); return T_AL;}
{EN}        {printf("(END %s %u %u)\n",yytext, yylineno, tokenno); return T_EN;}
{NO}        {printf("(NODES %s %u %u)\n",yytext, yylineno, tokenno); return T_NO;}
{ST}        {printf("(START %s %u %u)\n",yytext, yylineno, tokenno); return T_ST;}

 /* Ignore comments and whitespace. */
#[^\n]*     {}
[ \t\r\n]   {}

<<EOF>>     {printf("(eof %u)\n", yylineno); return 0;}

%%

int main(int argc, char* argv[]) {
  enum token last_token = T_NULL;
    while (1) {
    enum token t = yylex();
    if (t == 0) {
      break;
    }

    if (t == T_AL || last_token == T_NULL) { 
      if (last_token != -1 || t != T_AL) {
              printf("ERROR in line %u, got \"%s\", expected T_AL\n", yylineno, yytext);
              exit(1);
      }
   }

   if (t = T_CO && last_token != T_OP) {
    printf("ERROR in line %u, got \"%s\", expected T_OP\n", yylineno, yytext);
              exit(1);
   }

   if (last_token == T_NO || last_token == T_AL || last_token == T_EN || last_token == T_ST) {
      if (t != T_LB) {
        printf("ERROR in line %u, got \"%s\", expected T_LB\n", yylineno, yytext);
        exit(1);
   }
 }
   if (last_token == T_LB) {
      if (t != T_RB && t != T_OP) {
        printf("ERROR in line %u, got \"%s\", expected T_RB or T_OP\n", yylineno, yytext);
        exit(1);    
      }
  }

    last_token = t;
  }
  return EXIT_SUCCESS ;
}