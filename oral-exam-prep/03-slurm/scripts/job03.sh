#!/bin/bash
#SBATCH --job-name=matmul
#SBATCH --nodes=1
#SBATCH --ntasks=8
#SBATCH --time=00:15:00
#SBATCH --output=matmul_%j.out

export OMP_NUM_THREADS=8

srun ./matmul 4000
