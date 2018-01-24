#include "RNGClass.h"



RNGClass::RNGClass()
{
}


RNGClass::~RNGClass()
{
}
int RNGClass::GetRandomInteger(int i_LowerBound, int i_UpperBound)
{
    std::random_device m_RandomDevice;
    std::mt19937 MersenneTwister(m_RandomDevice());
    std::uniform_real_distribution<> Distrubtion(i_LowerBound, i_UpperBound + 1); //+1 added to  include upper bound in range
    int Number = Distrubtion(MersenneTwister);
   return Number;
}
