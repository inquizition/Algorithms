#include "stdint.h"
#include "Matrix.h"

typedef struct Linear {
    struct Matrix *weight;
    struct Matrix *bias;
} Linear;