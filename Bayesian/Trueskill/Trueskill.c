#include "stdio.h"
#include "Message.h"

void main( void )
{
    struct Message team1 = { .mu = 1, .var = 1};
    struct Message team2 = { .mu = 1, .var = 1};

    float y_obs[5] = {0.0, 1.0, 0.0, 0.0, 1.0};

    float start_var = 5;

    uint8_t i;
    for (i = 0; i < 5; i++)
    {
        start_var = update_skills_MP(team1, team2, start_var, y_obs[i]);
    }

    printf("%f", team1.mu);
    printf("%f",team2.mu);

}