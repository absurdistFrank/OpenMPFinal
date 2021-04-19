#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <omp.h>

typedef double* Matrix;

void generate_matrix(Matrix matrix, int size, int seed);
void print_mat(Matrix matrix, int size);
void matrix_mult_serial(Matrix matrix1, Matrix matrix2, Matrix matrix3, int size);
void matrix_mult_omp(Matrix matrix1, Matrix matrix2, Matrix matrix3, int size);
void matrix_mult_omp2(Matrix matrix1, Matrix matrix2, Matrix matrix3, int size);
int compare_matrices(Matrix matrix1, Matrix matrix2, int size);

