#include "lib8.2.h"

using namespace std;


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


    // LETTURA PARAMETRI OTTIMALI
    ifstream in("OUTPUT/optimal.dat");
    double mu, sigma; 
    if (!in) {
        cout << "Errore nell'apertura del file" << endl;
        return 1;
    } 
    in >> mu;
    in >> sigma;
    in.close();


    // GRAFICI CON PARAMETRI OTTIMALI
    double x=0;
    double Delta = 2.635;
    // check_acc(mu, sigma, rnd, Delta); 

    int N_equil=200000;
    for (int i=0; i<N_equil; i++){
        x = Metropolis(x, Delta, mu, sigma, rnd); 
    }

    int M = 100000; //numero di lanci totali
    int N = 100; //numero di blocchi
    int L = M/N; //numero di lanci per blocco


    ofstream out("OUTPUT/data.dat"); 
    ofstream out_acc("OUTPUT/acc.dat"); 
    ofstream out_hist("OUTPUT/histo.dat"); 
    double sum_ave=0; 
    double sum_ave2=0; 
    double x_prec=x; 
    for (int iblk=0; iblk<N; iblk++){
        double sum_block=0; 
        double acc=0; 
        for(int j=0; j<L; j++){
            x_prec=x;
            x = Metropolis(x, Delta, mu, sigma, rnd); 
            double e = local_energy(mu, sigma, x); 
            sum_block += e; 

            // accettanza
            if (x_prec != x) {
                acc ++; 
            }

            // istogramma
            out_hist << x << endl; 
        }
        double ave_blk = sum_block/L; 
        double ave2_blk = ave_blk*ave_blk; 
    
        sum_ave += ave_blk; 
        sum_ave2 += ave2_blk; 
        double val = sum_ave/(iblk+1); 
        double val2 = sum_ave2/(iblk+1); 
        double err = err_stat(val, val2, iblk); 
        out << endl << iblk+1  << " " << val << " " << err; 

        double acc_rate = acc/L;
        out_acc << iblk+1 << " " << acc_rate << endl; 
    }
    out.close(); 


    return 0; 
}

