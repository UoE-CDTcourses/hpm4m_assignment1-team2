#include <iostream>
#include <vector>
#include <math.h>
#include <cmath>
#include <mpi.h>

using std::cout;
using std::cin;
using std::abs;

//function to be integrated
double function(double x, double y)
{
    return x * sin(pow(x, 2)) + y * sin(pow(y,2));
}

// How it does the integration
double integrate(double a, double b, double c, double d, int n, int m)
{
    double step_1 = (b-a)/n;
    double step_2 = (d-c)/m;
    double output = 0;

    for (int i = 0 ; i <= n; i++)
    {
        for (int j = 0; j <= m; j++)
            {
                output += function(a + i * step_1, c + j * step_2 );
            }
    
    }
    output *= step_1 * step_2;
    return output;
}

int main() //main function
{
    int rank, size, ierr;
    MPI_Comm comm;

    comm  = MPI_COMM_WORLD;   //ensure the communicator is every proccess

    //initialise MPI and attach values to rank and size depending on which process is running the script
    MPI_Init(NULL,NULL);
    MPI_Comm_rank(comm, &rank);            
    MPI_Comm_size(comm, &size);

    double N = 8; //Number of processes, ensure you have changed in slurm file as well.
    int grid = pow(10, 5); // Number of grid points all together.
    double answer, a, b, c, d;
    for (int i = 0; i < N; i++)
    {
        a = 100 * rank / N;
        b = 100 * (rank + 1)/ N;
        //c = 100 * rank / N;
	    //d = 100 * (rank + 1)/ N;
	    c=0;
	    d=100;
	    int n = grid / N;
        answer = integrate(a,b,c,d,n,n);
    }
    // Just to see what is coming from where
    std::cout << answer << " This was the int I worked out and I'm rank  "<< rank <<". From "<< a<<" to "<< b<<std::endl;
    double int_total;

    // Sum all of the anwers together and return to the root process
    MPI_Reduce(&answer, &int_total, N,MPI_DOUBLE, MPI_SUM, 0, comm);
    
    // Then the root process only returns the total integral
    if(rank == 0)
    {
        std::cout << "The total integral is equal to " << int_total  << std::endl;
    }
    // Finalise the MPI
    MPI_Finalize();
}
