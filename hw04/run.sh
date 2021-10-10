#!/usr/bin/env bash
if [[ "$#" -ne 2 ]]; then
  echo "Illegal number of parameters"
  exit 1
fi

./parser "$2" < "$1"