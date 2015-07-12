#!/bin/sh

mpirun ./vectorcompute_noblock.o 15
if [ $? -ne 0 ]; then
    echo "mpirun vector_compute_noblock.o error"
    exit 1
fi
