%{
#include <stdio.h>
#include <stdlib.h>

enum token {
  T_ID = 1, T_INT = 2, T_WORD = 3, T_STATE_DEF = 4, T_LP = 5, T_RP = 6
};

int num;
char *word;
int current_char = 0;
int to_add = 0;

void check_id(char *id) {
	if (strcmp(id, "ALPH_SIZE") != 0 &&
		strcmp(id, "ST_NUM") != 0 &&
		strcmp(id, "ED_NUM") != 0 &&
		strcmp(id, "STATE") != 0 &&
		strcmp(id, "EDGE") != 0 &&
		strcmp(id, "START_ST") != 0) {
		printf("Error: wrong ID %s, %d %d\n", id, yylineno, current_char);
		exit(1);
	}
}

%}

%option yylineno
%option noyywrap

ID     [A-Z_]+
INT    [0-9]+
WORD   \"[0-9a-zA-Z]+\"
STATE_DEF (term|notterm)

%%
{ID}        {to_add = strlen(yytext);word = yytext; return T_ID;}
{INT}       {to_add = strlen(yytext);num = atoi(yytext); return T_INT;}
{WORD}      {to_add = strlen(yytext);word = yytext; return T_WORD;}
{STATE_DEF} {to_add = strlen(yytext);word = yytext; return T_STATE_DEF;}

[\n]       {current_char = 0;}
[ \t\r,=]   {++current_char;}
<<EOF>>     {printf("(eof %u)\n", yylineno); return 0;}

. {
  printf("ERROR in line %u, char_num %d, symbol %s\n", yylineno, current_char, yytext);
  exit(1);
}
%%


int main(int argc, char* argv[]) {
  while (1) {
    enum token t = yylex();
    if (t == 0) {
      break;
    }
    if (t == T_INT) {
    	printf("T_INT %d %d %d\n", num, yylineno, current_char);
    }
    if (t == T_ID) {
    	check_id(word);
    	printf("T_ID %s %d %d\n", word, yylineno, current_char);
    }
    if (t == T_WORD) {
    	printf("T_WORD ");
    	for (; *word != '\0'; ++word) {
    		if (*word != '"') {
    			printf("%c", *word);
    		}
    	}
    	printf(" %d %d\n", yylineno, current_char);
    }
    if (t == T_STATE_DEF) {
    	printf("T_STATE_DEF %s %d %d\n", word, yylineno, current_char);
    }

    current_char += to_add;
    to_add = 0;
  };
  return EXIT_SUCCESS ;
}