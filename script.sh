bison -d mygram.ypp
flex mylex.l
g++ mygram.tab.cpp lex.yy.c -o main
./main tests/automat1.txt tests/test1.txt > output.txt
./main tests/automat2.txt tests/test2.txt >> output.txt
./main tests/automat3.txt tests/test3.txt >> output.txt
#wrong automats
./main tests/wrong_automat1.txt tests/test1.txt >> output.txt
./main tests/wrong_automat2.txt tests/test2.txt >> output.txt
./main tests/wrong_automat3.txt tests/test3.txt >> output.txt

