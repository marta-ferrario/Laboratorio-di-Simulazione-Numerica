#include <fstream>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

#include "Individual.h"
#include "City.h"
#include "Population.h"
#include "GeneticAlgorithm.h"


using namespace std; 

int main (){

    // GENERATORE NUMERI CASUALI
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


    int n_cities = 34; 
    int pop_size = 200; 
    int n_gen=600;


    // CITTÀ SULLA CIRCONFERENZA
    vector<City> cities_c; 
    for(int i=0; i<n_cities; i++) {
        double theta = rnd.Rannyu(0,2*M_PI);
        double x = cos(theta);
        double y = sin(theta); 
        cities_c.push_back( City(x,y) );
    }

    ofstream out_loss_c("../Output/best_loss_c.dat"); 
    ofstream out_check_c("../Output/check_c.dat"); 
    ofstream out_ave_c("../Output/ave_c.dat"); 
    GeneticAlgorithm GA_c(pop_size, n_cities, cities_c, rnd); 
     
    for(int gen=0; gen<n_gen; gen++) {
        GA_c.Evolve(rnd);
        out_loss_c << gen << " " << GA_c.GetBest().GetFitness() << endl; // stampa della lunghezza del miglior percorso
        out_check_c << gen << " " << GA_c.GetBest().Check() << endl;  // stampa 1 se tutto ok
        out_ave_c << gen << " " << GA_c.GetBestHalfAverage() << endl; // stampa la lunghezza media della migliore metà dei percorsi
    }

    ofstream out_path_c ("../Output/best_path_c.dat");
    Individual best_ind_c = GA_c.GetBest(); 
    vector<int> best_path_c = best_ind_c.GetPath(); 
    for(int i=0; i<n_cities; i++){
        int c = best_path_c[i]; 
        out_path_c  << c << " " << cities_c[c].GetX() << " " << cities_c[c].GetY() << endl;  // stampa del miglior percorso (coordinate delle città)
    }

    int c0_c = best_path_c[0]; // stampa alla fine la prima città per chiudere il tour
    out_path_c << "1" << " " << cities_c[c0_c].GetX() << " " << cities_c[c0_c].GetY() << endl;



    // CITTÀ NEL QUADRATO
    vector<City> cities;
    for(int i=0; i<n_cities; i++) {
        double x = rnd.Rannyu();
        double y = rnd.Rannyu();
        cities.push_back( City(x,y) );
    }

    ofstream out_loss("../Output/best_loss.dat"); 
    ofstream out_check("../Output/check.dat"); 
    ofstream out_ave("../Output/ave.dat"); 
    GeneticAlgorithm GA(pop_size, n_cities, cities, rnd);  
    for(int gen=0; gen<n_gen; gen++) {
        GA.Evolve(rnd);
        out_loss << gen << " " << GA.GetBest().GetFitness() << endl;
        out_check << gen << " " << GA.GetBest().Check() << endl; 
        out_ave << gen << " " << GA.GetBestHalfAverage() << endl; 
        
    }

    ofstream out_path ("../Output/best_path.dat");
    Individual best_ind = GA.GetBest(); 
    vector<int> best_path = best_ind.GetPath(); 
    for(int i=0; i<n_cities; i++){
        int c = best_path[i]; 
        out_path  << c << " " << cities[c].GetX() << " " << cities[c].GetY() << endl; 
    }

    int c0 = best_path[0]; // stampo alla fine la prima città per chiudere il tour
    out_path << "1" << " " << cities[c0].GetX() << " " << cities[c0].GetY() << endl;


    return 0; 


}