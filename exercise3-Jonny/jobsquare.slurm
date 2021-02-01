#!/bin/bash

# Slurm job options (name, compute nodes, job time)
#SBATCH --job-name=trapz-cores-25
#SBATCH --time=0:10:0
#SBATCH --exclusive
#SBATCH --nodes=1
#SBATCH --tasks-per-node=25
#SBATCH --cpus-per-task=1

# Replace [budget code] below with your budget code (e.g. t01)
#SBATCH --account=dc122
# We use the "standard" partition as we are running on CPU nodes
#SBATCH --partition=standard
# We use the "standard" QoS as our runtime is less than 4 days
#SBATCH --qos=standard

# Change to the submission directory
cd $SLURM_SUBMIT_DIR

# Enforce threading to 1 in case underlying libraries are threaded
export OMP_NUM_THREADS=1

# Load gcc module
module load mpt
module load intel-compilers-19

# Launch the parallel job
#   Using 1 thread
srun  ./trapzsqexe > traps-sq-cores-25.out
