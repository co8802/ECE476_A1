#!/bin/bash
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --time=00:09:00
#SBATCH --output=sbatch/SLURM-mandelbrot-C33.log
#SBATCH --mem=16G
#SBATCH --cpus-per-task=33
#SBATCH --job-name=mandelbrot-C33
#SBATCH --distribution=block:block
#SBATCH --constraint=skylake
./prog1_mandelbrot_threads/mandelbrot -t 33 -i 1 2> logs/mandelbrot-C33.log
