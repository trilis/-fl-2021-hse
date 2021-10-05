#!/usr/bin/bash
BLUE='\033[1;34m'
PURPLE='\033[0;35m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

printf "\n= = = = = =  R U N N I N G   T E S T S = = = = = = \n\n"
printf "\t1-ST COLUMNS - ${PURPLE}EXPECTED(expected.txt)${NC}, 2-ND COLUMN - ${CYAN}RESULT(result.txt)${NC}\n\n"
for f in *
do
    if [ "$f" != "tests.sh" ]
    then
        printf "\tCase ${BLUE}$f${NC}...\n"
        cat "$f/automat.txt" | ../executable.out "$f/word.txt" > "$f/result.txt"
        diff --color=always -y -d "$f/expected.txt" "$f/result.txt"
        echo ""
    fi
done