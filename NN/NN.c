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

void linear( struct Linear linear_model, struct Matrix x, struct Matrix *y)
{

    //InitRandomMatrix(&linear_model->weight);
    //InitRandomMatrix(&linear_model->bias);
    transpose(&linear_model.weight);
    matMult(x, *linear_model.weight, y);
    

    
    return linear_model;
}