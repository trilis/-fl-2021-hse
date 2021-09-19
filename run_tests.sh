#!/usr/bin/env bash

flex kek_automata.l
gcc lex.yy.c

echo "Testing a.kek..."
./a.out <tests/a.kek >tests/a.out
if [ -n "$(cmp tests/a.out tests/a.correct)" ]; then
  echo "FAILED"
else
  echo "OK"
fi

echo "Testing b.kek..."
./a.out <tests/b.kek >tests/b.out
if [ -n "$(cmp tests/b.out tests/b.correct)" ]; then
  echo "FAILED"
else
  echo "OK"
fi

echo "Testing c.kek..."
./a.out <tests/c.kek >tests/c.out
if [ -n "$(cmp tests/c.out tests/c.correct)" ]; then
  echo "FAILED"
else
  echo "OK"
fi

echo "Testing wrong1.kek..."
./a.out <tests/wrong1.kek >tests/wrong1.out
if [ -n "$(cmp tests/wrong1.out tests/wrong1.correct)" ]; then
  echo "FAILED"
else
  echo "OK"
fi

echo "Testing wrong2.kek..."
./a.out <tests/wrong2.kek >tests/wrong2.out
if [ -n "$(cmp tests/wrong2.out tests/wrong2.correct)" ]; then
  echo "FAILED"
else
  echo "OK"
fi
