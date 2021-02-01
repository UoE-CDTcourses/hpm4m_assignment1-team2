#include <iostream>
#include <cmath>
#include <fstream>

// Serial implementation of the trapezoidal rule for the problem.

using namespace std;

// Define problem parameters
double a = 10, b = 10;
double trueSol = 0.5*(-b*cos(pow(a, 2)) - a*cos(pow(b, 2)) + a + b);

double trapezoidal (int nx, int ny, double x0, double x1, double y0, double y1)
{/*  
 nx, ny are the number of points in the x and y directions
 Integrate in x direction from a to b and y direction from c to d
 */
  double integral = 0, dx = (x1 - x0)/nx, dy = (y1 - y0)/ny; // Initialise variables
  for (double x = x0; x < x1; x += dx){
    for (double y = y0; y < y1; y += dy){
      // Add total value of f at 4 corners of gridpoint
      // NB: the true term is twice the computed value, thus the average is half
      // this value, we divide the whole sum by 2 at the end.
      integral += x*sin(pow(x, 2)) + y*sin(pow(y, 2))
	+ (x + dx)*sin(pow(x+dx, 2))+ (y + dy)*sin(pow(y+dy, 2));
    }
  }
  // Rescale integral using dx, dy and the factor of 4 from the average
  integral *= dx*dy/2;
  double error = abs(integral - trueSol);
  return error;
}

int main (){
  cout << "True solution: "<<  trueSol << "\n";  // Display true solution 
  // Save data for plotting
  ofstream fileNs, fileErr;
  fileNs.open("powers.txt");
  fileErr.open("errors.txt");  // Use 2^powMax steps in x and y directions
  
  int powMax = 15;
  
  // Approximate integral using 2^p gridpoints for 1 <= p <= powMax
  for (int p = 1; p <= powMax ; ++p){
    cout <<"p: " << p << "\n";
    int n = pow(2, p); 
    fileNs << n << "\n";
    double  err = trapezoidal(n, n, 0, a, 0, b);
    fileErr << err  << "\n";  // Compute and store error
  }
  fileNs.close();
  fileErr.close();
  return 0;
}
