#!/bin/bash

PROJECT_DIR='/Users/atumulak/Developer/sn-solver'

cd $PROJECT_DIR
make
bin/sn-solver | tee -a $1
python scripts/process.py $1
