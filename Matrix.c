#include "Matrix.h"


void generate_matrix(Matrix matrix, int size, int seed){
	time_t t;
	srand((unsigned) time(&t));
	for(int i = 0; i < size * size; i++) matrix[i] =(double) rand()/RAND_MAX*100;
}

void print_mat(Matrix matrix, int size){
	printf("Printing Matrix:\n");
	for(int i = 0; i < size; i++){
		printf("[");
		for(int j = 0; j < size; j++){
			printf(" %e ", matrix[i * size + j]);
		}
		printf("]\n");
	}
}

void matrix_mult_serial(Matrix matrix1, Matrix matrix2, Matrix matrix3, int size){
	for(int i=0; i<size; i++){
		for(int j=0; j<size; j++){
			double partial_sum = 0;
			for(int k=0; k<size; k++){
				partial_sum += matrix1[i*size+k]*matrix2[k*size+j];
			}
			matrix3[i*size+j] = partial_sum;
		}
	}
}

void matrix_mult_omp(Matrix matrix1, Matrix matrix2, Matrix matrix3, int size){
	#pragma omp parallel
	{
		int i, j, k;
		#pragma omp for
		for(i=0; i<size; i++){
			for(j=0; j<size; j++){
				double partial_sum = 0;
				for(k=0; k<size; k++){
					partial_sum += matrix1[i*size+k]*matrix2[k*size+j];
				}
				matrix3[i*size+j] = partial_sum;
			}
		}
	
	}
}

void matrix_mult_omp2(Matrix matrix1, Matrix matrix2, Matrix matrix3, int size){
	int i, j, k;
	#pragma omp parallel shared(matrix1,matrix2) private (i, j, k)
	{
		#pragma omp for schedule(static)
		for(i=0; i<size;i++){
			for(j=0; j<size; j++){
				double partial_sum = 0;
				for(k=0; k<size;k++){
					partial_sum += matrix1[i*size+k] * matrix2[k*size+j];
				}
				matrix3[i*size+j] = partial_sum;
			}
		}
	}
}

int compare_matrices(Matrix matrix1, Matrix matrix2, int size){
	for(int i = 0; i < size*size; i++){
		if(matrix1[i] != matrix2[i]) return -1;	
	}
	return 0;
}
