#!/usr/bin/env bash

RED='\033[0;31m'
NC='\033[0m'
GREEN='\033[0;32m'

passed=0;
failed=0;

printf "Sample1 test, default file\n";
printf "========================\n";
if [ "$(diff -q <(./lex <sample-tests/sample1.mk) sample-answers/sample1.a)" ]; then
	printf "Sample1 test ${RED}failed${NC}!\n";
	failed=$((failed+1));
else
	printf "Sample1 test ${GREEN}passed${NC}!\n";
	passed=$((passed+1));
fi
printf "\n";


printf "Sample2 test, default file\n";
printf "========================\n";
if [ "$(diff -q <(./lex <sample-tests/sample2.mk) sample-answers/sample2.a)" ]; then
	printf "Sample2 test ${RED}failed${NC}!\n";
	failed=$((failed+1));
else
	printf "Sample2 test ${GREEN}passed${NC}!\n";
	passed=$((passed+1));
fi
printf "\n";


printf "Sample3 test, default file\n";
printf "========================\n";
if [ "$(diff -q <(./lex <sample-tests/sample3.mk) sample-answers/sample3.a)" ]; then
	printf "Sample3 test ${RED}failed${NC}!\n";
	failed=$((failed+1));
else
	printf "Sample3 test ${GREEN}passed${NC}!\n";
	passed=$((passed+1));
fi
printf "\n";

printf "Sample4 test, comments\n";
printf "========================\n";
if [ "$(diff -q <(./lex <sample-tests/sample4.mk) sample-answers/sample4.a)" ]; then
	printf "Sample4 test ${RED}failed${NC}!\n";
	failed=$((failed+1));
else
	printf "Sample4 test ${GREEN}passed${NC}!\n";
	passed=$((passed+1));
fi
printf "\n";


printf "Sample5 test, bad syntax\n";
printf "========================\n";
if [ "$(diff -q <(./lex <sample-tests/sample5.mk) sample-answers/sample5.a)" ]; then
	printf "Sample5 test ${RED}failed${NC}!\n";
	failed=$((failed+1));
else
	printf "Sample5 test ${GREEN}passed${NC}!\n";
	passed=$((passed+1));
fi
printf "\n";


printf "Sample6 test, incorrect regexp\n";
printf "========================\n";
if [ "$(diff -q <(./lex <sample-tests/sample6.mk) sample-answers/sample6.a)" ]; then
	printf "Sample6 test ${RED}failed${NC}!\n";
	failed=$((failed+1));
else
	printf "Sample6 test ${GREEN}passed${NC}!\n";
	passed=$((passed+1));
fi
printf "\n";

printf "${GREEN}Passed ${NC}: ${passed}, ${RED} Failed ${NC}: ${failed}\n";