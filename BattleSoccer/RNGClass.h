#pragma once
#include <random>

class RNGClass
{

public:
    RNGClass();
    ~RNGClass();

    //returns a random integer that can include any number in [i_LowerBound, i_UpperBound] including end points
    int static GetRandomInteger (int i_LowerBound, int i_UpperBound);

private:
    
};

