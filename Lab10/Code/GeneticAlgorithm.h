#ifndef __GeneticAlgorithm__
#define __GeneticAlgorithm__

#include <vector>
#include "City.h"
#include "Individual.h"
#include "Population.h"
#include "../../Generatore_random/random.h"
using namespace std; 

class GeneticAlgorithm{

    private:
    Population pop; 
    vector<City> cities; 
    int pop_size; 
    int n_cities; 
    double p_cross;
    double p_mut;

    public: 
    // costruttore
    GeneticAlgorithm(int pop_size_, int n_cities_, const vector<City>& cities_, Random& rnd);

    // evolve una generazione
    void Evolve(Random& rnd);

    // crossover
    pair<Individual, Individual> Crossover(const Individual& p1, const Individual& p2, Random& rnd);

    // mutazioni
    void Mutate(Individual& ind, Random& rnd);

    // best individual
    Individual GetBest() const;

    // stampa best fitness
    void PrintBest() const;

    // accesso popolazione
    Population& GetPopulation();

    double GetBestHalfAverage() const;

    // replace worst
    void ReplaceWorst(const Individual& migrant); 




};





#endif