#!/bin/bash

if [ $# -eq 2 ]; then
    if [ $1 == "-e" ]; then
        2>$2
    fi
fi


echo -e "num\t0\t1\t2\t3\t4\t5\t6"

for num in {1..9}; do
    echo -n "$num"
    for power in {0..6}; do
        res=$((num ** power))
        echo -ne "\t$res"
    done
    echo
done