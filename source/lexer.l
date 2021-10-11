%{
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string>
#include <stdexcept>

#include "gram.tab.hpp"

int line_pos = 1;


const int MAX_TOKEN_SIZE = 4 * (1 << 10) - 1;
char buffer[MAX_TOKEN_SIZE] = "\0";


const char *data_copy(char *text) {
    strcpy(buffer, text);
    char * tmp = new char[strlen(buffer) + 1];
    strcpy(tmp, buffer);
    return tmp;
}

%}

%option yylineno
%option noyywrap
%option stack

NEW new
INITIALIZATION_VARIABLE [_]Alphabet|[_]States|[_]Edges|[_]Q0
VARIABLE [a-z][a-z_0-9]*
STRUCT_NAME State|Edge
STRUCT_FIELD [_]{2}[a-z][a-z_0-9]*

STATE State
EDGES_STATE_FIELD __edges
COMMENT_STATE_FIELD __comment
IS_TERMINAL_STATE_FIELD __is_terminal

EDGE Edge
ALLOWED_LETTERS_EDGE_FIELD __allowed_letters 
END_EDGE_FIELD __end

BOOLEAN False|True
OPEN_CURLY_BRACKET "{"
STRING ['].+[']

CLOSE_CURLY_BRACKET "}"
OPEN_ROUND_BRACKET "("
CLOSE_ROUND_BRACKET ")"
COLON ":"
EMPTY_LINE "\n"
SPACES [\s]*
COMM ,
DOT "."

%%
{NEW} { return T_NEW; }
{INITIALIZATION_VARIABLE} { yylval.str = data_copy(yytext); return T_INITIALIZATION_VARIABLE; }
{VARIABLE} { yylval.str = data_copy(yytext); return T_VARIABLE; }
{STRING} { yylval.str = data_copy(yytext); return T_STRING; }
{BOOLEAN} { yylval.str = data_copy(yytext); return T_BOOLEAN; }
{DOT} { return T_DOT; }

{STATE} {return T_STATE;}
{EDGES_STATE_FIELD} { return T_EDGES_STATE_FIELD; }
{COMMENT_STATE_FIELD} { return T_COMMENT_STATE_FIELD; }
{IS_TERMINAL_STATE_FIELD} {return T_IS_TERMINAL_STATE_FIELD;}


{EDGE} {return T_EDGE;}
{ALLOWED_LETTERS_EDGE_FIELD}  {return T_ALLOWED_LETTERS_EDGE_FIELD;}
{END_EDGE_FIELD} {return T_END_EDGE_FIELD;}

{OPEN_CURLY_BRACKET} { return T_OPEN_CURLY_BRACKET; }
{CLOSE_CURLY_BRACKET} { return T_CLOSE_CURLY_BRACKET; }
{OPEN_ROUND_BRACKET} { return T_OPEN_ROUND_BRACKET; }
{CLOSE_ROUND_BRACKET} { return T_CLOSE_ROUND_BRACKET; }
{COLON}  { return T_COLON; }
{SPACES} { return T_SPACES;}
{COMM}   { return T_COMM; } 
{EMPTY_LINE}  {return T_EMPTY_LINE;}
[\r ]*  { }
<<EOF>> { return 0;}
. { throw std::runtime_error("invalid character: " + std::string(yytext, yyleng) + " line: " + std::to_string(yylineno)); return 1;}

%%
