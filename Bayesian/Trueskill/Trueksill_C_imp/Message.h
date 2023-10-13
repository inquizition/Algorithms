#include "stdint.h"

struct Message
{
    float mu;
    float var;
    /* data */
};

float update_skills_MP(struct Message *t1,struct Message *t2, float var_t, float y_obs);