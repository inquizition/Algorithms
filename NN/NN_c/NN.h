#include "stdint.h"
#include "Matrix.h"

typedef struct Linear {
    int input_features;
    int output_features;
    struct Matrix *weight;
    struct Matrix *bias;
    struct Matrix *output;
} Linear;