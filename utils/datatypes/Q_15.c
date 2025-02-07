#include "Q_15.h"

int16_t Q15(float num) 
{
    return num*pow(2,15);
}

float toQ15(int16_t num, int mult) 
{
    return ((num-((pow(2,16))/2))/pow(2,15))*mult;
}
