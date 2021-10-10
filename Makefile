build: clean
	bison -d gram.ypp
	flex lex.l
	g++ -Wc++11-extensions gram.tab.cpp lex.yy.c

run: build

clean:
	rm -f *.cpp
	rm -f *.hpp
	rm -f *.c
	rm -f *.out