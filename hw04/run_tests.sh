#!/usr/bin/env bash

./build.sh

for TEST in test-data/*.txt; do
    TEST="$(basename "$TEST" .txt)"
    ./run.sh "test-data/$TEST.txt" "test-data/string$TEST.txt"
done