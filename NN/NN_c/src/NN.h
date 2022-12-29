#include "stdint.h"
#include "Matrix.h"

typedef struct Linear {
    int input_features;
    int output_features;
    struct Matrix *weight;
    struct Matrix *bias;
    struct Matrix *output;
} Linear;

typedef struct layer {
    struct Linear layer;
    struct layer *next_layer;
} layers_t;


double MSE_loss(struct Matrix pred, struct Matrix y);