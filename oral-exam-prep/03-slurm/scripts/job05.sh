#!/bin/bash
#SBATCH --job-name=halo
#SBATCH --nodes=2
#SBATCH --ntasks-per-node=64
#SBATCH --time=00:10:00
#SBATCH --output=halo_%j.out

srun ./halo_exchange
