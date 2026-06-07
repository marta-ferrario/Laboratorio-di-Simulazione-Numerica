#include "lib8.2.h"


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
    double sigma=0.5, mu=1.1; // parametri iniziali
    double delta_mu = 0.2;
    double delta_sigma = 0.2;
    double T = 1.0; 

    // SIMULATED ANNEALING
    ofstream out_SA("OUTPUT/SA_energy.dat"); 
    ofstream out_param("OUTPUT/SA_param.dat"); 
    double E_old=ave_energy(mu, sigma, rnd); 
    for(int t=0; t<70; t++){
        for (int i=0; i<300; i++){
            double mu_new = mu + rnd.Rannyu(-delta_mu, delta_mu); 
            double sigma_new = sigma + rnd.Rannyu(-delta_sigma, delta_sigma); 

            if (sigma_new <=0) continue; 
            if (mu_new <=0) continue; 

            double E_new = ave_energy(mu_new, sigma_new, rnd); 
            double beta = 1/T; 
            double p = exp(-beta * (E_new - E_old)); 

            if(p >= rnd.Rannyu()){
                mu = mu_new; 
                sigma = sigma_new; 
                E_old = E_new; 
            }

            out_SA << t*100 + i << " " << beta << " " << E_old << endl;
            out_param << t*100 + i << " "  << beta << " " << mu << " " << sigma << endl;

        }
        T *= 0.9; 
        cout << t << endl; 
    }

    cout << "Parametri ottimali:" << endl;
    cout << "mu = " << mu << endl;
    cout << "sigma = " << sigma << endl;
    cout << "Energia = " << E_old << endl; 



    ofstream out("OUTPUT/optimal.dat"); 
    out << mu << endl; 
    out << sigma << endl; 
    out.close(); 


    return 0; 
}