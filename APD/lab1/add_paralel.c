#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*
    schelet pentru exercitiul 5
*/

int* arr;
int* arr_res;
int array_size;
int cores;

int min (int a, int b) {if (a > b) return b; else return a;}

void *add(void *arg) {
  	long id = *(long*) arg;
	int start = id * (double)array_size / cores;
	int end = min((id + 1) * (double)array_size / cores, array_size);
	for (long i = start; i < end; i++)
		arr_res[i]  = arr[i] + 100;

  	pthread_exit(NULL);
}
int main(int argc, char *argv[]) {
    if (argc < 2) {
        perror("Specificati dimensiunea array-ului\n");
        exit(-1);
    }

    array_size = atoi(argv[1]);

    arr = malloc(array_size * sizeof(int));
    arr_res = malloc(array_size * sizeof(int));

    for (int i = 0; i < array_size; i++) {
        arr[i] = i;
    }

/*
    for (int i = 0; i < array_size; i++) {
        printf("%d", arr[i]);
        if (i != array_size - 1) {
            printf(" ");
        } else {
            printf("\n");
        }
    }
*/

	long cores = sysconf (_SC_NPROCESSORS_CONF);
	pthread_t threads[cores];
  	int r;
  	long id;
  	void *status;
  	long arguments[cores];

    	// TODO: aceasta operatie va fi paralelizata
  	for (id = 0; id < cores; id++) {
  		arguments[id] = id;
		r = pthread_create(&threads[id], NULL, add, &arguments[id]);

		if (r) {
	  		printf("Eroare la crearea thread-ului %ld\n", id);
	  		exit(-1);
		}
	}

	for (id = 0; id < cores; id++) {
		r = pthread_join(threads[id], &status);

		if (r) {
	  		printf("Eroare la asteptarea thread-ului %ld\n", id);
	  		exit(-1);
		}
  	}


/*
    for (int i = 0; i < array_size; i++) {
        printf("%d", arr_res[i]);
        if (i != array_size - 1) {
            printf(" ");
        } else {
            printf("\n");
        }
    }
*/

  	pthread_exit(NULL);
}
