#!/bin/bash

echo "Checking neopt..."
valgrind --tool=cachegrind --branch-sim=yes --verbose --log-file=neopt.cache ./tema2_neopt /export/asc/tema2/input_valgrind;
echo "Done."

echo "Checking opt_m..."
valgrind --tool=cachegrind --branch-sim=yes --verbose --log-file=opt_m.cache ./tema2_opt_m /export/asc/tema2/input_valgrind;
echo "Done."

echo "Checking blas..."
valgrind --tool=cachegrind --branch-sim=yes --verbose --log-file=blas.cache ./tema2_blas /export/asc/tema2/input_valgrind;
echo "Done."