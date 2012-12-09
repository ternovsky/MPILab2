#include <stdio.h> 
#include "mpi.h"
#define DEFAULT_TAG 0

void print_matrix(int **matrix, int row_count, int column_count) {
	int i, j;
	for (i = 0; i < row_count; i++) {
		for (j = 0; j < column_count; j++) {
			printf("%d  ", matrix[i][j]);	
		}
    	printf("\n");
	}	
}

void transpose(int n, int rank, int size, int loggable_rank) {
	int row_count, modulo, i, j, value, count;
    int k = 0, sum = 0;
    int *rbuf, *displs, *sbuf, *rcounts;
    int **matrix;
    
    row_count = n / size;
	modulo = n % size;
	if (modulo > 0) {
		if (rank <= modulo - 1) {
			row_count++;
			value = rank*row_count;
		} else if (rank == modulo) {
			value = rank*(row_count + 1);
		}
	} else {
		value = rank*row_count;
	}
	
	matrix = (int**) malloc(sizeof(int*)*row_count);
	for (i = 0; i < row_count; i++) {
		matrix[i] = (int*) malloc(sizeof(int*)*n);
		for (j = 0; j < n; j++) {
			matrix[i][j] = value;	
		}
		value++;
	}
	
	if (rank == loggable_rank) {
		printf("\n\nInitial matrix:\n");
		print_matrix(matrix, row_count, n);
	}
	
	sbuf = (int*) malloc(sizeof(int*)*row_count*n);
	for (i = 0; i < row_count; i++)
		for (j = 0; j < n; j++) {
			sbuf[k] = matrix[i][j];
			k++;	
		}
	//if (rank == 0) for (j = 0; j < row_count*n; j++) printf("%d ", sbuf[j]);	
	
	rbuf = (int*) malloc(sizeof(int)*n*n);
	displs = (int*) malloc(sizeof(int)*size);
	rcounts = (int*) malloc(sizeof(int)*size);
	for(i = 0; i < size; i++) {
		displs[i] = sum;
		count = n / size;
		if (modulo > 0 && i <= modulo - 1) count++;
		rcounts[i] = n*count;
		sum += rcounts[i];
	}
	//if (rank == 0) for(i = 0; i < size; i++) printf("%d -> %d\n", displs[i], rcounts[i]);
	
	MPI_Allgatherv(sbuf, row_count*n, MPI_INT, rbuf, rcounts, displs, MPI_INT, MPI_COMM_WORLD);
	//if (rank == 2) for(i = 0; i < n*n; i++) printf("%d ", rbuf[i]);
	
	matrix = (int**) malloc(sizeof(int*)*n);
	for (i = 0; i < n; i++) 
		matrix[i] = (int*) malloc(sizeof(int*)*n);
	
	k = 0;
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			matrix[j][i] = rbuf[k];
			k++;
		}
	}
	
	if (rank == loggable_rank) {
		printf("\n\nResult matrix:\n");
		print_matrix(matrix, n, n);
	}
}

int main(int argc, char **argv) {

	int rank, size, n, log_rank = -1;
	double t1, t2;
    
    MPI_Init( &argc, &argv );
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    if (log_rank > 0 && log_rank < size) {
    	transpose(8, rank, size, log_rank);
    } else {
    	for (n = 100; n <= 1000; n += 100) {
    		MPI_Barrier(MPI_COMM_WORLD);
    		if (rank == 0) {
    			t1 = MPI_Wtime();
    		}
    	
    		transpose(n, rank, size, log_rank);

			MPI_Barrier(MPI_COMM_WORLD);
    		if (rank == 0) {
    			t2 = MPI_Wtime();
    			printf("%d x %d -> time = %f seconds\n", n, n, t2 - t1);
    		}
    	} 
    }
	
	MPI_Finalize();
    return 0;
}
