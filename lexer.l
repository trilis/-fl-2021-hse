%{
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include "parser.tab.hpp"

int pos = 0;
%}

%option yylineno
%option noyywrap

%%

[0-9]*              {yylval.num = atoi(yytext);
                       pos+= strlen(yytext);
                       return VERTEX;};
\"[a-zA-Z0-9]\"    {char *  buf = new char[strlen(yytext) + 1];
                       strcpy(buf, yytext);
                       yylval.sym = buf[1];
                       pos += strlen(buf);
                       return ALPHA;};
\-                {yylval.sym = yytext[0];
                       pos += yyleng;
                       return DASH;};
\-\>              {char *  buf = new char[strlen(yytext) + 1];
                       strcpy(buf, yytext);
                       yylval.word = buf;
                       pos += strlen(buf);
                       return ARROW;};
\,                {yylval.sym = yytext[0];
                      pos += yyleng;
                      return COMMA;};
\:                {yylval.sym = yytext[0];
                       pos += yyleng;
                       return COLON;};
\{                {yylval.sym = yytext[0];
                       pos += yyleng;
                       return LBR;};
\}                {yylval.sym = yytext[0];
                       pos += yyleng;
                       return RBR;};
\[                {yylval.sym = yytext[0];
                       pos += yyleng;
                       return SLBR;};
\]                {yylval.sym = yytext[0];
                       pos += yyleng;
                       return SRBR;};

[\n]           {pos = 0;}
[ \t\r]        {pos++;}

<<EOF>>     {printf("(eof %u)\n", yylineno); return 0;}

. {
  printf("ERROR in line %u, symbol %s %d\n", yylineno, yytext, pos);
  exit(1);
}

%%
