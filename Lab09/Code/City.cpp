
#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include "City.h"

// Costruttore
City::City(double x_, double y_) : x(x_), y(y_) {}

// Metodi Get
double City::GetX() const {
    return x;
}

double City::GetY() const {
    return y;
}



