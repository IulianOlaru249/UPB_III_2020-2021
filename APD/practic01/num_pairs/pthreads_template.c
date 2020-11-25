#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>

/* Mutex 
 * pthread_mutex_lock(&mutex);
 * pthread_mutex_unlock(&mutex);
 */
pthread_mutex_t mutex;
pthread_mutex_t mutex2;
/* Barrier 
 * pthread_barrier_wait(&barrier);
 */
pthread_barrier_t barrier;

/* Condition variable */
pthread_cond_t cond1 = PTHREAD_COND_INITIALIZER; 

/* Problem size */
int N = 5;
int M = 3;
int can_eat = 1;

/*!!! Result !!! */
long sum = 0;

/* Struct for arguments */
typedef struct _thread_args {
	long id;
	int threads;
	int N;
	/* Fill With ParAmeter If Needed */
	int can_eat;
	int is_eating;
} thread_args;
thread_args **thr_args;

double min(double a, double b) {if (a < b) return a; else return b;}

/* Thread Function */
void *thread_func(void *arg) {
	thread_args *args = (thread_args *) arg;
	/* Get arguments */
	long id = args->id;
	int can_eat = args->can_eat;

	/* Do stuff */
 	pthread_mutex_lock(&mutex);
	if (can_eat == 1) {
		for (int i = 1; i <= N; i++) {
 			pthread_mutex_lock(&mutex2);
			if(thr_args[i]->is_eating == 0);
			{	
				thr_args[i]->can_eat = 0;
			}
 			pthread_mutex_unlock(&mutex2);
		}


		printf ("Dog from thread %ld is waiting!\n", id);
		pthread_cond_wait(&cond1, &mutex);
	}
	else 
	{
		pthread_cond_signal(&cond1);
		
		for (int i = 1; i <= N; i++) {
 			pthread_mutex_lock(&mutex2);
			if(thr_args[i]->is_eating == 0)
				thr_args[i]->can_eat = 1;
 			pthread_mutex_unlock(&mutex2);
		}
		

	}

 	pthread_mutex_unlock(&mutex);
	printf ("Dog from thread %ld is done!\n", id);
	/* Done */
    	pthread_exit(NULL);
}

int main() {
    	void *status;
	int r;

	/* Get number of cores */
	int cores = sysconf(_SC_NPROCESSORS_CONF); 
	cores = N;

	/* Parameters for thread function. */
	thr_args = (thread_args **) malloc ((cores + 1) * sizeof (struct _thread_args));
	
	/* Init the mutex and barier */
    	pthread_mutex_init(&mutex, NULL);
    	pthread_mutex_init(&mutex2, NULL);
    	pthread_barrier_init(&barrier, NULL, cores);

	/* Start threads */
    	pthread_t threads[cores];
    	for (long i = 1; i <= cores; i++) {
		threads[i] = i;
		/* Init thread arg */
		thr_args[i] = (thread_args *) malloc (sizeof (struct _thread_args));
		/* !!! Complete Parameters !!! */
		thr_args[i]->id = i;
		thr_args[i]->threads = cores;
		if (i <= M) {
			thr_args[i]->can_eat = 1;
			thr_args[i]->is_eating = 1;
		}
		else {
			thr_args[i]->can_eat = 0;
			thr_args[i]->is_eating = 0;
		}

		r = pthread_create(&threads[i], NULL, thread_func, thr_args[i]);
        	if (r) {
            		printf("Erroare la creare thread-ului %ld \n", i);
            		exit(-1);
        	}
    	}

	/* Join Threads */
    	for (long id = 1; id <= cores; id++) {
        	r = pthread_join(threads[id], &status);
        	if (r) {
            		printf("Erroare la asteptarea thread-ului %ld \n", id);
            		exit(-1);
        	}
    	}

	/* Clear all */
    	for (long id = 1; id <= cores; id++) {
		free (thr_args[id]);
	}
	free (thr_args);
    	pthread_mutex_destroy(&mutex);
	pthread_barrier_destroy(&barrier);
}
