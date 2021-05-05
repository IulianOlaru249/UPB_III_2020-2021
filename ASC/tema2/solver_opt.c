/*
 * Tema 2 ASC
 * 2021 Spring
 */
#include "utils.h"

/* Get min of two integers */
int min(int i, int j)
{
	if (i < j)
		return i;

	return j;
}

/**
 * @param1: Number of rows and columns
 * @return: the new matrix, as a continuous
 *			memory area
 */
double *alloc_matrix(int N)
{
	return (double *)calloc(N * N, sizeof(double));
}

/**
 * @param1: the matrix, as a continuous
 *			memory area
 */
void free_matrix(double *A)
{
	free(A);
}

/**
 * Multiply two matrixes.
 * A must be superior triangular.
 *
 * @param1: the number of rows and columns
 * @param2: the first matrix, as a continuous
 *			memory area
 * @param3: the second matrix, as a continuous
 *			memory area
 * @return: the result of the multiplication
 */
double *multiply(int N, double *A, double *B)
{
	register int i = 0;
	register int j = 0;
	register int k = 0;
	register double *line_A = NULL;
	register double sum = 0.0;
	double *res = alloc_matrix(N);

	for (i = 0; i < N; i++)
		for (j = 0; j < N; j++) {
			/**
			 * Because A is upper triangular, ignore [0..i)
			 * because all values are 0.
			 */
			line_A = &A[i * N];
			sum = 0.0;
			for (k = i; k < N; k++)
				sum += *(line_A + k) * B[k * N + j];
			res[i * N + j] = sum;
		}

	return res;
}

/**
 * Multyply a matrix with the transpose of the other.
 *
 * @param1: the number of rows and clumns
 * @param2: the first matrix, as a continuous
 *			memory area
 * @param3: the second matrix, as a continuous
 *			memory area. Will be transposed
 * @return: the result of the multiplication
 */
double *multiply_with_transpose_right(int N, double *A, double *At)
{
	register int i = 0;
	register int j = 0;
	register int k = 0;
	register double sum = 0.0;
	register double *line_A = NULL;
	register double *line_At = NULL;
	double *res = alloc_matrix(N);

	for (i = 0; i < N; i++)
		for (j = 0; j < N; j++) {
			line_A = &A[i * N];
			line_At = &At[j * N];
			sum = 0.0;
			for (k = 0; k < N; k++)
				sum += *(line_A + k) * *(line_At + k);
			res[i * N + j] = sum;
		}

	return res;
}

/**
 * Multyply the transpose of matrix wit other matrix.
 * The matrix must be upper triagonal.
 *
 * @param1: the number of rows and clumns
 * @param2: the first matrix, as a continuous
 *			memory area
 * @param3: the second matrix, as a continuous
 *			memory area. Will be transposed
 * @return: the result of the multiplication
 */
double *multiply_with_transpose_left(int N, double *A, double *At)
{
	register int i = 0;
	register int j = 0;
	register int k = 0;
	register double sum = 0.0;
	double *res = alloc_matrix(N);

	for (i = 0; i < N; i++)
		/**
		 * This works because the result of the multiplication
		 * will be a symetric matrix. Therefore, the number of steps
		 * is halfed.
		 */
		for (j = i; j < N; j++) {
			/**
			 * Since A' is lower triagonal and A is upper triagonal,
			 * k does not need to go all the way to N. This prevents
			 * useless opperations such as 0 + 0.
			 */
			sum = 0.0;
			for (k = 0; k < min(i, j) + 1; k++)
				sum += At[k * N + i] * A[k * N + j];
			res[i * N + j] = sum;
			res[j * N + i] = sum;
		}

	return res;
}

/**
 * Add two matrixes.
 *
 * @param1: the number pf rows and columns
 * @param2: the first matrix, as a continuous
 *			memory area
 * @param3: the second matrix, as a continuous
 *			memory area
 * @return: the result of the addition
 */
double *add(int N, double *A, double *B)
{
	register int i = 0;
	register int j = 0;
	register double *line_A = NULL;
	register double *line_B = NULL;
	register double *line_res = NULL;

	double *res = alloc_matrix(N);

	for (i = 0; i < N; i++) {
		line_A = &A[i * N];
		line_B = &B[i * N];
		line_res = &res[i * N];
		for (j = 0; j < N; j++)
			*(line_res + j) = *(line_A + j) + *(line_B + j);
	}
	
	return res;
}

/* @return: A * B * B' + A' * A */
double* my_solver(int N, double *A, double *B) {
	double *AB = NULL;
	double *ABBt = NULL;
	double *AtA = NULL;
	double *C = NULL;

	/* (A * B) */
	AB = multiply(N, A, B);

	/* (A * B * B') */
	ABBt = multiply_with_transpose_right(N, AB, B);

	/* (A' * A) */
	AtA = multiply_with_transpose_left(N, A, A);

	/* (A * B * B' + A' * A) */
	C = add(N, ABBt, AtA);

	free_matrix(AB);
	free_matrix(ABBt);
	free_matrix(AtA);

	return C;
}
