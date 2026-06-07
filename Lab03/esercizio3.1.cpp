#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <algorithm>
#include "../Generatore_random/random.h"

using namespace std;

double S(double T, double S0, double r, double sigma, double W); 
double N(double x); 
double call(double S, double t,double T, double S0, double r, double sigma, double K);
double put(double S, double t,double T, double S0, double r, double sigma, double K); 

double err_stat(double av, double av2, int n);

int main(){

    // inizializzazione generatore numeri casuali
    Random rnd;
    int seed[4];
    int p1, p2;
    ifstream Primes("../Generatore_random/Primes");
    if (Primes.is_open()){
        Primes >> p1 >> p2 ;
    } else cerr << "PROBLEMA: impossibile aprire Primes" << endl;
    Primes.close();

    ifstream input("../Generatore_random/seed.in");
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


    double T=1; 
    double S0=100; 
    double r=0.1; 
    double sigma=0.25; 
    double K=100; 


    // RISULTATI ATTESI
    ofstream out_atteso("data_atteso.dat");
    double c_atteso = call(S0,0,T,S0,r,sigma,K);
    double p_atteso = put(S0,0,T,S0,r,sigma,K); 

    out_atteso << c_atteso << " " << p_atteso; 
    
    // SIMULAZIONE CONTINUO
    int N=150; //numero blocchi
    int M=100000; //numero lanci totali
    int L=M/N; //numero lanci per blocco

    double somma_blocco_c=0; 
    double media_blocco_c; 
    double media2_c; 
    double somma_medie_c=0; 
    double sm2_c=0;
    double media_finale_c; 
    double mf2_c; 
    double varianza_c; 

    double somma_blocco_p=0; 
    double media_blocco_p; 
    double media2_p; 
    double somma_medie_p=0; 
    double sm2_p=0;
    double media_finale_p; 
    double mf2_p; 
    double varianza_p; 


    ofstream out_c("data_c.dat");
    ofstream out_p("data_p.dat");
    for(int j=0; j<N; j++){
        for(int i=0; i<L; i++){
            double W=rnd.Gauss(0,T);
            double St = S(T, S0, r, sigma, W); 
            double c_mc= exp(-r*T)*max(0.,St-K); 
            double p_mc= exp(-r*T)*max(0.,K-St); 
            somma_blocco_c+=c_mc;
            somma_blocco_p+=p_mc; 
        }
        media_blocco_c=somma_blocco_c/L;
        media2_c=media_blocco_c*media_blocco_c; 
        somma_medie_c+=media_blocco_c; 
        sm2_c+=media2_c; 

        media_finale_c=somma_medie_c/(j+1);
        mf2_c=sm2_c/(j+1);
        varianza_c=err_stat(media_finale_c,mf2_c,j+1);
        somma_blocco_c=0; 

        out_c << j+1 << " " << media_finale_c << " " << varianza_c << endl;


        media_blocco_p=somma_blocco_p/L;
        media2_p=media_blocco_p*media_blocco_p; 
        somma_medie_p+=media_blocco_p; 
        sm2_p+=media2_p; 

        media_finale_p=somma_medie_p/(j+1);
        mf2_p=sm2_p/(j+1);
        varianza_p=err_stat(media_finale_p,mf2_p,j+1);
        somma_blocco_p=0; 

        out_p << j+1 << " " << media_finale_p << " " << varianza_p << endl;
    }
    
    out_c.close(); 
    out_p.close();

    somma_blocco_c=0; 
    somma_blocco_p=0; 
    somma_medie_c=0; 
    somma_medie_p=0; 
    sm2_c=0;
    sm2_p=0;


    // SIMULAZIONE DISCRETA
    ofstream out_c_dis("data_c_dis.dat");
    ofstream out_p_dis("data_p_dis.dat");
    for(int j=0; j<N; j++){
        for(int i=0; i<L; i++){
            double S_dis=S0; 
            double dt = T/100.0; 
            for(int k=0; k<100; k++){
                double z = rnd.Gauss(0,1);
                S_dis *= exp((r - 0.5*sigma*sigma)*dt + sigma * z * sqrt(dt));
            }

            double St = S_dis; 
            double c_mc= exp(-r*T)*max(0.,St-K); 
            double p_mc= exp(-r*T)*max(0.,K-St); 
            somma_blocco_c+=c_mc;
            somma_blocco_p+=p_mc; 
        }
        media_blocco_c=somma_blocco_c/L;
        media2_c=media_blocco_c*media_blocco_c; 
        somma_medie_c+=media_blocco_c; 
        sm2_c+=media2_c; 

        media_finale_c=somma_medie_c/(j+1);
        mf2_c=sm2_c/(j+1);
        varianza_c=err_stat(media_finale_c,mf2_c,j+1);
        somma_blocco_c=0; 

        out_c_dis << j+1 << " " << media_finale_c << " " << varianza_c << endl;


        media_blocco_p=somma_blocco_p/L;
        media2_p=media_blocco_p*media_blocco_p; 
        somma_medie_p+=media_blocco_p; 
        sm2_p+=media2_p; 

        media_finale_p=somma_medie_p/(j+1);
        mf2_p=sm2_p/(j+1);
        varianza_p=err_stat(media_finale_p,mf2_p,j+1);
        somma_blocco_p=0; 

        out_p_dis << j+1 << " " << media_finale_p << " " << varianza_p << endl;
    }

    out_c_dis.close(); 
    out_p_dis.close();


    return 0;

}

double S(double T, double S0, double r, double sigma, double W){
    double S=S0*exp((r-0.5*sigma*sigma)*T + sigma*W); 
    return S; 
}

double N(double x){
    return 0.5*(1+erf(x/sqrt(2)));
}

double call(double S, double t,double T, double S0, double r, double sigma, double K){
    double d1 = (1/(sigma*sqrt(T-t))) * (log(S/K) + (r+0.5*sigma*sigma)*(T-t)); 
    double d2 = d1-sigma*sqrt(T-t); 
    double c = S*N(d1) - K*exp(-r*(T-t))*N(d2); 
    return c; 
}

double put(double S, double t,double T, double S0, double r, double sigma, double K){
    double d1 = (1/(sigma*sqrt(T-t))) * (log(S/K) + (r+0.5*sigma*sigma)*(T-t)); 
    double d2 = d1-sigma*sqrt(T-t); 
    double p = S*(N(d1)-1) - K*exp(-r*(T-t))*(N(d2)-1); 

    return p;
}

double err_stat(double av, double av2, int n) {
    if(n == 0) {
        return 0;
    }
    else {
        return sqrt((av2 - av*av)/n);
    }
}