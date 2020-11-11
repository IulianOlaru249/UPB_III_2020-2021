/*
 * APD - Tema 1
 * Octombrie 2020
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

char *in_filename_julia;
char *in_filename_mandelbrot;
char *out_filename_julia;
char *out_filename_mandelbrot;

int min (int a, int b) {if (a > b) return b; else return a;}

// structura pentru un numar complex
typedef struct _complex {
	double a;
	double b;
} complex;

// structura pentru parametrii unei rulari
typedef struct _params {
	int is_julia, iterations;
	double x_min, x_max, y_min, y_max, resolution;
	complex c_julia;
} params;

typedef struct _thread_params {
	params *par;
	int id;
	int cores;
	int **result; 
	int width;
	int height;
} thread_params;

// citeste argumentele programului
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

// citeste fisierul de intrare
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

// scrie rezultatul in fisierul de iesire
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

// aloca memorie pentru rezultat
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

// elibereaza memoria alocata
void free_memory(int **result, int height)
{
	int i;

	for (i = 0; i < height; i++) {
		free(result[i]);
	}
	free(result);
}

pthread_barrier_t barrier;
// ruleaza algoritmul Julia
void *run_julia(void *arg)
{
	thread_params *thread_par = (thread_params*) arg;
	int w, h, i;

	printf("Hello from thread %d!\n", thread_par->id);

	int start = thread_par->id * (double)thread_par->height / thread_par->cores;
	int end = min((thread_par->id + 1) * (double)thread_par->height / thread_par->cores, thread_par->height - 1);


	for (w = 0; w < thread_par->width; w++) {
		for (h = start; h < end; h++) {
			int step = 0;
			complex z = { .a = w * thread_par->par->resolution + thread_par->par->x_min,
						  	.b = h * thread_par->par->resolution + thread_par->par->y_min };

			while (sqrt(pow(z.a, 2.0) + pow(z.b, 2.0)) < 2.0 && step < thread_par->par->iterations) {
				complex z_aux = { .a = z.a, .b = z.b };

				z.a = pow(z_aux.a, 2) - pow(z_aux.b, 2) + thread_par->par->c_julia.a;
				z.b = 2 * z_aux.a * z_aux.b + thread_par->par->c_julia.b;

				step++;
			}

			thread_par->result[h][w] = step % 256;
		}

		pthread_barrier_wait(&barrier);
	}

	// transforma rezultatul din coordonate matematice in coordonate ecran
	start = thread_par->id * ((double)thread_par->height / 2) / thread_par->cores;
	end = min((thread_par->id + 1) * ((double)thread_par->height / 2) / thread_par->cores, thread_par->height);

	for (i = start; i < end; i++) {
		int *aux = thread_par->result[i];
		thread_par->result[i] = thread_par->result[thread_par->height - i - 1];
		thread_par->result[thread_par->height - i - 1] = aux;
	}

	pthread_exit(NULL);
}

// ruleaza algoritmul Mandelbrot
void run_mandelbrot(void *arg)
{
	thread_params *thread_par = (thread_params*) arg;
	int w, h, i;

	for (w = 0; w < thread_par->width; w++) {
		for (h = 0; h < thread_par->height; h++) {
			complex c = { .a = w * thread_par->par->resolution + thread_par->par->x_min,
							.b = h * thread_par->par->resolution + thread_par->par->y_min };
			complex z = { .a = 0, .b = 0 };
			int step = 0;

			while (sqrt(pow(z.a, 2.0) + pow(z.b, 2.0)) < 2.0 && step < thread_par->par->iterations) {
				complex z_aux = { .a = z.a, .b = z.b };

				z.a = pow(z_aux.a, 2.0) - pow(z_aux.b, 2.0) + c.a;
				z.b = 2.0 * z_aux.a * z_aux.b + c.b;

				step++;
			}

			thread_par->result[h][w] = step % 256;
		}
	}

	// transforma rezultatul din coordonate matematice in coordonate ecran
	for (i = 0; i < thread_par->height / 2; i++) {
		int *aux = thread_par->result[i];
		thread_par->result[i] = thread_par->result[thread_par->height - i - 1];
		thread_par->result[thread_par->height - i - 1] = aux;
	}
}

int main(int argc, char *argv[])
{
	// Se aloca memorie pentru parametrii
	thread_params *thread_par;
	//thread_par = (thread_params**) malloc (2 * sizeof (struct _thread_params));

	//for (int i = 0; i < 2; i++) {
		thread_par = (thread_params*) malloc (sizeof (struct _thread_params));
		thread_par->par = (params*) malloc (sizeof (struct _params));
	//}

	// Se citesc argumentele programului
	get_args(argc, argv);

	// Se citeste numarul de core-uri
	thread_par->cores = atoi(argv[5]);
	pthread_t tid[thread_par->cores];

	pthread_barrier_init(&barrier, NULL, thread_par->cores);

	//Creeaza thread-urile
	//for (int i = 0; i < P; i++) {
	//	thread_id[i] = i;
	//	pthread_create(&thread_id[i], NULL, julia_thread_function, &thread_id[i]);
	//}

	// Julia:
	// - se citesc parametrii de intrare
	// - se aloca tabloul cu rezultatul
	// - se ruleaza algoritmul
	// - se scrie rezultatul in fisierul de iesire
	// - se elibereaza memoria alocata
	read_input_file(in_filename_julia, thread_par->par);
	thread_par->width = (thread_par->par->x_max - thread_par->par->x_min) / thread_par->par->resolution;
	thread_par->height = (thread_par->par->y_max - thread_par->par->y_min) / thread_par->par->resolution;
	thread_par->result = allocate_memory(thread_par->width, thread_par->height);

	/*
	read_input_file(in_filename_mandelbrot, thread_par[1]->par);
	thread_par[1]->width = (thread_par[1]->par->x_max - thread_par[1]->par->x_min) / thread_par[1]->par->resolution;
	thread_par[1]->height = (thread_par[1]->par->y_max - thread_par[1]->par->y_min) / thread_par[1]->par->resolution;
	thread_par[1]->result = allocate_memory(thread_par[1]->width, thread_par[1]->height);
	*/

	//for (int i = 0; i < thread_par->cores; i++) {
	//	thread_par->id = i;
	//	pthread_create(&tid[i], NULL, run_julia, thread_par);
	run_julia(thread_par);
	//}

	//for (int i = 0; i < thread_par->cores; i++) {
	//	pthread_join(tid[i], NULL);
	//}

	//run_mandelbrot(thread_par[1]);


	write_output_file(out_filename_julia, thread_par->result, thread_par->width, thread_par->height);
	//free_memory(thread_par->result, thread_par->height);

	//write_output_file(out_filename_mandelbrot, thread_par[1]->result, thread_par[1]->width, thread_par[1]->height);
	//free_memory(thread_par[1]->result, thread_par[1]->height);

	// Mandelbrot:
	// - se citesc parametrii de intrare
	// - se aloca tabloul cu rezultatul
	// - se ruleaza algoritmul
	// - se scrie rezultatul in fisierul de iesire
	// - se elibereaza memoria alocata
	
	/*
	read_input_file(in_filename_mandelbrot, thread_par[1]->par);

	thread_par[1]->width = (thread_par[1]->par->x_max - thread_par[1]->par->x_min) / thread_par[1]->par->resolution;
	thread_par[1]->height = (thread_par[1]->par->y_max - thread_par[1]->par->y_min) / thread_par[1]->par->resolution;

	thread_par[1]->result = allocate_memory(thread_par[1]->width, thread_par[1]->height);

	run_mandelbrot(thread_par[1]);

	write_output_file(out_filename_mandelbrot, thread_par[1]->result, thread_par[1]->width, thread_par[1]->height);
	free_memory(thread_par[1]->result, thread_par[1]->height);
	*/
	

	// Se dezaloca memoria pentru parametrii
	//for (int i = 0; i < 2; i++) {
	//	free (thread_par->par);
	//	free (thread_par);
	//}

	return 0;
}