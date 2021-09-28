%{
#include <stdio.h>
#include <stdlib.h>
int tokenno = 0;

enum token {
  T_NULL, T_ID = 1, T_OP = 2, T_LB = 3, T_RB = 4, T_AL = 5, T_NO = 6, 
  T_ST = 7, T_EN = 8, T_SE = 9, T_NL = 10, T_COL = 11, T_COM = 12
};

%}

%option yylineno
%option noyywrap


AL      ALPHABET
NO      NODES
ST      START
EN      END
ID      [A-Za-z_][A-Za-z_0-9]*
INT     -?[1-9][0-9]*
OP      [-+*/^=]
%%

 /* Print delimiters. */
[{]         {printf("(LB %u %u)\n", yylineno, ++tokenno);        return T_LB;}
[}]         {printf("(RB %u %u)\n", yylineno, ++tokenno);        return T_RB;}
[\n]        {tokenno = 0;                                        return T_NL;}
[:]         {printf("(colon %u %u)\n", yylineno, ++tokenno);     return T_COL;}
[,]         {printf("(comma %u %u)\n", yylineno, ++tokenno);     return T_COM;}
[;]         {printf("(semicolon %u %u)\n", yylineno, ++tokenno); return T_SE;}

 /* Print identifiers, integers and operators. */

{AL}        {printf("(ALPHABET %u %d)\n", yylineno, ++tokenno);         return T_AL;}
{EN}        {printf("(END %u %u)\n",      yylineno, ++tokenno);         return T_EN;}
{NO}        {printf("(NODES %u %u)\n",    yylineno, ++tokenno);         return T_NO;}
{ST}        {printf("(START %u %u)\n",    yylineno, ++tokenno);         return T_ST;}
{ID}        {printf("(ID %s %u %u)\n",    yytext, yylineno, ++tokenno); return T_ID;}
{OP}        {printf("(OP %s %u %u)\n",    yytext, yylineno, ++tokenno); return T_OP;}
  

 /* Ignore comments and whitespace. */
#[^\n]*     {}
[ \t\r\n]   {}

<<EOF>>     {printf("(eof %u)\n", yylineno); return 0;}

%%

int main(int argc, char* argv[]) {

  enum token last_token = T_NULL;
  int is_lb_opened = 0;
  int expecting_al = 1;
  int expecting_st = 1;
  int expecting_en = 1;
  int expecting_no = 1;

  while (1) {

    enum token t = yylex();
    
    if (t == 0) {
      if (expecting_al || expecting_no || expecting_en || expecting_st) {
        printf("ERROR didn't find all key words (ALPHABET, NODES, START, END)\n");
        exit(1);
      }
      break;
    }

    if (t == T_AL || last_token == T_NULL) {
      expecting_al = 0; 
      if (last_token != T_NULL || t != T_AL) {
        printf("ladt token %u\n", last_token);
              printf("ERROR in line %u, token %u, got \"%s\", expected T_AL\n", yylineno, tokenno,  yytext);
              exit(1);
      }
    }

    if (t ==T_NO)
      expecting_no = 0;

    if (t == T_ST)
      expecting_st = 0;

    if (t == T_EN) 
      expecting_en = 0;

    if (t == T_COL && last_token != T_ID) {
      printf("ERROR in line %u, token %u , got \"%s\", expected T_OP\n", yylineno,
        tokenno, yytext); 
      exit(1);
   } 

   if (last_token == T_NO || last_token == T_AL || last_token == T_EN 
                          || last_token == T_ST) {
      if (t != T_LB) {
        printf ("wtf %d  \n", t);
        printf("ERROR in line %u, token %u , got \"%s\", expected T_LB\n",
                yylineno, tokenno, yytext);
        exit(1);
     }
  }

  if (is_lb_opened) {
      if (t != T_RB && t != T_ID && t != T_COM) {
        printf("ERROR in line %u, token %u , got \"%s\", expected T_RB , T_ID or T_COM\n", yylineno,  tokenno, yytext);
        exit(1);    
      }
  }

  if (t == T_ID && (last_token != T_LB && last_token != T_COM && last_token != T_NL && last_token != T_RB && last_token != T_COL)) {
            printf("ERROR in line %u, token %u , got \"%s\", that wasn't expected\n", yylineno,  tokenno, yytext);
        exit(1);  
  }

  if (t == T_LB)
    is_lb_opened++;
  if (t == T_RB) 
    is_lb_opened--;

    last_token = t;
  }
  return EXIT_SUCCESS ;
}