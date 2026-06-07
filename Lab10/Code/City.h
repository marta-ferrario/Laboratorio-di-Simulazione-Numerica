#ifndef __City__
#define __City__


class City{
    private: 
    double x,y; 

    public: 
    City(double x_=0, double y_=0); 
    double GetX() const; 
    double GetY() const; 
};

#endif