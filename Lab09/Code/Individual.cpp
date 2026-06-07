#include "Individual.h"
#include <algorithm>
#include <cmath>
#include <iostream>

//////////////////////////////////////////////////////////
// COSTRUTTORI
//////////////////////////////////////////////////////////
Individual::Individual(){
    fitness = 1e9; 
}

Individual::Individual(int n_cities, Random& rnd){
    for (int i=0; i<n_cities; i++){
        path.push_back(i); 
    }
    for(int i=0; i<n_cities; i++){
        SwapMutation(rnd); 
    }

}

//////////////////////////////////////////////////////////
// GETTER E UTILITIES
//////////////////////////////////////////////////////////
const std::vector<int>& Individual::GetPath() const {
    return path;
}

double Individual::GetFitness() const {
    return fitness;
}

int Individual::Size() const {
    return path.size();
}

void Individual::SetPath(const std::vector<int>& p) {
    path = p;
}

void Individual::ComputeFitness(const vector<City>& cities){
    fitness=0.0; 

    for(int i=0; i<path.size()-1; i++){
        int a = path[i]; 
        int b = path[i+1]; 

        double dx = cities[a].GetX() - cities[b].GetX(); 
        double dy = cities[a].GetY() - cities[b].GetY(); 

        fitness += sqrt(dx*dx+dy*dy); 
    }


    // aggiungo il tratto per tornare alla città iniziale
    int last = path.back(); 
    int first = path.front(); 
    double dx = cities[last].GetX() - cities[first].GetX(); 
    double dy = cities[last].GetY() - cities[first].GetY(); 
    fitness += sqrt(dx*dx+dy*dy); 
}

bool Individual::Check() const {
    if(path.empty()) return false; 
    if (path[0] != 0) return false; 

    vector<bool> visited(path.size(), false); 

    for(int i=0; i<path.size(); i++){
        int c = path[i]; 
        if(c<0 || c>=path.size()) return false; 
        if(visited[c]) return false; 
        visited[c] = true; 
    }
    return true; 
}


void Individual::Print() const{
    cout << "[ "; 
    for (int i=0; i<path.size(); i++){
        cout << path[i] << " ";
    }
    cout << "]" << endl; 
}

void Individual::SetNumberCities(int cities){
    n_cities = cities; 
}

//////////////////////////////////////////////////////////
// MUTAZIONI
//////////////////////////////////////////////////////////

void Individual::SwapMutation(Random& rnd){

    int n = path.size();

    int i = int(rnd.Rannyu(1, n));
    int j = int(rnd.Rannyu(1, n));

    std::swap(path[i], path[j]);
}

void Individual::ShiftMutation(Random& rnd) {

    int n = path.size();
    int start = 1 + int(rnd.Rannyu() * (n - 2));
    int len = 1 + int(rnd.Rannyu() * (n - start));
    int shift = 1 + int(rnd.Rannyu() * (n - 1 - len));

    vector<int> block; // copia del blocco
    for(int i = 0; i < len; i++) {
        block.push_back(path[start + i]);
    }

    path.erase(path.begin() + start, path.begin() + start + len); // rimozione blocco

    int new_pos = (start + shift) % (path.size()); // nuova posizione
    if(new_pos == 0)
        new_pos = 1;

    path.insert(path.begin() + new_pos, block.begin(), block.end()); // reinserimento blocco
}

void Individual::BlockMutation(Random& rnd){

    int N = path.size();

    int m = 1 + int(rnd.Rannyu() * ((N-1)/2));

    int start1 = 1 + int(rnd.Rannyu() * (N - 2*m));

    int start2 = start1 + m;
    int max_shift = N - start2 - m;

    if(max_shift > 0){
        start2 += int(rnd.Rannyu() * (max_shift + 1));
    }

    for(int i=0; i<m; i++){
        std::swap(path[start1+i], path[start2+i]);
    }
}
    
void Individual::InversionMutation(Random& rnd) {
    int n = path.size();
    int i = 1 + rnd.Rannyu() * (n - 2);
    int j = 1 + rnd.Rannyu() * (n - 2);

    if (i > j) swap(i, j);

    reverse(path.begin() + i, path.begin() + j + 1);
}
