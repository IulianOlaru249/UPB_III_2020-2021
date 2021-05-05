#!/bin/bash

module load compilers/gnu-5.4.0
make

./tema2_neopt ./my_in # /export/asc/tema2/input

./tema2_opt_m ./my_in # /export/asc/tema2/input

./tema2_blas ./my_in # /export/asc/tema2/input
