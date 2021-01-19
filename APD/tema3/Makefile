build:
	mpicxx main.cpp helpers.cpp helpers.h -o main
run:
	mpirun -oversubscribe -np 5 ./main ./tests/in/input2.txt
clean:
	rm main