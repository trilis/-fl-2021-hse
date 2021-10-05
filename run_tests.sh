#!/usr/bin/env bash

bison -d gram.ypp
flex lex.l
g++ gram.tab.cpp lex.yy.c -o parse

echo "Testing a.kek..."
./parse tests/a.kek tests/a1.in >tests/a.out
if [ -n "$(cmp tests/a.out tests/a1.correct)" ]; then
  echo "FAILED"
else
  echo "OK"
fi

./parse tests/a.kek tests/a2.in >tests/a.out
if [ -n "$(cmp tests/a.out tests/a2.correct)" ]; then
  echo "FAILED"
else
  echo "OK"
fi

./parse tests/a.kek tests/a3.in >tests/a.out
if [ -n "$(cmp tests/a.out tests/a3.correct)" ]; then
  echo "FAILED"
else
  echo "OK"
fi

echo "Testing b.kek..."
./parse tests/b.kek tests/b1.in >tests/b.out
if [ -n "$(cmp tests/b.out tests/b1.correct)" ]; then
  echo "FAILED"
else
  echo "OK"
fi

./parse tests/b.kek tests/b2.in >tests/b.out
if [ -n "$(cmp tests/b.out tests/b2.correct)" ]; then
  echo "FAILED"
else
  echo "OK"
fi

./parse tests/b.kek tests/b3.in >tests/b.out
if [ -n "$(cmp tests/b.out tests/b3.correct)" ]; then
  echo "FAILED"
else
  echo "OK"
fi

echo "Testing c.kek..."
./parse tests/c.kek tests/c1.in >tests/c.out
if [ -n "$(cmp tests/c.out tests/c1.correct)" ]; then
  echo "FAILED"
else
  echo "OK"
fi

./parse tests/c.kek tests/c2.in >tests/c.out
if [ -n "$(cmp tests/c.out tests/c2.correct)" ]; then
  echo "FAILED"
else
  echo "OK"
fi

./parse tests/c.kek tests/c3.in >tests/c.out
if [ -n "$(cmp tests/c.out tests/c3.correct)" ]; then
  echo "FAILED"
else
  echo "OK"
fi

./parse tests/c.kek tests/c4.in >tests/c.out
if [ -n "$(cmp tests/c.out tests/c4.correct)" ]; then
  echo "FAILED"
else
  echo "OK"
fi

echo "Testing d.kek (non deterministic)..."
./parse tests/d.kek tests/d.in 2> tests/d.out
if [ -n "$(cmp tests/d.out tests/d.correct)" ]; then
  echo "FAILED"
else
  echo "OK"
fi

echo "Testing e.kek (incomplete)..."
./parse tests/e.kek tests/e.in 2> tests/e.out
if [ -n "$(cmp tests/e.out tests/e.correct)" ]; then
  echo "FAILED"
else
  echo "OK"
fi

echo "Testing f.kek (no start state)..."
./parse tests/f.kek tests/f.in 2> tests/f.out
if [ -n "$(cmp tests/f.out tests/f.correct)" ]; then
  echo "FAILED"
else
  echo "OK"
fi

echo "Testing g.kek (no such symbol)..."
./parse tests/g.kek tests/g.in 2> tests/g.out
if [ -n "$(cmp tests/g.out tests/g.correct)" ]; then
  echo "FAILED"
else
  echo "OK"
fi