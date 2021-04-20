#!/bin/bash

#number of threads:
for t in 1 2 4 6 8
do 
    echo NUMT = $t
    #number of subdivisions:
    for s in 10 100 1000 1500 2000 2500 3000
    do
	echo NUMNODES = $s
	g++ -DNUMNODES=$s -DNUMT=$t Project2.cpp -o Project2 -lm -fopenmp
	./Project2
    done
done    
