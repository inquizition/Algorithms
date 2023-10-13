#include "stdio.h"
#include "Message.h"

void main( void )
{
    char buffer[10];
    struct Message team1 = { .mu = 1, .var = 1};
    struct Message team2 = { .mu = 1, .var = 1};

    float y_obs;

    float start_var; // 

    printf("Enter mean and variance of first team: ");
    scanf("%f %f", &team1.mu, &team1.var);
    printf("\n");
    printf("Enter mean and variance of second team: ");
    scanf("%f %f", &team2.mu, &team2.var);
    printf("\n");
    printf("Enter start variance of match: ");
    scanf("%f", &start_var);
    printf("\n");


    while(1)
    {
        printf("\nWho won?\n");
        scanf("%f", &y_obs);
        start_var = update_skills_MP(&team1, &team2, start_var, y_obs);
        printf("Mean team 1: %f, var: %f\n", team1.mu, team1.var);
        printf("Mean team 2: %f, var: %f\n",team2.mu, team2.var);
    }
}