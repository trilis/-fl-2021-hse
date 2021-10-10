#!/usr/bin/env bash

flex lexer.l
bison -d parser.ypp
g++ parser.tab.cpp lex.yy.c -o main

#good tests

echo 'TEST ------------------ string = ab' > tests/result.txt
./main tests/automat1.txt tests/test1.txt >> tests/result.txt
echo 'TEST ------------------ number mod 5 == 0' >> tests/result.txt
./main tests/automat2.txt tests/test2.txt >> tests/result.txt
echo 'TEST ------------------ string containts "bb"' >> tests/result.txt
./main tests/automat3.txt tests/test3.txt >> tests/result.txt


#bad tests
echo 'TEST ------------------ string = ab' >> tests/result.txt
./main tests/automat1.txt tests/bad_test1.txt >> tests/result.txt
echo 'TEST ------------------ number mod 5 == 0' >> tests/result.txt
./main tests/automat2.txt tests/bad_test2.txt >> tests/result.txt
echo 'TEST ------------------ string containts "bb"' >> tests/result.txt
./main tests/automat3.txt tests/bad_test3.txt >> tests/result.txt
echo 'TEST ------------------ edge from stock' >> tests/result.txt
./main tests/automat4.txt tests/bad_test3.txt >> tests/result.txt
echo 'TEST ------------------ alphabet is not unique' >> tests/result.txt
./main tests/automat5.txt tests/bad_test3.txt >> tests/result.txt
