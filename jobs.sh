#!/bin/bash

# List of node counts to test
RUN_COUNTS=(1 2 3 4 5 6 7 8 9 10)
NODE_COUNTS=(1 2 4 8 10 18 24 34 40)

# Iterate over node counts
for RUN in "${RUN_COUNTS[@]}"; do
  for NODES in "${NODE_COUNTS[@]}"; do
    LOG_FILE="output_${NODES}.txt"

    # Submit the job with the current node count
    sbatch --ntasks="$NODES" --output="$LOG_FILE" --open-mode=append test.sh
  done
done