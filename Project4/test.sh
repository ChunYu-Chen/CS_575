#!/bin/bash

# number of threads:
for t in 1024 2048 4096 8192 16384 32768 65536 131072 262144 524288 1048576 2097152 4194304 8388608
do
	ARRAY_SIZE=$t
	g++ -DARRAY_SIZE=$t Project4.cpp -o Project4 -lm -fopenmp
	./Project4
done
