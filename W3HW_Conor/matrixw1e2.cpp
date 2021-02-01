#include <iostream>
#include <mpi.h>
#include <vector>
#include <unistd.h>

using namespace std; 
const int N = 3;  //Set N as a constant the whole way through, ensure this is the same as the tasks in slurm file
int main ()
{   
    int rank, size, ierr;
    MPI_Comm comm;

    comm  = MPI_COMM_WORLD;   //ensure the communicator is every proccess

    //initialise MPI and attach values to rank and size depending on which process is running the script
    MPI_Init(NULL,NULL);
    MPI_Comm_rank(comm, &rank);            
    MPI_Comm_size(comm, &size);

    // intialise 2D arrays B and C...
    int B[N][N];
    int C[N][N];
    // ... and ensure they are equal to 0.
    for(int i = 0; i < N; i++)
        {
           for(int j = 0; j < N; j++)
            {
                B[i][j] = 0;
                C[i][j] = 0;
            }
        }
        // if process is the root process it computes matrix B...
        if (rank == 0)
        {
            for (int i = 0; i < N; i++)
            {
                for (int j = 0; j < N; j++)
                {
                    B[i][j] = (j + i + 2) * (N - j);
                }
                
            }
        }
        // ... then broadcasts it to all the other processess....
        MPI_Bcast(&B, N*N, MPI_INT, 0, MPI_COMM_WORLD);
        int row[N];
        int Cr[N];
        // ... which each calculates their Crow....
        for (int j = 0; j < N; j++)
        {
            int A_ij = (N - j + rank + 1) * (rank + 1);
            row[j] = A_ij;
        }
        for (int i = 0; i < N; i++)
        {
            for (int k = 0; k < N; k++)
            {
                Cr[i] += row[k] * B[k][i];
            }
        }
        // ... and prints them out to be sure.
	cout << Cr[0]<<" " << Cr[1]<<" " << Cr[2] << " I'm from Rank " << rank << endl;
	
    // Return the rows of matrix C to the root process for it to store in 2D array C.
	MPI_Gather(&Cr, N, MPI_INT, C, N, MPI_INT, 0, MPI_COMM_WORLD);
    
    //Only the root process then prints out the final matrix C which has been computed
	if(rank == 0)
	  {
    for(int i=0; i<N; i++)    //This loops on the rows.
        {
            for(int j=0; j<N; j++) //This loops on the columns
            {
	      cout << C[i][j]  << "  " << " ";
            }
            cout << endl;
        }
	  }
    // Finalise the MPI
    MPI_Finalize();
}
