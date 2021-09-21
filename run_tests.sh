#!/usr/bin/env bash

if [[ "$#" -eq 0 ]]; then
    printf "filename required\n"
    exit 1
fi

flex "$1"
gcc lex.yy.c

for TEST in test-data/*.in; do
    TEST="$(basename "$TEST" .in)"
    ./a.out < "test-data/$TEST.in" > "test-data/$TEST.out"
    if [ -n "$(cmp "test-data/$TEST.out" "test-data/$TEST.expected")" ]; then
        echo "test #$TEST: FAILED"
    else
        echo "test #$TEST: OK"
    fi
done
