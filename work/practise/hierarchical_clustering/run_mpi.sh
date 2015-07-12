#!/bin/sh

mpirun ./hierachical_clustering 100000
if [ $? -ne 0 ]; then
    echo "mpirun hierachical_clustering 100000 error"
    exit 1
fi
