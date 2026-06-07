#include "Population.h"
#include <algorithm>
#include <cmath>
#include <iostream>

Population::Population(){}

Population::Population(int pop_size, int n_cities, Random& rnd){
    for(int i=0; i<pop_size; i++){
        individuals.push_back(Individual(n_cities, rnd)); 
    }
}


void Population::ComputeFitnesses(const vector<City>& cities, int pop_size){
    for(int i=0; i<pop_size; i++){
        individuals[i].ComputeFitness(cities); 
    }
}

void Population::Sort(){
    sort(individuals.begin(), individuals.end(), [](const Individual&a, const Individual&b) {
        return a.GetFitness() < b.GetFitness();
    }); 
}

Individual Population::Selection(Random& rnd){
    int M = individuals.size(); 
    double p = 2.0; 
    int j = int(M*pow(rnd.Rannyu(), p)); 

    return individuals[j]; 
}


int Population::Size() const{
    return individuals.size(); 
}

void Population::AddIndividual(const Individual& ind){
    individuals.push_back(ind); 
}

Individual Population::GetBest() const {
    return individuals[0]; 
}


const Individual& Population::GetIndividual(int i) const {
    return individuals[i];
}