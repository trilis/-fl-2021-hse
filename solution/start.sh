#!/bin/bash
bison -d gram.ypp
flex lexer.l
g++ gram.tab.cpp lex.yy.c
touch result.txt
echo 'Результаты тестов:' > result.txt
echo 'Тест 1: Слова длины 3, начинающиеся с "f"' >> result.txt
./a.out aut1.txt test1.1.txt >> result.txt
./a.out aut1.txt test1.2.txt >> result.txt
echo '' >> result.txt

echo 'Тест 2: Чётные числа' >> result.txt
./a.out aut2.txt test2.1.txt >> result.txt
./a.out aut2.txt test2.2.txt >> result.txt
echo '' >> result.txt

echo 'Тест 3: Двоичные числа, делящиеся на 3' >> result.txt
./a.out aut3.txt test3.1.txt >> result.txt
./a.out aut3.txt test3.2.txt >> result.txt
echo '' >> result.txt

echo 'Тест 4: Натуральные числа и 0 без лидирующих нулей, делящиеся на 5' >> result.txt
./a.out aut4.txt test4.1.txt >> result.txt
./a.out aut4.txt test4.2.txt >> result.txt
./a.out aut4.txt test4.3.txt >> result.txt
./a.out aut4.txt test4.4.txt >> result.txt

echo '' >> result.txt
echo 'Неккоректные автоматы:' >> result.txt
echo 'Тест 1: Начальное состояние корректно определено' >> result.txt
./a.out bad_aut1.txt test4.1.txt >> result.txt
echo '' >> result.txt

echo 'Тест 2: Повторяющиеся символы алфавита' >> result.txt
./a.out bad_aut2.txt test4.1.txt >> result.txt
\echo '' >> result.txt

echo 'Тест 3: Символ не из алфавита' >> result.txt
./a.out bad_aut3.txt test4.1.txt >> result.txt
echo '' >> result.txt

echo 'Тест 4: Автомат не детерминирован' >> result.txt
./a.out bad_aut4.txt test4.1.txt >> result.txt
echo '' >> result.txt

echo 'Тест 5: Автомат не полон' >> result.txt
./a.out bad_aut5.txt test4.1.txt >> result.txt

