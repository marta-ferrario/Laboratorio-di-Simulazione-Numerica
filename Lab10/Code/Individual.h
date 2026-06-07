#ifndef __Individual__
#define __Individual__

#include <vector>
#include "City.h"
#include "../../Generatore_random/random.h"

using namespace std; 

class Individual{
    private:
        int n_cities; 
        vector<int> path; 
        double fitness; 

    public: 
    // costruttori
    Individual();
    Individual(const vector<int>& m_path, vector<City> cities);
    Individual(int n_cities, Random&); 

    // getter e utilities
    const vector<int>& GetPath() const; 
    double GetFitness() const; 
    void SetPath(const vector<int>& p);
    void ComputeFitness(const vector<City>& cities); 
    int Size() const; 
    bool Check() const;  
    void Print() const; 
    void SetNumberCities(int cities);

    // mutazioni
    void SwapMutation(Random& rnd);
    void ShiftMutation(Random& rnd);
    void BlockMutation(Random& rnd); 
    void InversionMutation(Random& rnd);

}; 

#endif