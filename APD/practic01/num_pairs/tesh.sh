#!/bin/bash

echo "First arg: $1"

make build
for i in $( seq 1 $1 )
	do make run; 
done
make clean
