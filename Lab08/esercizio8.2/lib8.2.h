#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include "../../Generatore_random/random.h"

using namespace std; 


double psi(double mu, double sigma, double x);
double pot(double x); 
double psi_der2(double mu, double sigma, double x); 
double local_energy(double mu, double sigma, double x);
double Metropolis(double xold, double Delta, double mu, double sigma, Random &rnd); 
double ave_energy(double mu, double sigma, Random &rnd); 
double check_acc(double mu, double sigma, Random &rnd, double Delta); 

double err_stat(double av, double av2, int n);



