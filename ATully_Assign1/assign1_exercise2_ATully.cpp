#include <iostream>
#include <mpi.h>
using namespace std;

int main(){
    int N{3};  					// Initialise size of matrices (and in turn number of processes)
    int rank{},size{};				// Initialise variables for rank and size for use with MPI
    int A[N]{}, B[N*N]{}, C[N]{},D[N*N]{};	// Initialise variables for the matrices used throughout
    int i{}, j{}, i_mat{}, j_mat{};		// Initialise indexing variables (_mat versions used as disparity between matrix indexing and cpp array indexing)
    MPI_Comm comm;
    
    comm = MPI_COMM_WORLD;
    
    MPI_Init(NULL,NULL);
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);

    int root_process{0};			// Define the root process as 0

    if (rank==root_process) {			// If you are the root process..
	    for (i=0; i<N; ++i) {
		    i_mat = i+1;
		    for (j=0; j<N; ++j) {
			    j_mat = j+1;

			    B[i*N + j] = (j_mat + i_mat) * (N - j_mat + 1); 	// ..create the matrix B (Here stored as a flat array as I struggled to get 2D arrays to send with MPI)
		    }
	    }
    }

    MPI_Bcast(&B[0],N*N,MPI_INT,root_process, MPI_COMM_WORLD);			// Broadcast the matrix B
    
    i_mat = rank + 1;
    for (j=0; j<N; j++) {
	    j_mat = j+1;

	    A[j] = (N - j_mat +i_mat + 1) * i_mat;				// All processes create their designated row of matrix A
    }

    for (j=0; j<N; j++) {
	    for (int k=0; k<N; k++) {
		    C[j] += A[k] * B[k*N + j];					// All processes compute their designated row of the solution
	    }
    }

    if (rank==root_process) { 			// If you are the root process..
	    MPI_Gather(&C[0], N, MPI_INT, D, N, MPI_INT, root_process, MPI_COMM_WORLD);		// ..gather the rows of the solution from the other processes in a flat array  D..
	    
	    for (i=0;i<N;i++) {
		    for (j=0;j<N;j++) {
			    cout << D[i*N + j] << " ";						// ..and construct the print the solution matrix from the pieces stored in D
		    }
		    cout << endl;
	    }
    }

    else {					// If you are not the root process..
	    MPI_Gather(&C[0], N, MPI_INT, NULL, 0, MPI_INT, root_process, MPI_COMM_WORLD);	// ..send your designated row of the solution to the root process
    }
    
    
    MPI_Finalize();
}
