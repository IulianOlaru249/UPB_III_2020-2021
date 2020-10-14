#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_THREADS 2

void *f(void *arg) {
  	long id = *(long*) arg;
	for (int i = 0; i < 100; i++)
  		printf("Hello World nr %d din thread-ul %ld!\n", i, id);
  	pthread_exit(NULL);
}

void *g(void *arg) {
  	long id = *(long*) arg;
	for (int i = 0; i < 100; i++)
  		printf("Hello OTHER World nr %d din thread-ul %ld!\n", i, id);
  	pthread_exit(NULL);
}


int main(int argc, char *argv[]) {
	pthread_t threads[NUM_THREADS];
  	int r;
  	long id;
  	void *status;
	long cores = sysconf (_SC_NPROCESSORS_CONF);
	printf("cores: %ld\n", cores);

  	//long arguments[cores];
  	long arguments[NUM_THREADS];

  	//for (id = 0; id < cores; id++) {
  		//arguments[id] = id;
  		arguments[0] = 0;
		//r = pthread_create(&threads[id], NULL, f, &arguments[id]);
		r = pthread_create(&threads[0], NULL, f, &arguments[0]);

		if (r) {
	  		//printf("Eroare la crearea thread-ului %ld\n", id);
	  		printf("Eroare la crearea thread-ului %d\n", 0);
	  		exit(-1);
		}

  		arguments[1] = 1;
		r = pthread_create(&threads[1], NULL, g, &arguments[1]);

		if (r) {
	  		//printf("Eroare la crearea thread-ului %ld\n", id);
	  		printf("Eroare la crearea thread-ului %d\n", 1);
	  		exit(-1);
		}
  	//}

  	for (id = 0; id < NUM_THREADS; id++) {
		r = pthread_join(threads[id], &status);

		if (r) {
	  		printf("Eroare la asteptarea thread-ului %ld\n", id);
	  		exit(-1);
		}
  	}

/*
		Task3: Rezultatele se afiseaza pe masura ce sunt 
		terminate, nu neaparat cand se termina toate operatiile 
		pe un thread. De aceea se poate afisa mesajul de pe 
		thread-ul 1 apoi de pe thread-ul 3 apoi, din nou, 
		de pe thread-ul 1
*/

  	pthread_exit(NULL);
}
