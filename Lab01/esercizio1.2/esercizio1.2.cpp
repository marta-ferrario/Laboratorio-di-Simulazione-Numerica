#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include "../../Generatore_random/random.h"

using namespace std;

int main() {

    // Inizializzazione generatore di numeri casuali
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

    ///////////////////////////////////////////////////////////////////

    // test della distribuzione exp
    ofstream out_test_exp("test_exp.dat"); 
    for (int i=0; i<1000; i++){
        double x=rnd.Exp(2.5); 
        out_test_exp << x << endl; 
    }
    out_test_exp.close(); 

    // test della distribuzione CL
    ofstream out_test_CL("test_CL.dat"); 
    for (int i=0; i<1000; i++){
        double x=rnd.CL(0,0.5); 
        out_test_CL << x << endl; 
    }
    out_test_CL.close(); 


    ///////////////////////////////////////////////////////////////////

    int N[4]={1,2,10,100}; 
    double SN[4]; 
    double sum=0;

    // distribuzione uniforme (dado)
    ofstream out_unif("unif.dat");
    for(int i=0; i<10000; i++){
        for(int j=0; j<4; j++){
            for(int k=0; k<N[j]; k++){
                double u=rnd.Rannyu(1,6); 
                sum=sum+u;
            }
            SN[j]=sum/N[j]; 
            sum=0; 
        }
        out_unif << SN[0] << "\t" << SN[1] << "\t" << SN[2] << "\t" << SN[3] << endl;
    }
    out_unif.close(); 


    // distribuzione esponenziale
    ofstream out_exp("exp.dat");
    for(int i=0; i<10000; i++){
        for(int j=0; j<4; j++){
            for(int k=0; k<N[j]; k++){
                double u=rnd.Exp(1); 
                sum=sum+u;
            }
            SN[j]=sum/N[j]; 
            sum=0; 
        }
        out_exp << SN[0] << "\t" << SN[1] << "\t" << SN[2] << "\t" << SN[3] << endl;
    }
    out_exp.close(); 


    // distribuzione di Cauchy-Lorentz
    ofstream out_CL("CL.dat");
    for(int i=0; i<10000; i++){
        for(int j=0; j<4; j++){
            for(int k=0; k<N[j]; k++){
                double u=rnd.CL(0,1); 
                sum=sum+u;
            }
            SN[j]=sum/N[j]; 
            sum=0; 
        }
        out_CL << SN[0] << "\t" << SN[1] << "\t" << SN[2] << "\t" << SN[3] << endl;
    }
    out_CL.close(); 
    return 0; 
}