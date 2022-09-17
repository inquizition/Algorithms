#include "stdint.h"

struct Message
{
    uint32_t mu;
    uint32_t var;
    /* data */
};

float update_skills_MP(struct Message t1,struct Message t2, float var_t, float y_obs);