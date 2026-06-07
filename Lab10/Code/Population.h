#ifndef __Population__
#define __Population__

#include <vector>
#include "City.h"
#include "Individual.h"
#include "../../Generatore_random/random.h"
using namespace std; 

class Population {
    private: 
    vector<Individual> individuals; 

    public: 
    //costruttore:
    Population(); 
    Population(int pop_size, int n_cities, Random& rnd); 
    
    void ComputeFitnesses(const vector<City>& cities, int pop_size); //compute fitness di ogni individuo
    void Sort(); 
    Individual& GetBest(); 
    Individual Selection(Random& rnd); 
    int Size() const; 
    void AddIndividual(const Individual& ind); 
    Individual& operator[](int i);
    Individual GetBest() const; // presuppone che sia già avvenuto l'ordinamento
    const Individual& GetIndividual(int i) const; 
    void SetIndividual(int i, const Individual& ind);

}; 


#endif