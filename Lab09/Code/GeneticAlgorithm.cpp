#include "GeneticAlgorithm.h"
#include <algorithm>
#include <cmath>
#include <iostream>

GeneticAlgorithm::GeneticAlgorithm(int pop_size_, int n_cities_, const vector<City>& cities_, Random& rnd) : pop(pop_size_, n_cities_, rnd) {
    
    pop_size = pop_size_;
    n_cities = n_cities_;
    cities = cities_;

    p_cross = 0.8;
    p_mut = 0.1;

    pop.ComputeFitnesses(cities, pop_size);
    pop.Sort();
}


pair<Individual, Individual> GeneticAlgorithm::Crossover(const Individual& p1, const Individual& p2, Random& rnd){
    int N=n_cities; 
    vector<int> child1(N, -1);
    vector<int> child2(N, -1);
    child1[0] = 0;
    child2[0] = 0;

    int cut = 1 + int(rnd.Rannyu() * (N-2));

    // copia della prima parte
    for(int i=1; i<cut; i++) {
        child1[i] = p1.GetPath()[i];
        child2[i] = p2.GetPath()[i];
    }

    // completamento child1
    int idx1 = cut;
    for(int i=1; i<N; i++) {
        int city = p2.GetPath()[i];
        if(find(child1.begin(), child1.end(), city) == child1.end())
        {
            child1[idx1] = city;
            idx1++;
        }
    }

    // completamento child2
    int idx2 = cut;
    for(int i=1; i<N; i++) {
        int city = p1.GetPath()[i];
        if(find(child2.begin(), child2.end(), city) == child2.end())
        {
            child2[idx2] = city;
            idx2++;
        }
    }

    Individual son1;
    Individual son2;
    son1.SetPath(child1);
    son2.SetPath(child2);

    return make_pair(son1, son2);
}

void GeneticAlgorithm::Mutate(Individual& ind, Random& rnd) {
    if(rnd.Rannyu() < p_mut)
        ind.SwapMutation(rnd);

    if(rnd.Rannyu() < p_mut)
        ind.ShiftMutation(rnd);

    if(rnd.Rannyu() < p_mut)
        ind.BlockMutation(rnd);

    if(rnd.Rannyu() < p_mut)
        ind.InversionMutation(rnd);
}


void GeneticAlgorithm::Evolve(Random& rnd){
    Population new_pop; 
    while(new_pop.Size() < pop_size){
        Individual p1 = pop.Selection(rnd);
        Individual p2 = pop.Selection(rnd);

        pair<Individual,Individual> children;

        if(rnd.Rannyu() < p_cross) {
            children = Crossover(p1,p2,rnd);
        } else {
            children.first  = p1;
            children.second = p2;
        }

        Mutate(children.first, rnd);
        Mutate(children.second, rnd);

        children.first.ComputeFitness(cities);
        children.second.ComputeFitness(cities);

        new_pop.AddIndividual(children.first);

        if(new_pop.Size() < pop_size) {
            new_pop.AddIndividual(children.second);
        }
    }

    new_pop.Sort();
    pop = new_pop;
}

Individual GeneticAlgorithm::GetBest() const {
    return pop.GetBest();
}

void GeneticAlgorithm::PrintBest() const {
    cout << pop.GetBest().GetFitness() << endl;
}

Population& GeneticAlgorithm::GetPopulation() {
    return pop;
}


double GeneticAlgorithm::GetBestHalfAverage() const {

    int M = pop.Size();
    double sum = 0.0;

    for(int i = 0; i < M/2; i++) {
        sum += pop.GetIndividual(i).GetFitness();
    }
    return sum / (M/2);
}