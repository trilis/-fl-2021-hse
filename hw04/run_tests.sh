#!/usr/bin/env bash

./build.sh

for TEST in test-data/*.in; do
    TEST="$(basename "$TEST" .in)"
    printf "test #$TEST: %s\n" "$(./run.sh test-data/$TEST.in test-data/string$TEST.txt)"
done