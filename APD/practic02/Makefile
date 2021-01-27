build:
	mpicxx main.cpp helpers.cpp helpers.h -o main
run:
	mpirun -oversubscribe -np 5 ./main input.in
clean:
	rm main