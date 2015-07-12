#!/bin/sh

mpirun ./vectorcompute.o 15
if [ $? -ne 0 ]; then
    echo "mpirun vector_compute.o error"
    exit 1
fi
