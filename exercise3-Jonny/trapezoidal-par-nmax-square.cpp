#include <iostream>
#include <cmath>
#include <ctime>
#include <mpi.h>
using namespace std;

// MPI implementation using square sub-division.


// Define problem parameters
double a = 10, b = 10;
double trueSol = 0.5*(-b*cos(pow(a, 2)) - a*cos(pow(b, 2)) + a + b);


int main (){
    // Save data for plotting
  int powMax = 15;
  // Initialise MPI interface
  int rank, size, root = 0;
  MPI_Comm comm = MPI_COMM_WORLD;
  clock_t t;
  t = clock();
  MPI_Init(NULL, NULL);
  MPI_Comm_rank(comm, &rank);
  MPI_Comm_size(comm, &size);

  // Approximate integral using Nmax = 2^15
    int n = pow(2, 15); 
    double dx = a/n, dy = b/n; // Initialise variables
    double totalInt = 0;

    double x0 = round(rank*n/pow(size,0.5))*dx, x1 = round((rank+1)*n/pow(size,0.5))*dx,
      y0 = round((rank)*n/pow(size,0.5))*dy, y1 = round((rank+1)*n/pow(size,0.5))*dy;  // Split grid into rectangles
    double integral = 0;
    for (double x = x0; x < x1; x += dx){
      for (double y = y0; y < y1; y += dy){
        // Add total value of f at 4 corners of gridpoint
        // NB: the true term is twice the computed value, thus the average is half
        // this value, we divide the whole sum by 2 at the end.
        integral += x*sin(pow(x, 2)) + y*sin(pow(y, 2))
	  + (x + dx)*sin(pow(x+dx, 2))+ (y + dy)*sin(pow(y+dy, 2));
      }
    }
    // Add output from all parallel processes
    MPI_Reduce(&integral, &totalInt, 1, MPI_DOUBLE, MPI_SUM, root, comm);
    // Rescale integral using dx, dy and the factor of 4 from the average
    totalInt *= dx*dy/2;
    t = clock() - t;
    

  MPI_Finalize();
  if (rank == root){
  cout << "Time: " << t << "\n";
  }
  return 0;
}
