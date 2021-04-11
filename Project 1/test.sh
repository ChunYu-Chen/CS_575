#!/bin/bash

#number of threads:
for t in 1 2 4 8
do 
    echo NUMT = $t
    #number of subdivisions:
    for s in 1 10 100 1000 10000 100000 150000 200000 250000 300000 350000 400000 450000 500000
    do
	echo NUMTRIALS = $s
	g++ -DNUMTRIALS=$s -DNUMT=$t Project1.cpp -o Project1 -lm -fopenmp
	./Project1
    done
done    
