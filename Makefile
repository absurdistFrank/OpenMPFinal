RM = rm
CFLAGS = -g -Wall -fopenmp

all: ZGEMM-omp

ZGEMM-omp: main.o Matrix.o
	        gcc $(CFLAGS) main.o Matrix.o -o ZGEMM
main.o: main.c
	        gcc $(CFLAGS) -c main.c

Matrix.o: Matrix.c Matrix.h
	        gcc $(CFLAGS) -c Matrix.c

clean:
	        $(RM) *.o 
