#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include "../../Generatore_random/random.h"

using namespace std;

double err_stat(double av, double av2, int n);

int main () {

    Random rnd; 

    ///////////////////////////////////////////////////////////////////
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


    ///////////////////////////////////////////////////////////////////


    int M=120; //blocchi
    int N=1000000; //lanci totali
    int Lanci=N/M; //lanci per blocco

    double x; //distanza dalla riga
    double d=1.; //distanza tra le linee 
    double L=0.5; //lunghezza ago

    int N_hit=0; 
    double pi;  
    double pi_varianza; 
    double pi2; 
    double sum_pi=0; 
    double sum_pi2=0;

    ofstream out("data.dat"); 
    for(int j=0; j<M; j++){
        for(int i=0; i<Lanci; i++){
            x=rnd.Rannyu(0,d/2); 

            // estrazione di sin(theta) senza utilizzare pi
            double u = rnd.Rannyu(-1, 1);
            double v = rnd.Rannyu(-1, 1);
            while(u*u + v*v > 1){
                u = rnd.Rannyu(-1, 1);
                v = rnd.Rannyu(-1, 1);
            } // rigetto per essere nel cerchio unitario
            double sin_theta = v / sqrt(u*u + v*v);
            if(x<(L/2)*fabs(sin_theta)){
                N_hit++;
            }
        

        }
        double pi_block=(2*L*Lanci)/(d*N_hit);
        sum_pi  += pi_block;
        sum_pi2 += pi_block*pi_block;  
        N_hit=0; 

        pi = sum_pi / (j+1);
        pi2 = sum_pi2 / (j+1); 
        pi_varianza=err_stat(pi, pi2, j+1); 
        

        out << endl << j+1  << " " << pi << " " << pi_varianza;  
    }
    out.close(); 

    return 0; 
}

double err_stat(double av, double av2, int n) {
    if((n-1) == 0) {
        return 0;
    }
    else {
        return sqrt((av2 - av*av)/n);
    }
}