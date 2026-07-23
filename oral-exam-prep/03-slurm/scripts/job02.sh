#!/bin/bash
#SBATCH --job-name=ring
#SBATCH --nodes=2
#SBATCH --ntasks=16
#SBATCH --time=00:05:00
#SBATCH --output=ring_%j.out

mpirun -np 4 ./ring
