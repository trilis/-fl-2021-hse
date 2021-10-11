#!/bin/bash
for (( i=1; i <= 7; i++ ))
do
    ./automata "./test$i/automata.txt" "./test$i/tests.txt" > "./test$i/output.txt"
done
