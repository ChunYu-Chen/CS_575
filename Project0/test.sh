#!/bin/bash
# number of threads:

for t in 1 4
do
    echo NUMT = $t
    g++ -DNUMT=$t Project0.cpp -o Project0 -lm -fopenmp
    ./Project0
done
