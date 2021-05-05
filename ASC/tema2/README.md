Nume: Olaru Gabriel Iulian

Grupă: 334CC

# Assignment 2 Code Optimisation

General Structure
-

The purpose of the assignment is to implement optimisation for computing a mathematical result.

**C = A × B × B' + A' × A**

* The first approach is to solve it without any optimisation. This will serve as a standard.
* The second approach adds memory access optimisations, without changing the complexity of the algorithm.
* The third approach uses the BLAS library.

The code has been broken up into functions, for better readability.
| Signature     | Description |
| ----------- | ----------- |
| multiply(int N, double *A, double *B)      | Multiply a upper tiagonal matrix with another matrix ( A × B ) |
| multiply_with_transpose_right(int N, double *A, double *At)   |    Multiply a matrix with a transpose matrix ( A × B')  |
| double *multiply_with_transpose_left(int N, double *A, double *At) | Multiply a transposed lower triagonal matrix with another matrix (A' × A) |

Details about the memory are specified in the **.memory** files.
Details about cache access are specified in the **.cache** files.

Implementation Details
-

Taking advantage that A is upper triagonal
-

Both the unoptimised and optimised approaches take advantage of the fact that A is upper triagonal, in order
to reduce the number of iterations:
  * When multiplying an upper triagonal with another matrix, we can ignore the elements below the main diagonal.
  Therefore the number of iterations will be reduced from **N^3** to **(N * N * (N + 1)) / 2**
  * The transpose of an upper triagonal is a lower triagonal. When multiplying a lower triagonal with an upper triagonal,
  we can reduce the number of iterations by ignoring the 0 + 0 opperations. This will be achived by only starting from
  the MIN(line, col) + 1.

Unoptimised Implementation
-

This Approach does every computation with O(N^3) complexity and N^3 iterations. It does not take advantage of any memory optimisations.
The clasic, **iterative** algorithm is used.
```
Run=./tema2_neopt: N=100: Time=0.029485
Run=./tema2_neopt: N=200: Time=0.163215
Run=./tema2_neopt: N=300: Time=0.432635
Run=./tema2_neopt: N=400: Time=1.041117
Run=./tema2_neopt: N=600: Time=3.516554
Run=./tema2_neopt: N=700: Time=5.611711
Run=./tema2_neopt: N=800: Time=8.437189
Run=./tema2_neopt: N=1000: Time=16.108234
Run=./tema2_neopt: N=1200: Time=28.163424
Run=./tema2_neopt: N=1600: Time=76.199486
```

Optimised Implementation
-

This approach takes advantage of memory access optimisation.

1. Identifing loop constants:
  The sum has been computed outside the third loop, the assigned to the result.
2. Keeping variables in registers:
  The sum and also the indexes of the loops are kept in registers in order to limit the memory access.
3. Limiting array access in loops:
  A pointer to eahc line of the matrix is computed, and the values needed for other opperations are accessed by it, instead of A[][].
  This limits the unecessary opperations when accessing memory.
4. Taking advantage that A' * A is symetrical, the number of iterations done is halfed.

```
Run=./tema2_opt_m: N=100: Time=0.004197
Run=./tema2_opt_m: N=200: Time=0.057072
Run=./tema2_opt_m: N=300: Time=0.147125
Run=./tema2_opt_m: N=400: Time=0.276927
Run=./tema2_opt_m: N=600: Time=0.973798
Run=./tema2_opt_m: N=700: Time=1.719783
Run=./tema2_opt_m: N=800: Time=2.214869
Run=./tema2_opt_m: N=1000: Time=4.257677
Run=./tema2_opt_m: N=1200: Time=7.444541
Run=./tema2_opt_m: N=1600: Time=21.773569
```
BLAS Implementation
-

This implementation takes advantade of the BLAS library.
Also, a special function that perform opperations on upper triangular matrices is used when computing A * B.

cblas_dtrmm() Takes care of multiplying an upper triagonal matrix with another.
cblas_dgemm() Takes care of multiplying matricses with inverses of other matricses.

BLAS offer no support for adding two matricses, so this is done "manually", with memory access optimisations.

```
Run=./tema2_blas: N=100: Time=0.001497
Run=./tema2_blas: N=200: Time=0.005292
Run=./tema2_blas: N=300: Time=0.016226
Run=./tema2_blas: N=400: Time=0.035308
Run=./tema2_blas: N=600: Time=0.112777
Run=./tema2_blas: N=700: Time=0.177244
Run=./tema2_blas: N=800: Time=0.291450
Run=./tema2_blas: N=1000: Time=0.496417
Run=./tema2_blas: N=1200: Time=0.846135
Run=./tema2_blas: N=1600: Time=1.982621
```

Performance Analysis
-

Ox: number of lines and columns
Oy: time in ms

As pictured in the below graph (and indicated by the complexity), the first two approaches are exponential.
Blas, on the other hand, is almost linear. ̿̿ ̿̿ ̿̿ ̿'̿'\̵͇̿̿\з= ( ▀ ͜͞ʖ▀) =ε/̵͇̿̿/’̿’̿ ̿ ̿̿ ̿̿ ̿̿

The second approach manages to slow the growth rate by better managing memory, as described above, so there is a noticable improvement.

![Performance Analysis](https://github.com/IulianOlaru249/UPB_III_2020-2021/blob/master/ASC/tema2/data_plots.png)


Cache Analysis
-

Looking at the tables below, there are ** 5 times ** less reft in the optimised version than in the unoptimised one.

This is due to the way memorry is access and loaded in the cache.


** Unoptimised **
```
==89560== I   refs:      5,766,410,284
==89560== I1  misses:            1,319
==89560== LLi misses:            1,297
==89560== I1  miss rate:          0.00%
==89560== LLi miss rate:          0.00%
```


** Optimised **
```
==90076== I   refs:      1,998,849,682
==90076== I1  misses:            1,307
==90076== LLi misses:            1,282
==90076== I1  miss rate:          0.00%
==90076== LLi miss rate:          0.00%
```


Resources
-

* BLAS: https://developer.apple.com/documentation/accelerate/1513132-cblas_dtrmm?language=objc
* Plotting: https://www.geeksforgeeks.org/graph-plotting-in-python-set-1/
* Optimisations: https://ocw.cs.pub.ro/courses/asc/laboratoare/05
* Testing: https://ocw.cs.pub.ro/courses/asc/laboratoare/07

