%{
#include <iostream>
#include "parser.tab.hpp"

%}

%option yylineno
%option noyywrap


STR_LITERAL     <<[a-zA-Z]+>>
NUM_LITERAL_D   [1-9][0-9]*|0
NUM_LITERAL_B   0b1[0-1]*|ob0
IF              if
ELSE            else
MAIN            main
WHILE           while
SKIP            skip
IDENTIFIER      [a-zA-Z][a-zA-Z0-9]*
COMMENT         ##.*?\n
COMMA           ,
SEMICOLON       ;    
EQUEL           =
OR              \|\|
AND             &&
NOT             !
IS_EQUEL        ==
IS_NOT_EQUEL    \/=
LT              <
GT              >
LT_EQ           <=
GT_EQ           >=
PLUS            \+
MINUS           -
TIMES           \*
DIV             \/
EXP             \^

%%

{STR_LITERAL}     { std::string string_ans(strndup(yytext, 100));
                    yylval.node = new Node(Messege_type::STR_LITERAL_, string_ans); 
                    return STR_LITERAL; }
{NUM_LITERAL_D}   { yylval.node = new Node(Messege_type::NUM_LITERAL_D_, atoi(yytext)); 
                    return NUM_LITERAL_D; }
{NUM_LITERAL_B}   { yylval.node = new Node(Messege_type::NUM_LITERAL_B_, atoi(yytext+2)); 
                    return NUM_LITERAL_B; }
{IF}              { return IF; }
{ELSE}            { return ELSE; }
{MAIN}            { yylval.node = new Node(Messege_type::MAIN_); 
                    return MAIN; }
{WHILE}           { return WHILE; }
{SKIP}            { yylval.node = new Node(Messege_type::SKIP_); 
                    return SKIP; }
{IDENTIFIER}      { std::string string_ans(strndup(yytext, 100));
                    yylval.node = new Node(Messege_type::IDENTIFIER_, string_ans); 
                    return IDENTIFIER; }
{COMMENT}         { std::string string_ans(strndup(yytext+2, 100));
                    yylval.node = new Node(Messege_type::COMMENT_, string_ans);
                    return COMMENT; }
[{]               { return LB; }
[}]               { return RB; }
[(]               { return LP; }
[)]               { return RP; }
{COMMA}           { return COMMA; }
{SEMICOLON}       { return SEMICOLON; }
{EQUEL}           { yylval.node = new Node(Messege_type::EQUEL_);
                    return EQUEL; }
{OR}              { yylval.node = new Node(Messege_type::OR_); 
                    return OR; }
{AND}             { yylval.node = new Node(Messege_type::AND_); 
                    return AND; }
{NOT}             { yylval.node = new Node(Messege_type::NOT_); 
                    return NOT; }
{IS_EQUEL}        { yylval.node = new Node(Messege_type::IS_EQUEL_); 
                    return IS_EQUEL; }
{IS_NOT_EQUEL}    { yylval.node = new Node(Messege_type::IS_NOT_EQUEL_); 
                    return IS_NOT_EQUEL; }
{LT}              { yylval.node = new Node(Messege_type::LT_); 
                    return LT; }
{GT}              { yylval.node = new Node(Messege_type::GT_); 
                    return GT; }
{LT_EQ}           { yylval.node = new Node(Messege_type::LT_EQ_); 
                    return LT_EQ; }
{GT_EQ}           { yylval.node = new Node(Messege_type::GT_EQ_); 
                    return GT_EQ; }
{PLUS}            { yylval.node = new Node(Messege_type::PLUS_); 
                    return PLUS; }
{MINUS}           { yylval.node = new Node(Messege_type::MINUS_); 
                    return MINUS; }
{TIMES}           { yylval.node = new Node(Messege_type::TIMES_); 
                    return TIMES; }
{DIV}             { yylval.node = new Node(Messege_type::DIV_); 
                    return DIV; }
{EXP}             { yylval.node = new Node(Messege_type::EXP_); 
                    return EXP; }
[ \t\r\n]         {}
<<EOF>>           {return 0;}
.                 { printf("ERROR in line %u, symbol %s\n", yylineno, yytext);
                    exit(1);
                  }

%%

