#!/bin/bash
#SBATCH -J MatrixMul
#SBATCH -A cs475-575
#SBATCH -p class
#SBATCH --gres=gpu:1
#SBATCH -o first2.out
#SBATCH -e first2.err
#SBATCH --mail-type=BEGIN,END,FAIL
#SBATCH --mail-user=chench6@oregonstate.edu
for t in 1 8 64 256 1024 2048 4096 8192
do
        for s in 8 16 32 64 128 256 512
        do
        g++ -o first2 first2.cpp -DNMB=$t -DLOCAL_SIZE=$s /usr/local/apps/cuda/cuda-10.1/lib64/libOpenCL.so.1.1 -lm -fopenmp
        ./first2
        done
done

