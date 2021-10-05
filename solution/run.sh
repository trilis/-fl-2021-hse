#!/usr/bin/bash

if [ -z "$1" ] || [ -z "$2" ]; then
    echo "Usage: ./run.sh {automat_file} {word_file}"
    exit 1
else
    :
fi

cat "$1" | ./executable.out "$2"