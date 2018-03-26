#!/bin/bash

# Number of threads for multithreading
nthreads=2
nevents=100

# Build simulation in contained directory
rm -rf build
mkdir build
cd build
cmake ..
make "-j""$nthreads"
#./IceCube -t "$nthreads" -n "$nevents"
