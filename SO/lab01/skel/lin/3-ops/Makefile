CC = gcc
CFLAGS = -Wall
LDLIBS = -lm

all: ops

ops: ops.o add.o mul.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)
add.o: add.c
	gcc -c $^ 

mul.o: mul.c
	gcc -c $^ 

ops.o: ops.c
	gcc -D HAVE_MATH -c $^

clean:
	rm -rf *.o ops

