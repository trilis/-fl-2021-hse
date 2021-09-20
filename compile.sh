#!/usr/bin/bash

GREEN='\033[0;32m'
BLUE='\033[0;34m'
RED='\033[0;31m'
NC='\033[0m' # No Color

filename="$1"
if [[ $# -eq 0 ]] ; then
    printf "${RED}One argumet required!${NC}\n"
    exit 1
fi
filename="$1"

printf "Applying ${GREEN}flex${NC}...\n"
flex -o "$filename.c" "$filename"
printf "Compiling with ${GREEN}gcc${NC}...\n"
gcc "$filename.c" -o "$filename.out"
printf "Final binary: ${GREEN} $filename.out ${NC}\n"

