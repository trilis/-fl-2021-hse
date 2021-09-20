%{
#include <stdio.h>
#include <stdlib.h>

enum token {
  T_CONST = 1, 
  T_KEYWORD = 2, 
  T_STATE = 3, 
  T_ALPHABET_ELEMENT = 4, 
  T_PUNC = 5, 
  T_L_BRACE = 6,  
  T_R_BRACE = 7
};

unsigned int sym_num, str_num, last = 1, balance = 0;
int last_sym = -1,  step = -1;
%}

%option yylineno
%option noyywrap

CONST  (true|false)
KEYWORD (start|finish|states)
STATE  [A-Za-z_][A-Za-z_0-9]*
ALPHABET_ELEMENT \"[^\"]\"
L_BRACE [\{]
R_BRACE [\}]
PUNC [\,=\"\"\;\:]
%%

 /* Print identifiers, integers and operators. */
 
{KEYWORD}            {sym_num = last;str_num = yylineno;last += strlen(yytext);return T_KEYWORD;}
{ALPHABET_ELEMENT}   {sym_num = last;str_num = yylineno;last += strlen(yytext);
                      return T_ALPHABET_ELEMENT;}
{CONST}              {sym_num = last;str_num = yylineno;last += strlen(yytext);return T_CONST;}
{STATE}              {sym_num = last;str_num = yylineno;last += strlen(yytext);return T_STATE;}
{PUNC}               {sym_num = last;str_num = yylineno;last += strlen(yytext);return T_PUNC;}
{L_BRACE}            {sym_num = last;str_num = yylineno;last += strlen(yytext);return T_L_BRACE;}
{R_BRACE}            {sym_num = last;str_num = yylineno;last += strlen(yytext);return T_R_BRACE;}
 /* Ignore comments and whitespace. */
 
[\n]   {if (balance != 0) {
           printf("ERROR in line %u, not enough closing braces\n", yylineno);
           exit(1);
        }  balance = 0; step = -1; last = 1; last_sym = -1;}
#[^\n]*     {}
[ \t\r^\n] {}


%%

int main(int argc, char *argv[]) {
  int pos[14] = {T_STATE, T_PUNC,    T_L_BRACE, T_KEYWORD, T_PUNC,
                 T_CONST, T_PUNC,    T_KEYWORD, T_PUNC,    T_CONST,
                 T_PUNC,  T_KEYWORD, T_PUNC,    T_L_BRACE};
  const char *token_to_str[7] = {
      "T_CONST", "T_KEYWORD", "T_STATE",  "T_ALPHABET_ELEMENT",
      "T_PUNC",  "T_L_BRACE", "T_R_BRACE"};

  while (1) {
    enum token t = yylex();
    step++;
    if (t == 0) {
      break;
    }

    if ((last_sym == T_CONST ||
         (last_sym == T_ALPHABET_ELEMENT && t != T_R_BRACE)) &&
        t != T_PUNC) {
      printf("ERROR in line %u, got \"%s\", expected T_PUNC\n", yylineno,
             yytext);
      exit(1);
    }

    last_sym = t;
    if (t == T_L_BRACE) {
      balance++;
    }

    if (step < 14 && t != pos[step]) {
      printf("ERROR in line %u, got \"%s\", expected %s\n", yylineno, yytext,
             token_to_str[pos[step] - 1]);
      exit(1);
    }

    if (step == 15 && t != T_PUNC && t != T_STATE) {
      printf("ERROR in line %u, got \"%s\", expected T_L_BRACE or T_STATE\n",
             yylineno, yytext);
      exit(1);
    }
    if (t == T_R_BRACE) {
      balance--;
      if (balance == 0) {
        step = -1;
        last = 0;
      }
    }
    printf("%s \"%s\" line: %u, pos: %u\n", token_to_str[t - 1], yytext,
           str_num, sym_num);
  }

  return EXIT_SUCCESS;
}
