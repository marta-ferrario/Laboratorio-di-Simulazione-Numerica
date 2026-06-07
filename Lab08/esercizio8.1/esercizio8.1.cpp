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
void check_acc(double mu, double sigma, Random &rnd, double Delta); 

double err_stat(double av, double av2, int n);


int main () {

    // GENERATORE DI NUMERI
    Random rnd;
    int seed[4];
    int p1, p2;
    ifstream Primes("../../Generatore_random/Primes");
    if (Primes.is_open()){
        Primes >> p1 >> p2 ;
    } else cerr << "PROBLEMA: impossibile aprire Primes" << endl;
    Primes.close();

    ifstream input("../../Generatore_random/seed.in");
    string property;
    if (input.is_open()){
        while ( !input.eof() ){
            input >> property;
            if( property == "RANDOMSEED" ){
                input >> seed[0] >> seed[1] >> seed[2] >> seed[3];
                rnd.SetRandom(seed,p1,p2);
            }
        }
        input.close();
    } else cerr << "PROBLEMA: impossibile aprire seed.in" << endl;

    // PARAMETRI
    double sigma=0.5, mu=1.1; // parametri scelti
    double Delta = 1.1001; 
    int M = 100000; //numero di lanci totali
    int N = 100; //numero di blocchi
    int L = M/N; //numero di lanci per blocco
    double x=0;


    // // EQUILIBRAZIONE
    int N_equil=20000;
    for (int i=0; i<N_equil; i++){
        x = Metropolis(x, Delta, mu, sigma, rnd); 
    }

    // DETERMINAZIONE DI DELTA
    // check_acc(mu, sigma, rnd, Delta); 
    

    // DATA BLOCKING
    ofstream out("data.dat"); 
    double sum_ave=0; 
    double sum_ave2=0; 
    for (int iblk=0; iblk<N; iblk++){
        double sum_block=0; 
        for(int j=0; j<L; j++){
            x = Metropolis(x, Delta, mu, sigma, rnd); 
            double e = local_energy(mu, sigma, x); 
            sum_block += e; 
        }
        double ave_blk = sum_block/L; 
        double ave2_blk = ave_blk*ave_blk; 

        sum_ave += ave_blk; 
        sum_ave2 += ave2_blk; 
        double val = sum_ave/(iblk+1); 
        double val2 = sum_ave2/(iblk+1); 
        double err = err_stat(val, val2, iblk); 
        out << endl << iblk+1  << " " << val << " " << err; 
    }
    out.close(); 




    return 0; 
}


double psi(double mu, double sigma, double x){
    double psi = exp(- pow((x- mu),2)/(2*sigma*sigma)) + exp(- pow((x+ mu),2)/(2*sigma*sigma));
    return psi; 
}

double pot(double x){
    double V = pow(x,4) - ((5.)/(2.))*pow(x,2); 
    return V; 
}

double psi_der2(double mu, double sigma, double x){
    double sigma2 = sigma * sigma;
    double sigma4 = sigma2 * sigma2;
    double exp1 = exp(((-x*x - mu*mu) / 2.0 + mu*x) / sigma2);
    double exp2 = exp(((-x*x - mu*mu) / 2.0 - mu*x) / sigma2);
    double term1 = (x*x - 2*mu*x - sigma2 + mu*mu) * exp1 / sigma4;
    double term2 = (x*x / sigma4 + 2*mu*x / sigma4 - 1.0 / sigma2 + mu*mu / sigma4) * exp2;

    return term1 + term2;
}

double local_energy(double mu, double sigma, double x){
    double loc_en = -0.5 * (psi_der2(mu, sigma, x)/psi(mu,sigma,x)) + pot(x);
    return loc_en; 
}

double Metropolis(double xold, double Delta, double mu, double sigma, Random &rnd){
    double delta = rnd.Rannyu(-Delta,Delta); 
    double xnew = xold + delta; 

    double frac = pow(psi(mu,sigma,xnew),2) / pow(psi(mu,sigma,xold),2); 
    double A = min(1.0,frac); 

    double r = rnd.Rannyu(); 
    if (r<A){
        return xnew; 
    } else{
        return xold; 
    }

}


void check_acc(double mu, double sigma, Random &rnd, double Delta){
    double x=0.; 
    double x_prec; 
    double acc=0;
    double rej=0; 
    double n_tot = 100000; 
    for (int i=0; i < n_tot; i++){
        x_prec=x; 
        x=Metropolis(x, Delta, mu, sigma, rnd); 
        if (x_prec == x) {
            rej ++; 
        } else {
            acc ++; 
        }
    }
    double acc_rate = acc/ n_tot; 
    cout << "Percetuale mosse accettate: " << acc_rate << endl;
}

double err_stat(double av, double av2, int n) {
    if(n == 0) {
        return 0;
    }
    else {
        return sqrt((av2 - av*av)/n);
    }
}