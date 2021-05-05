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
	int i = 0;
	int j = 0;
	int k = 0;
	double *res = alloc_matrix(N);

	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			/**
			 * Because A is upper triangular, ignore [0..i)
			 * because all values are 0.
			 */
			for (k = i; k < N; k++)
				res[i * N + j] += A[i * N + k] * B[k * N + j];
		}
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
	int i = 0;
	int j = 0;
	int k = 0;
	double *res = alloc_matrix(N);

	for (i = 0; i < N; i++)
		for (j = 0; j < N; j++)
			for (k = 0; k < N; k++)
				res[i * N + j] += A[i * N + k] * At[j * N + k];

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
	int i = 0;
	int j = 0;
	int k = 0;
	double *res = alloc_matrix(N);

	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++)
			/**
			 * Since A' is lower triagonal and A is upper triagonal,
			 * k does not need to go all the way to N. This prevents
			 * useless opperations such as 0 + 0.
			 */
			for (k = 0; k < min(i, j) + 1; k++)
				res[i * N + j] += At[k * N + i] * A[k * N + j];
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
	int  i = 0;
	int j = 0;
	double *res = alloc_matrix(N);

	for (i = 0; i < N; i++)
		for (j = 0; j < N; j++)
			res[i * N + j] = A[i * N + j] + B[i * N + j];
	
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
