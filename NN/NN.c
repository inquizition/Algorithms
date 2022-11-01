#include "NN.h"
#include "math.h"

float relu(float z) {
    if(z > 0) {
        return z;
    } else {
        return 0;
    }
}

float d_relu(float z) {
    if(z > 0) {
        return 1;
    } else {
        return 0;
    }
}

struct Linear linear( int in_features, int out_features)
{
    struct Linear *linear_model;
    int L = sizeof(in_features);
    int M = sizeof(out_features);
    linear_model->weight    =   (float*)malloc(L*M);
    linear_model->bias      =   (float*)malloc(sizeof(L));
    int i;
    for(i = 0; i < L*M; i++)
    {
        if( i < M)
        {
            linear_model->bias[i] = randn();
        }
        linear_model->weight[i] = randn();
    }
    
    return linear_model;
}