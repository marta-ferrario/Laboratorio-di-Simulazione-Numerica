#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include "../../Generatore_random/random.h"

using namespace std;

double err_stat(double av, double av2, int n);

int main(){


    // inizializzazione generatore numeri casuali
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


    int N=100; //numero blocchi
    int M=10000; //numero lanci totali
    int L=M/N; //numero lanci per blocco

    // variabili per sampling uniforme
    double sum_blocco=0;
    double med_blocco;
    double somma_medie=0;
    double media_fin;
    double media2;
    double sm2=0;
    double mf2=0;
    double varianza;

    // variabili per importance sampling
    double imp_sum_blocco=0;
    double imp_med_blocco;
    double imp_somma_medie=0;
    double imp_media_fin;
    double imp_media2;
    double imp_sm2=0;
    double imp_mf2=0;
    double imp_varianza;


    ofstream out("data.dat");
    ofstream out_imp("data_imp.dat");


    for(int j=0; j<N; j++){
        for (int i=0; i<L; i++){
            double x=rnd.Rannyu(); //estrazione uniforme 
            double x_imp = 1 - sqrt(1 - x); //estrazione secondo p(x)
            double f = (M_PI/2)*cos((M_PI*x)/2);
            double f_imp= (M_PI/2)*cos((M_PI*x_imp)/2);
            double p = 2*(1-x_imp);
            sum_blocco=sum_blocco+f;
            imp_sum_blocco=imp_sum_blocco+(f_imp/p);
        }

        med_blocco=sum_blocco/L;
        media2=med_blocco*med_blocco;
        imp_med_blocco=imp_sum_blocco/L;
        imp_media2=imp_med_blocco*imp_med_blocco;

        imp_somma_medie=imp_somma_medie+imp_med_blocco;
        imp_sm2=imp_sm2+imp_media2;
        somma_medie=somma_medie+med_blocco;
        sm2=sm2+media2;

        media_fin=somma_medie/(j+1);
        mf2=sm2/(j+1);
        varianza=err_stat(media_fin,mf2,j+1);
        imp_media_fin=imp_somma_medie/(j+1);
        imp_mf2=imp_sm2/(j+1);
        imp_varianza=err_stat(imp_media_fin,imp_mf2,j+1);

        sum_blocco=0;
        imp_sum_blocco=0;

        out << j+1 << " " << media_fin << " " << varianza << endl;
        out_imp << j+1 << " " << imp_media_fin << " " << imp_varianza << endl;
    }
    out.close();
    out_imp.close();
    return 0;
    }
    
double err_stat(double av, double av2, int n) {
    if(n == 0) {
        return 0;
    }
    else {
        return sqrt((av2 - av*av)/n);
    }
}