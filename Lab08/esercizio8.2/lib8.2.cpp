#include "lib8.2.h"


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

double ave_energy(double mu, double sigma, Random &rnd){
    double Delta = 1.1001; 
    int M = 20000; //numero di lanci totali
    int N = 50; //numero di blocchi
    int L = M/N; //numero di lanci per blocco
    double x=0;

    // EQUILIBRAZIONE
    int N_equil=20000;
    for (int i=0; i<N_equil; i++){
        x = Metropolis(x, Delta, mu, sigma, rnd); 
    }

    // DATA BLOCKING
    double sum_ave=0;
    for (int iblk=0; iblk<N; iblk++){
        double sum_block=0; 
        for(int j=0; j<L; j++){
            x = Metropolis(x, Delta, mu, sigma, rnd); 
            double e = local_energy(mu, sigma, x); 
            sum_block += e; 
        }
        double ave_blk = sum_block/L; 
        sum_ave += ave_blk; 
    }

    double E = sum_ave/N; 
    return E; 
}


double check_acc(double mu, double sigma, Random &rnd, double Delta){
    double x=0.; 
    double x_prec; 
    double acc=0; 
    double n_tot = 100000; 
    for (int i=0; i < n_tot; i++){
        x_prec=x; 
        x=Metropolis(x, Delta, mu, sigma, rnd); 
        if (x_prec != x) {
            acc ++; 
        }
    }
    double acc_rate = acc/ n_tot; 
    return acc_rate; 
}


double err_stat(double av, double av2, int n) {
    if(n == 0) {
        return 0;
    }
    else {
        return sqrt((av2 - av*av)/n);
    }
}