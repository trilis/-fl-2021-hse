%{
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

enum token {
	WORD_ = 1,
	EXIT = 2	
};

int alphabet_size = 0;
int states_num = 0;
int edges_num = 0;

int is_num(char *start, char *end) {
	for (; start != end; ++start) {
		if (!isdigit(*start)) {
			return 0;
		}
	}
	return 1;
}

void print_word(char *start, char *end) {
	for (; start != end; ++start) {
		printf("%c", *start);
	}
}

int check_term(char *start, char *end) {
	if (end - start == 7 && strncmp(start, "notterm", 7) == 0) {
		return 1;
	}
	if (end - start == 4 && strncmp(start, "term", 4) == 0) {
		return 1;
	}
	return 0;
}

int get_num(char **s) {
	char *ptr = *s;
	while (isspace(*ptr)) {
		++ptr;
	}
	if (!isdigit(*ptr)) {
		return -1;
	}
	int res = 0;
	while (isdigit(*ptr)) {
		res *= 10;
		res += (*ptr) - '0';
		++ptr;
	}
	*s = ptr;
	return res;
}

void process_first_line(char *s) {
	char *start = s;
	int first = get_num(&s);
	if (first == -1) {
		printf("ERROR: expected 3 non negative numbers in first line\n");
		exit(0);
	} else {
		printf("alphabet_size %d 1 %d\n", first, s - start);
	}
	int second = get_num(&s);
	if (second == -1) {
		printf("ERROR: expected 3 non negative numbers in first line\n");
		exit(0);
	} else {
		printf("states_num %d 1 %d\n", second, s - start);
	}
	int third = get_num(&s);
	if (third == -1) {
		printf("ERROR: expected 3 non negative numbers in first line\n");
		exit(0);
	} else {
		printf("edges_num %d 1 %d\n", third, s - start);
	}
	if (*s != '\0') {
		printf("ERROR: expected 3 non negative numbers in first line, something extra is here\n");
		exit(0);
	}
}

char *get_word(char **s) {
	while (isspace(**s)) {
		++*s;
	}
	char *ptr = *s;
	while (*ptr != '\0' && !isspace(*ptr)) {
		++ptr;
	}
	return ptr;
}

void process_second_line(char *s) {
	char *start = s;
	while (*s != '\0') {
		char *ptr = get_word(&s);
		printf("alphabet_character ");
		char *word_start = s;
		print_word(s, ptr);
		printf(" %d %d\n", 2, word_start - start);
		s = ptr;
	}
	printf("\n");
}

void parse_line(char *s) {
	char *first_word_end = get_word(&s);
	char *first_word_start = s;
	s = first_word_end;
	char *second_word_end = get_word(&s);
	char *second_word_start = s;
	s = second_word_end;
	char *third_word_end = get_word(&s);
	char *third_word_start = s;
	s = third_word_end;
	if (*s != '\0') {
		printf("ERROR: expected at most 3 words in line %u\n", yylineno);
		exit(0);
	}

	if (first_word_end == second_word_end) {
		printf("ERROR: expected at least 2 words in line %u\n", yylineno);
		exit(0);
	}
	if (second_word_end == third_word_end) {
		if (is_num(first_word_start, first_word_end) && 
			check_term(second_word_start, second_word_end)) {
			print_word(second_word_start, second_word_end);
			printf(" vertex ");
			print_word(first_word_start, first_word_end);
			printf(" %u %d", yylineno, 0);
		} else {
			printf("ERROR: expected vertex definition in line %u\n", yylineno);
			exit(0);
		}
	} else {
		if (is_num(first_word_start, first_word_end) &&
			is_num(second_word_start, second_word_end)) {
			printf("edge ");
			print_word(first_word_start, first_word_end);
			printf(" -- ");
			print_word(second_word_start, second_word_end);
			printf(" with character ");
			print_word(third_word_start, third_word_end);
			printf(" %u %d", yylineno, 0);
		} else {
			printf("ERROR: expected edge definition in line %u\n", yylineno);
			exit(0);
		}
	}
	printf("\n");
}


%}

%option yylineno
%option noyywrap

LINE [ a-zA-Z0-9]* 

%%

 /* Print delimiters. */
[(]         {printf("(left-parenthesis %u)\n", yylineno);}
[)]         {printf("(right-parenthesis %u)\n", yylineno);}
[;]         {printf("(semicolon %u)\n", yylineno);}

 /* Print identifiers, integers and operators. */
{LINE} {
	if (yylineno == 1) {
		process_first_line(yytext);
	} else if (yylineno == 2) {
		//printf("%s\n", yytext);
		process_second_line(yytext);
	} else {
		parse_line(yytext);
	}
}

 /* Ignore comments and whitespace. */
#[^\n]*     {}
[ \t\r\n]   {}

<<EOF>>     {printf("(eof %u)\n", yylineno); return 0;}

%%

int main(int argc, char* argv[]) {
  yylex() ;
  return EXIT_SUCCESS ;
}