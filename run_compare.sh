#!/usr/bin/env bash

RED='\033[0;31m'
NC='\033[0m'
GREEN='\033[0;32m'

passed=0;
failed=0;

g++ -std=c++17 -o src main.cpp -I parser_fast parser_fast/parser.* -fsanitize=address -O2 -Wuninitialized

printf "Test #1, regular expression with many operators\n";
printf "========================\n";
if [ "$(diff -q <(./src $(cat tests/test1.txt)) answers/test1.a)" ]; then
	printf "Test #1 ${RED}failed${NC}!\n";
	failed=$((failed+1));
else
	printf "Test #1 ${GREEN}passed${NC}!\n";
	passed=$((passed+1));
fi
printf "\n";


printf "Test #2, star operator\n";
printf "========================\n";
if [ "$(diff -q <(./src $(cat tests/test2.txt)) answers/test2.a)" ]; then
	printf "Test #2 ${RED}failed${NC}!\n";
	failed=$((failed+1));
else
	printf "Test #2 ${GREEN}passed${NC}!\n";
	passed=$((passed+1));
fi
printf "\n";


printf "Test #3, multiple matching\n";
printf "========================\n";
if [ "$(diff -q <(./src $(cat tests/test3.txt)) answers/test3.a)" ]; then
	printf "Test #3 ${RED}failed${NC}!\n";
	failed=$((failed+1));
else
	printf "Test #3 ${GREEN}passed${NC}!\n";
	passed=$((passed+1));
fi
printf "\n";


printf "Test #4, empty string\n";
printf "========================\n";
if [ "$(diff -q <(./src $(cat tests/test4.txt)) answers/test4.a)" ]; then
	printf "Test #4 ${RED}failed${NC}!\n";
	failed=$((failed+1));
	echo "$(./src $(cat tests/test4.txt))"
else
	printf "Test #4 ${GREEN}passed${NC}!\n";
	passed=$((passed+1));
fi
printf "\n";


printf "Test #5, binary string\n";
printf "========================\n";
if [ "$(diff -q <(./src $(cat tests/test5.txt)) answers/test5.a)" ]; then
	printf "Test #5 ${RED}failed${NC}!\n";
	failed=$((failed+1));
else
	printf "Test #5 ${GREEN}passed${NC}!\n";
	passed=$((passed+1));
fi
printf "\n";


printf "Test #6, incorrect binary string\n";
printf "========================\n";
if [ "$(diff -q <(./src $(cat tests/test6.txt)) answers/test6.a)" ]; then
	printf "Test #6 ${RED}failed${NC}!\n";
	failed=$((failed+1));
else
	printf "Test #6 ${GREEN}passed${NC}!\n";
	passed=$((passed+1));
fi
printf "\n";

printf "${GREEN}Passed ${NC}: ${passed}, ${RED} Failed ${NC}: ${failed}\n";