#!/bin/bash

echo "Checking neopt..."
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --log-file=neopt.memory --verbose ./tema2_neopt /export/asc/tema2/input_valgrind;
echo "Done."

echo "Checking opt_m..."
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=opt_m.memory ./tema2_opt_m /export/asc/tema2/input_valgrind;
echo "Done."

echo "Checking blas..."
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=blas.memory ./tema2_blas /export/asc/tema2/input_valgrind;
echo "Done."
