#!/bin/bash
#SBATCH --job-name=pi
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=1
#SBATCH --time=00:10:00
#SBATCH --output=pi_%j.out

export OMP_NUM_THREADS=64

./pi_par_loop 1000000000
