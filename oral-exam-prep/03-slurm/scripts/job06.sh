#!/bin/bash
#SBATCH --job-name=hybrid
#SBATCH --nodes=2
#SBATCH --ntasks-per-node=2
#SBATCH --cpus-per-task=32
#SBATCH --time=00:20:00
#SBATCH --output=hybrid_%j.out

export OMP_NUM_THREADS=$SLURM_CPUS_PER_TASK

srun ./heat_hybrid 8192 1000
