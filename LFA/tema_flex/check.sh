#!/bin/bash

make
echo

for filename in ./tests/*.txt; do

	for i in {1..25}
	do
   		echo -n "-"
	done

	echo " $(basename "$filename") "


	./hw < $filename > out/$(basename "$filename" .txt).out

	result=$(diff ref/$(basename "$filename" .txt).ref out/$(basename "$filename" .txt).out)

	if [ $? -eq 0 ]
	then
        	echo "Files are the same"
	else
		diff ref/$(basename "$filename" .txt).ref out/$(basename "$filename" .txt).out
	fi

	echo
done

make clean
