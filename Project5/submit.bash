#!/bin/bash
#SBATCH -J CUDA_Monte_Carlo
#SBATCH -A cs475-575
#SBATCH -p class
#SBATCH --gres=gpu:1
#SBATCH -o montecarlo.out
#SBATCH -e montecarlo.err
#SBATCH --mail-type=BEGIN,END,FAIL
#SBATCH --mail-user=chench6@oregonstate.edu

for t in 16 32 64 128 256 512
do
    for s in 2048 4096 8192 16384 32768 65536 131072 262144 524288 1048576
    do
    /usr/local/apps/cuda/cuda-10.1/bin/nvcc -DBLOCKSIZE=$t -DNUMTRIALS=$s -o montecarlo montecarlo.cu
    ./montecarlo
    done
done
