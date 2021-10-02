#!/bin/bash
#SBATCH -J Project7
#SBATCH -A cs475-575
#SBATCH -p class
#SBATCH -N 8 #number of nodes
#SBATCH -n 8 #number of tasks
#SBATCH -o Project7.out
#SBATCH -e Project7.err
#SBATCH --mail-type=BEGIN,END,FAIL
#SBATCH --mail-user=chench6@oregonstate.edu
for s in 4 6 8 16 32
do
mpic++ Project7.cpp -o Project7 -lm
mpiexec -mca btl self,tcp -np $s Project7
done
