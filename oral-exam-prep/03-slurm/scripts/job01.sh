#!/bin/bash
#SBATCH --job-name=matmul
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=16
#SBATCH --time=00:15:00
#SBATCH --output=matmul_%j.out

export OMP_NUM_THREADS=$SLURM_CPUS_PER_TASK

./matmul 4000
