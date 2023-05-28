#!/bin/bash

# Loop from 0 to 100
# for ((i=1; i<=200; i++)); do
    for ((j=2; j<=100; j++)); do
        # Construct the command with the current argument
        command="./bin/place -aux ./benchmark/ibm09/ibm09-cu90.aux 2 $j"
        
        # Run the command
        echo "Running command: $command"
        $command
    done
# done