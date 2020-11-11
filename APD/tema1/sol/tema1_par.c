/*
 * APD - Tema 1
 * Octombrie 2020
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <math.h>

char *in_filename_julia;
char *in_filename_mandelbrot;
char *out_filename_julia;
char *out_filename_mandelbrot;

// Compute the min of two numbers
int min (int a, int b) {if (a > b) return b; else return a;}

// Barier
pthread_barrier_t barrier;

// Where Julia and Mandelbrot store their computations
int **result;

// Structure for complex number
typedef struct _complex {
	double a;
	double b;
} complex;

// Structure for params of a run
typedef struct _params {
	int is_julia, iterations;
	double x_min, x_max, y_min, y_max, resolution;
	complex c_julia;
} params;

// Structure for params needed by a thread
typedef struct _thread_params {
	// Generic thread info
	int thread_id;
	int threads;

	// Julia data
	params *julia_params;
	int julia_width;
	int julia_height;
	
	// Mandelbrot data
	int mandelbrot_width;
	int mandelbrot_height;
	params *mandelbrot_params;
} thread_params;

// Read program arguments
void get_args(int argc, char **argv)
{
	if (argc < 5) {
		printf("Numar insuficient de parametri:\n\t"
				"./tema1 fisier_intrare_julia fisier_iesire_julia "
				"fisier_intrare_mandelbrot fisier_iesire_mandelbrot\n");
		exit(1);
	}

	in_filename_julia = argv[1];
	out_filename_julia = argv[2];
	in_filename_mandelbrot = argv[3];
	out_filename_mandelbrot = argv[4];
}

// Read from input file
void read_input_file(char *in_filename, params* par)
{
	FILE *file = fopen(in_filename, "r");
	if (file == NULL) {
		printf("Eroare la deschiderea fisierului de intrare!\n");
		exit(1);
	}

	fscanf(file, "%d", &par->is_julia);
	fscanf(file, "%lf %lf %lf %lf",
			&par->x_min, &par->x_max, &par->y_min, &par->y_max);
	fscanf(file, "%lf", &par->resolution);
	fscanf(file, "%d", &par->iterations);

	if (par->is_julia) {
		fscanf(file, "%lf %lf", &par->c_julia.a, &par->c_julia.b);
	}

	fclose(file);
}

// Write to output file
void write_output_file(char *out_filename, int **result, int width, int height)
{
	int i, j;

	FILE *file = fopen(out_filename, "w");
	if (file == NULL) {
		printf("Eroare la deschiderea fisierului de iesire!\n");
		return;
	}

	fprintf(file, "P2\n%d %d\n255\n", width, height);
	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			fprintf(file, "%d ", result[i][j]);
		}
		fprintf(file, "\n");
	}

	fclose(file);
}

// Allocate memory for result
int **allocate_memory(int width, int height)
{
	int **result;
	int i;

	result = malloc(height * sizeof(int*));
	if (result == NULL) {
		printf("Eroare la malloc!\n");
		exit(1);
	}

	for (i = 0; i < height; i++) {
		result[i] = malloc(width * sizeof(int));
		if (result[i] == NULL) {
			printf("Eroare la malloc!\n");
			exit(1);
		}
	}

	return result;
}

// Free memory of result
void free_memory(int **result, int height)
{
	int i;

	for (i = 0; i < height; i++) {
		free(result[i]);
	}
	free(result);
}


// Run Julia algorithm
void run_julia(params* par, int thread_id, int threads, int width, int height)
{
	int w, h, i;

	// Break workload in chunks
	int start = thread_id * height / threads;
	int end = min((thread_id + 1) * height / threads, height);

	for (w = 0; w < width; w++) {
		for (h = start; h < end; h++) {
			int step = 0;
			complex z = { .a = w * par->resolution + par->x_min,
							.b = h * par->resolution + par->y_min };

			while (sqrt(z.a * z.a + z.b * z.b) < 2.0 && step < par->iterations) {
				complex z_aux = { .a = z.a, .b = z.b };

				z.a = z_aux.a * z_aux.a - z_aux.b * z_aux.b + par->c_julia.a;
				z.b = 2 * z_aux.a * z_aux.b + par->c_julia.b;

				step++;
			}

			result[h][w] = step % 256;
		}
	}

	// Wait for all threads to be finished before swapping the 
	// columns of the matrix in order to transform to screen 
	// cooprdinates
	pthread_barrier_wait(&barrier);

	// Break workload in chunks
	start = thread_id * (height / 2) / threads;
	end = min((thread_id + 1) * (height / 2) / threads, (height / 2));

	// Transform to screen coordinates
	for (i = start; i < end; i++) {
		int *aux = result[i];
		result[i] = result[height - i - 1];
		result[height - i - 1] = aux;
	}
	
}

// Run Mandelbrot algorithm
void run_mandelbrot(params* par, int thread_id, int threads, int width, int height)
{
	int w, h, i;

	// Break workload in chunks
	int start = thread_id * height / threads;
	int end = min((thread_id + 1) * height / threads, height);

	for (w = 0; w < width; w++) {
		for (h = start; h < end; h++) {
			complex c = { .a = w * par->resolution + par->x_min,
							.b = h * par->resolution + par->y_min };
			complex z = { .a = 0, .b = 0 };
			int step = 0;

			while (sqrt(z.a * z.a + z.b * z.b) < 2.0 && step < par->iterations) {
				complex z_aux = { .a = z.a, .b = z.b };

				z.a = z_aux.a * z_aux.a - z_aux.b * z_aux.b + c.a;
				z.b = 2.0 * z_aux.a * z_aux.b + c.b;

				step++;
			}

			result[h][w] = step % 256;
		}
	}

	// Wait for all threads to be finished before swapping the 
	// columns of the matrix in order to transform to screen 
	// cooprdinates
	pthread_barrier_wait(&barrier);

	// Break workload in chunks
	start = thread_id * (height / 2) / threads;
	end = min((thread_id + 1) * (height / 2) / threads, (height / 2));

	// Transform to screen coordinates
	for (i = start; i < end; i++) {
		int *aux = result[i];
		result[i] = result[height - i - 1];
		result[height - i - 1] = aux;
	}
}

void *thread_function (void *arg) {
	
	thread_params * thread_par = (thread_params*) arg;

	/* Julia */

	// Allocate result. Only one thread needs to do it
	if (thread_par->thread_id == 0) {
		result = allocate_memory (thread_par->julia_width, thread_par->julia_height);
	}
	// Wait for thread 0 to finish allocating memory
	pthread_barrier_wait(&barrier);

	// Run computations and wait for them to be done
	run_julia(thread_par->julia_params, thread_par->thread_id, thread_par->threads, thread_par->julia_width, thread_par->julia_height);
	pthread_barrier_wait(&barrier);

	//Write julia result and free memory
	if (thread_par->thread_id == 0) {
		write_output_file(out_filename_julia, result, thread_par->julia_width, thread_par->julia_height);
		free_memory (result, thread_par->julia_height);
	}

	/* Mandelbrot */
	
	// Allocate result. Only one thread needs to do it
	if (thread_par->thread_id == 0) {
		result = allocate_memory (thread_par->mandelbrot_width, thread_par->mandelbrot_height);
	}
	// Wait for thread 0 to finish allocating memory
	pthread_barrier_wait(&barrier);

	// Run computations
	run_mandelbrot(thread_par->mandelbrot_params, thread_par->thread_id, thread_par->threads, thread_par->mandelbrot_width, thread_par->mandelbrot_height);
	pthread_barrier_wait(&barrier);

	// Write mandelbrot result and free memory
	if (thread_par->thread_id == 0) {
		write_output_file(out_filename_mandelbrot, result, thread_par->mandelbrot_width, thread_par->mandelbrot_height);
		free_memory (result, thread_par->mandelbrot_height);
	}

	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
	// Read program arguments
	int i;
	int threads;
	get_args(argc, argv);
	threads = atoi(argv[5]);

	// Allocate memory and read params from input files
	params *julia_par, *mandelbrot_par;
	julia_par = (params*) malloc (sizeof (struct _params));
	mandelbrot_par = (params*) malloc (sizeof (struct _params));

	read_input_file(in_filename_julia, julia_par);
	read_input_file(in_filename_mandelbrot, mandelbrot_par);

	// Compute width and height so they can pe passed to threads
	int julia_width = (julia_par->x_max - julia_par->x_min) / julia_par->resolution;
	int julia_height = (julia_par->y_max - julia_par->y_min) / julia_par->resolution;

	int mandelbrot_width = (mandelbrot_par->y_max - mandelbrot_par->y_min) / mandelbrot_par->resolution;
	int mandelbrot_height =(mandelbrot_par->x_max - mandelbrot_par->x_min) / mandelbrot_par->resolution;

	// Barrier and threads initialisation
	pthread_t tid[threads];
	pthread_barrier_init(&barrier, NULL, threads);

	// Array of structures. One for each thread
	thread_params **thread_par = (thread_params **) malloc (threads * sizeof (struct _thread_params));
	for (i = 0; i < threads; i++) {
		// Allocate memory to each field of the structure for thread i
		thread_par[i] = (thread_params*) malloc ( sizeof (struct _thread_params));
		thread_par[i]->julia_params = (params*) malloc (sizeof (struct _params));
		thread_par[i]->mandelbrot_params = (params*) malloc (sizeof (struct _params));

		// Populate fields of structure with relevant data
		memcpy (thread_par[i]->julia_params, julia_par, sizeof (struct _params));
		memcpy (thread_par[i]->mandelbrot_params, mandelbrot_par, sizeof (struct _params));

		thread_par[i]->thread_id = i;
		thread_par[i]->threads = threads;

		thread_par[i]->julia_height = julia_height;
		thread_par[i]->julia_width = julia_width;

		thread_par[i]->mandelbrot_height = mandelbrot_height;
		thread_par[i]->mandelbrot_width = mandelbrot_width;

		// Create the thread
		pthread_create(&tid[i], NULL, thread_function, thread_par[i]);
	}
	
	// Joid all threads
	for (i = 0; i < threads; i++) {
		pthread_join(tid[i], NULL);
	}

	// Free memory for the aray of structures and the params for a run
	for (i = 0; i < threads; i++) {
		free (thread_par[i]->mandelbrot_params);
		free (thread_par[i]->julia_params);
		free (thread_par[i]);
	}
	free (thread_par);
	free (julia_par);
	free (mandelbrot_par);

	return 0;
}