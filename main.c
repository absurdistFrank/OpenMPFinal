#include <argp.h>
#include <stdlib.h>
#include <stdio.h>

#include "Matrix.h"

const char *argp_program_version = "Z-GEMM 1.0";
const char *argp_program_bug_address = "<zakariae.belmekki@cranfield.ac.uk>";

static char doc[] = "Z-GEMM -- Performs General Matrix Multiplication using both CUDA and OpenMP.";

static char args_doc[] = "MATRIX_WIDTH/LENGTH";

static struct argp_option options[] = {
	{"threads-number", 't', "THREADS", 0, "Set the number of threads by block to THREADS. (DEFAULT=16)"},
	{"iterations", 'i', "ITER", 0, "Set the number of iterations to ITER. (DEFAULT=1)"},
	{"verbose", 'v', 0, 0, "Activate verbose mode."},
	{0}
};

struct arguments{
	char *args[1];
	int iterations;
       	int threads_number;
	int verbose;
};


static error_t parse_opt (int key, char *arg, struct argp_state *state){
	struct arguments *arguments = state->input;
	switch(key){
		case 't':
			arguments->threads_number = atoi(arg);
			break;
		case 'i':
			arguments->iterations = atoi(arg);
			break;
		case 'v':
			arguments->verbose = 1;
		case ARGP_KEY_ARG:
			if(state->arg_num > 1) argp_usage(state);
			arguments->args[state->arg_num] = arg;
			break;
		case ARGP_KEY_END:
			if(state->arg_num < 1) argp_usage(state);
			break;
		default:
			return ARGP_ERR_UNKNOWN;
	}
	return 0;
}

static struct argp argp = {options, parse_opt, args_doc, doc};

int main(int argc, char** argv){
	struct arguments arguments;
	arguments.iterations = 1;
	arguments.threads_number = 0;
	arguments.verbose = 0;
	argp_parse(&argp, argc, argv, 0, 0, &arguments);
	if(arguments.threads_number != 0){
		omp_set_dynamic(0); 
		omp_set_num_threads(arguments.threads_number);
	}
	int length = atoi(arguments.args[0]);
	size_t size = length * length * sizeof(double);
	for(int i = 0; i < arguments.iterations; i++){
		Matrix matrix1, matrix2, matrix3, matrix4, matrix5;
		matrix1 = (Matrix) malloc(size);
		matrix2 = (Matrix) malloc(size);
		matrix3 = (Matrix) malloc(size);
		matrix4 = (Matrix) malloc(size);
		matrix5 = (Matrix) malloc(size);

		generate_matrix(matrix1, length, i);
		generate_matrix(matrix2, length, i+5);
		if(arguments.verbose == 1) {
			print_mat(matrix1, length);
			print_mat(matrix2, length);
		}
		double t1, t2;
		t1 = omp_get_wtime();
		matrix_mult_serial(matrix1 ,matrix2, matrix3, length);
		t2 = omp_get_wtime();
		printf("---- Serial GEMM: %lf\n", t2-t1); 
		if(arguments.verbose == 1) print_mat(matrix3, length);

		double t3, t4;
		t3 = omp_get_wtime();
		matrix_mult_omp(matrix1, matrix2, matrix4, length);
		t4 = omp_get_wtime();
		printf("---- OpenMP GEMM: %lf\n", t4-t3);
		if(arguments.verbose == 1) print_mat(matrix4, length);
		if(compare_matrices(matrix3, matrix4, length) == -1){
			printf("Error - Matrices not similar.\n");
			exit(-1);
		}

		double t5, t6;
		t5 = omp_get_wtime();
		matrix_mult_omp2(matrix1, matrix2, matrix5, length);
		t6 = omp_get_wtime();
		printf("---- OpenMP better memory usage GEMM: %lf\n", t6-t5);
		if(arguments.verbose == 1) print_mat(matrix5, length);
		if(compare_matrices(matrix5, matrix4, length) == -1) {
			printf("Error - Matrices not similar.\n");
			exit(-1);
		}
		printf("+------------+------------------+\n"
		       "|   Type     |      Runtime     |\n"
		       "+------------+------------------+\n"
		       "|   Serial   |  %e s  |\n"
		       "+------------+------------------+\n"
		       "|   OpenMP   |  %e s  |\n"
                       "+------------+------------------+\n"
		       "| OpenMP V2  |  %e s  |\n"
		       "+------------+------------------+\n", t2-t1, t4-t3, t6-t5);
       
		
	}

	return 0;
}

