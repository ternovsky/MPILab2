#include <stdio.h> 
#include "mpi.h"
#define DEFAULT_TAG 0
#define SIZE 4

int main(int argc, char **argv) {
    int rank, size, N = 5, n, i, j, value, size_of_int;
    
    int a[2][4] = {0, 0, 0, 0, 1, 1, 1, 1};
    int b[8];

    MPI_Status status;
    MPI_Init( &argc, &argv );
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    MPI_Datatype columntype;
    MPI_Datatype matrixtype;
    //MPI_Type_vector(2, 1, 4, MPI_INT, &columntype);
    MPI_Type_vector(4, 1, 4, columntype, &matrixtype); 
	MPI_Type_commit(&matrixtype); 
	
	if (rank == 1) {
		MPI_Send(&a, 1, matrixtype, 0, 0, MPI_COMM_WORLD);
	}
	if (rank == 0) {
		MPI_Recv(&b, 8, MPI_INT, 1, 0, MPI_COMM_WORLD,&status);
		for (i = 0; i < 8; i++) printf("%d ", b[i]);
	}

	/*
	n = ceil((double)N/size);
	int** matrix = (int*) malloc(sizeof(int)*n);
	
	value = rank*n;
	for (i = 0; i < n && value < N; i++) {
		matrix[i] = (int) malloc(sizeof(int)*N);
		for (j = 0; j < N; j++) {
			matrix[i][j] = value;	
		}
		value++;
	}*/
	
	MPI_Finalize();
    return 0;
}

/*#include "mpi.h" 
#define SIZE 4 
int main(argc,argv) 
int argc; 
char *argv[];  { 
int numtasks, rank, source=0, dest, tag=1, i, j; 
float a[SIZE][SIZE] =  
  {1.0, 2.0, 3.0, 4.0,   
   5.0, 6.0, 7.0, 8.0,  
   9.0, 10.0, 11.0, 12.0, 
  13.0, 14.0, 15.0, 16.0}; 
float b[SIZE][SIZE];  
MPI_Status stat; 
MPI_Datatype columntype; 

MPI_Init(&argc,&argv); 
MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
MPI_Comm_size(MPI_COMM_WORLD, &numtasks); 
    
MPI_Type_vector(SIZE, 1, SIZE, MPI_FLOAT, &columntype); 
MPI_Type_commit(&columntype); 
	
	//MPI_Gather(&a[rank], SIZE, MPI_FLOAT, &b, 1, columntype, 0, MPI_COMM_WORLD);


	MPI_Send(&a[rank], SIZE, MPI_FLOAT, source, tag, MPI_COMM_WORLD); 	
  if (rank == 0) { 
     for (i=0; i < numtasks; i++)  
        MPI_Recv(&b[i], 1, columntype, i, tag, MPI_COMM_WORLD, &stat);
        
     
     for (i = 0; i < SIZE; i++) {
     	for (j = 0; j < SIZE; j++) {
     		printf("%f ", b[i][j]);
     	}
     	printf("\n");     
     } 
  } 
 MPI_Finalize(); 
} */
