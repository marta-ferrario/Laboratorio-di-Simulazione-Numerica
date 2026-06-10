#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include "../../Generatore_random/random.h"

using namespace std;

double err_stat(double av, double av2, int n);

int main () {

   int M = 1000000; //numero di lanci totali
   int N = 100; //numero di blocchi
   int L = M/N; //numero di lanci per blocco


   double average;
   double ave2; 
   double error;
   double sum_prog = 0;
   double sum2_prog = 0;


   // inizializzazione del generatore di numeri casuali
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


   // ESERCIZIO 1.1.1
   ofstream out("data.dat");
   for (int j = 0; j < N; j++) {
      double sum = 0;
      for (int i = 0; i < L; i++) {
         double r = rnd.Rannyu();
         sum += r; 
      }
      double media = sum / L;
      double media2 = media * media;

      sum_prog  += media;
      sum2_prog += media2;

      average = sum_prog / (j + 1);
      ave2 = sum2_prog / (j + 1); 
      error = err_stat(average, ave2, j + 1);

      out << (j+1)*L << " " << average << " " << error << endl;
   }
   out.close();



   // ESERCIZIO 1.1.2
   ofstream out2("data2.dat");
   sum_prog=0;
   sum2_prog=0;

   for (int j = 0; j < N; j++) {
      double sum = 0;
      for (int i = 0; i < L; i++) {
         double r = rnd.Rannyu();
         sum += (r-0.5)*(r-0.5);
      }
      double media = sum / L;
      double media2 = media * media;

      sum_prog  += media;
      sum2_prog += media2;

      average = sum_prog / (j + 1);
      ave2 = sum2_prog / (j + 1); 
      error = err_stat(average, ave2, j + 1);

      out2 << (j+1)*L << " " << average << " " << error << endl;
   }
   out2.close(); 



   // ESERCIZIO 1.1.3: TEST DEL CHI QUADRO
   int intervalli=100;
   int contatori[intervalli]; 
   int estrazioni = 10000; 
   double chi=0; 

   ofstream out3("data_chi.dat");
   for(int j=0; j<100; j++){
      // inizializzo il vettore dei contatori
      for(int i=0; i<intervalli; i++){
         contatori[i]=0; 
      }

      // estraggo i numeri casuali e conto quanti numeri cadono in ogni intervallo
      for(int i=0; i<estrazioni; i++){
         double n=rnd.Rannyu();
         int bin = n*intervalli; // trovo l'intervallo
         contatori[bin]++; 
      }

      // calcolo del chi quadro
      for(int i=0; i<intervalli; i++){
         chi=chi+ pow((contatori[i]-(estrazioni/intervalli)),2)/(estrazioni/intervalli); 
      }
   
      out3 << chi << endl;
      chi=0; 
   }
   out3.close();
   return 0;
}


//////////////////

double err_stat(double av, double av2, int n) {
    if(n == 0) {
        return 0;
    }
    else {
        return sqrt((av2 - av*av)/n);
    }
}