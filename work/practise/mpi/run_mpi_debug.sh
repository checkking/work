#!/bin/sh

mpirun ./vectorcompute_debug.o 15
if [ $? -ne 0 ]; then
    echo "mpirun vectorcompute_debug.o error"
    exit 1
fi
