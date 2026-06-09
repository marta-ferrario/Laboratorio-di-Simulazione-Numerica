#include <fstream>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>
#include <mpi.h>

#include "Individual.h"
#include "City.h"
#include "Population.h"
#include "GeneticAlgorithm.h"


using namespace std; 

vector<City> ReadCities(const string& filename); 


int main (int argc, char* argv[]){
    MPI_Init(&argc, &argv);

    bool use_migration = true; // variabile boolena per atiivare/disattivare le migrazioni

    int rank; // ID del continente
    int size; // numero totale di continenti
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

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

    if (input.is_open()) {
        string property;

        while (input >> property) {
            if (property == "RANDOMSEED") {
                input >> seed[0] >> seed[1] >> seed[2] >> seed[3];

                seed[0] += rank;   // differenzia i continenti

                rnd.SetRandom(seed, p1, p2);
            }
        }

        input.close();
    }

    cout << "Sono il processo: " << rank << endl; 
    vector<City> cities = ReadCities("../Input/cap_prov_ita.dat");

    int n_cities = cities.size(); 
    int pop_size = 1000; 
    int n_gen=2000;
    int N_migr = 50; 

    GeneticAlgorithm GA(pop_size, n_cities, cities, rnd);

    ofstream out;
    if(use_migration){
        out.open("../Output/With_migration/best_rank_" +
                to_string(rank) + ".dat");
    } else{
        out.open("../Output/No_migration/best_rank_" +
                to_string(rank) + ".dat");
    }
    
    for(int gen=0; gen<n_gen; gen++) {
        GA.Evolve(rnd);
        out << gen << " " << GA.GetBest().GetFitness() << endl;

        if (use_migration && gen > 0 && gen % N_migr == 0) {
            int pair[2];


            // Solo rank 0 sceglie la coppia che scambia gli individui
            if (rank == 0) {
                do {
                    pair[0] = (int)rnd.Rannyu(0, size);
                    pair[1] = (int)rnd.Rannyu(0, size);
                } while (pair[0] == pair[1]);
            }

        
            MPI_Bcast(pair, 2, MPI_INT, 0, MPI_COMM_WORLD);
            int a = pair[0], b = pair[1];
            if (rank == a || rank == b) {
                Individual best = GA.GetBest();
                vector<int> send_path = best.GetPath();
                vector<int> recv_path(n_cities);

                if (rank == a) {
                    MPI_Sendrecv(
                        send_path.data(), n_cities, MPI_INT, b, 1,
                        recv_path.data(), n_cities, MPI_INT, b, 2,
                        MPI_COMM_WORLD, MPI_STATUS_IGNORE
                    );
                } else { // rank == b
                    MPI_Sendrecv(
                        send_path.data(), n_cities, MPI_INT, a, 2,
                        recv_path.data(), n_cities, MPI_INT, a, 1,
                        MPI_COMM_WORLD, MPI_STATUS_IGNORE
                    );
                }

                Individual migrant(recv_path, cities);
                GA.ReplaceWorst(migrant);
            }


 
        }
    }


    ofstream path;
    if(use_migration){
        path.open("../Output/With_migration/best_path_rank_" +
                to_string(rank) + ".dat");
    } else{
        path.open("../Output/No_migration/best_path_rank_" +
                to_string(rank) + ".dat");
    }
    Individual best = GA.GetBest();
    for(int i=0;i<n_cities;i++){
        int city = best.GetPath()[i];
        path << cities[city].GetX() << " " << cities[city].GetY() << endl; // stampa delle coordinate delle città del miglior percorso
    
    }

    // if(rank == 0){
    //     Individual best = GA.GetBest();
    //     ofstream out_sing("best_global.dat");

    //     for(int i : best.GetPath()){
    //         out_sing << cities[i].GetX() << " " << cities[i].GetY() << endl;
    //     }
    // }


    MPI_Finalize();
    return 0; 


}


vector<City> ReadCities(const string& filename) {
    ifstream file(filename);
    if (!file) {
        cerr << "Errore apertura file: " << filename << endl;
        exit(1);
    }

    vector<City> cities;
    double x, y;

    while (file >> x >> y) {
        cities.emplace_back(x, y);
    }

    return cities;
}