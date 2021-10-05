#!/bin/bash
bison gram.ypp
mv gram.tab.cpp gram.tab.hpp
flex lex.l
g++ lex.yy.c my.cpp
rm gram.tab.hpp
rm lex.yy.c
for ii in $(cat tests/list)
do
	echo $ii
	i="tests/"$ii
	./a.out $i".in1" $i".in2" > $i".out"
	diff $i".expect" $i".out" && echo "test complete"
	rm $i".out"
done
rm a.out
