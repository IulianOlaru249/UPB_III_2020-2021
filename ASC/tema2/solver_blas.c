/*
 * Tema 2 ASC
 * 2021 Spring
 */
#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "cblas.h"

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
 * ref: https://developer.apple.com/documentation/accelerate/1513132-cblas_dtrmm?language=objc
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
	double *res = alloc_matrix(N);

	/**
	 * res = B;
	 * res = A * res;
	 */
	memcpy(res, B, N * N * sizeof(*res));

	cblas_dtrmm(
		CblasRowMajor,
		CblasLeft,
		CblasUpper,
		CblasNoTrans,
		CblasNonUnit,
		N, N,
		1.0, A, N,
		res, N
	);
	
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
	double *res = alloc_matrix(N);

	cblas_dgemm(
		CblasRowMajor,
		CblasNoTrans,
		CblasTrans,
		N, N,
		N, 1,
		A, N,
		At, N,
		0, res, N);

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
	double *res = alloc_matrix(N);

	cblas_dgemm(
		CblasRowMajor,
		CblasTrans,
		CblasNoTrans,
		N, N,
		N, 1,
		A, N,
		At, N,
		0, res, N);

	return res;
}

/**
 * Add two matrixes.
 * There are no BLAS subroutines for adding
 * two matrices.
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
	int i = 0;
	int j = 0;
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
