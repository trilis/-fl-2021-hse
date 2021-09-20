%{
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <iostream>
#include <sstream>
using std::map, std::cout, std::endl;

int line = 0;
%}

%option noyywrap

LINE	^.+$

%%

{LINE}	{
line++;
if(line == 1) {
	for(int i = 0; i < strlen(yytext); i++) {
		if(yytext[i] == ' ') continue;
		cout << "T_SYMB " << *(yylex+i) << 
		" " << line << " " << i << '\n';
	}
} else {
if(yytext[0] == 'q') {
	int pos = 1;
	int num = 0;
	while(yytext[pos] != ' ') {
		num *= 10;
		num += yytext[pos] - '0';
		pos++;
	}
	char type = yytext[++pos];
	cout << "T_VERTEX " << num << " " << 
	(type == 's' ? "start" : (type == 't' ? "terminal" : "neutral")) << '\n';
} else {
	int pos = 3;
	
	int num = 0;
	while(yytext[pos] != ' ') {
		num *= 10;
		num += yytext[pos] - '0';
		pos++;
	}

	pos += 2;
	int num2 = 0;
	while(yytext[pos] != ' ') {
		num2 *= 10;
		num2 += yytext[pos] - '0';
		pos++;
	}

	cout << "T_EDGE q" << num << " q" << num2 << 
	" characters: ";
	pos += 1;
	std::stringstream ss;
	for(;pos < strlen(yytext); pos ++) {
		ss << yytext[pos] << " ";
	}
	cout << ss.str().c_str() << endl;
}
}
return 1;
}


<<EOF>>     {return 0;}

%%

int main(int argc, char* argv[]) {
	while(true) {
		int c = yylex();
		if(c == 0) {
			break;
		}
	}

	return EXIT_SUCCESS;
}
