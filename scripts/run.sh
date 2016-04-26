#!/bin/bash

PROJECT_DIR='/Users/atumulak/Developer/sn-solver'

cd $PROJECT_DIR

rm src/sn-solver.cpp
cp $1 src/sn-solver.cpp

make

bin/sn-solver | tee -a $2
python scripts/process.py $2
