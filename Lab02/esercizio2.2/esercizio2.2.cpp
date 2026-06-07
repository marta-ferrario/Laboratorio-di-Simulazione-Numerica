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


    int walk=10000; //numero random walk
    int passi=100; //passi per random walk
    int blocchi=100; //numero di blocchi
    int walk_blocchi=walk/blocchi; //numero di walk per ogni blocco


    int posizione[3]={0,0,0};  
    double r2[passi]; 
    double sum[passi]; 
    double r2_blocks[blocchi][passi]; // matrice dibimensionale[righe][colonne]


    for(int i=0; i<passi; i++){
        sum[i]=0;
    }


    // CUBIC LATTICE
    for(int k=0; k<blocchi; k++){
        for(int j=0; j<walk_blocchi; j++){
            posizione[0]=0; // posizione azzerata all'inizio di ogni cammino
            posizione[1]=0;
            posizione[2]=0;
            for(int i=0; i<passi; i++){
                int asse=(int)rnd.Rannyu(0,3); //0=x, 1=y, 2=z
                int verso=(int)rnd.Rannyu(0,2); //0=indietro 1=avanti
                int segno;
                if(verso==0){
                    segno=-1; 
                } else{
                    segno=+1; 
                }

                posizione[asse]+= segno;
                r2[i]=(posizione[0]*posizione[0] + posizione[1]*posizione[1] + posizione[2]*posizione[2]);
                sum[i]+=r2[i];
            }
        }

        // calcolo della media di r2 per ogni passo (per il k-blocco)
        for (int u = 0; u<passi; u++){
            r2_blocks[k][u] = sum[u]/walk_blocchi;
        } 

        for(int i=0; i<passi; i++){
            sum[i]=0; 
        }
    }

    // calcolo media e incertezza sui blocchi
    ofstream out_lattice("data_lattice.dat");
    for(int i=0;i<passi;i++){
        double media=0;
        double media2=0;

        for(int k=0;k<blocchi;k++){
            media += r2_blocks[k][i];
            media2 += r2_blocks[k][i] *r2_blocks[k][i];
        }

        media/=blocchi;
        media2/=blocchi;

        double valore=sqrt(media);
        double errore = 0.5/sqrt(media)*err_stat(media,media2,blocchi);

        out_lattice << i+1 << " " << valore << " "<< errore << endl;
    }
    out_lattice.close();

    // CONTINUO
    double posizione_cont[3]={0.,0.,0.}; 
    for(int k=0; k<blocchi; k++){
        for(int j=0; j<walk_blocchi; j++){
            posizione_cont[0]=0;
            posizione_cont[1]=0;
            posizione_cont[2]=0;
            for(int i=0; i<passi; i++){
                double theta = rnd.Rannyu(0, M_PI); 
                double phi = rnd.Rannyu(0, 2*M_PI); 

                posizione_cont[0] += sin(theta) * cos(phi); 
                posizione_cont[1] += sin(theta) * sin(phi);
                posizione_cont[2] += cos(theta); 

                r2[i]=(posizione_cont[0]*posizione_cont[0] + posizione_cont[1]*posizione_cont[1] + posizione_cont[2]*posizione_cont[2]);
                sum[i]+=r2[i];
            }
        }

        for (int u = 0; u<passi; u++){
            r2_blocks[k][u] = sum[u]/walk_blocchi;
        } 

        for(int i=0; i<passi; i++){
            sum[i]=0; 
        }
    }

    // calcolo media e incertezza sui blocchi
    ofstream out_cont("data_cont.dat");
    for(int i=0;i<passi;i++){
        double media=0;
        double media2=0;

        for(int k=0;k<blocchi;k++){
            media += r2_blocks[k][i];
            media2 += r2_blocks[k][i] *r2_blocks[k][i];
        }

        media/=blocchi;
        media2/=blocchi;

        double valore=sqrt(media);
        double errore = 0.5/sqrt(media)*err_stat(media,media2,blocchi);

        out_cont << i+1 << " " << valore << " "<< errore << endl;
    }

    out_cont.close();




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