#include "Matrix.h"
#include "model.h"
#include "LinearModel.h"
#include <stdlib.h>
#include <time.h>

void run_trained_model(double *image);
void encode_data(double *data, double *encoded_data);
void decode_data(double *encoded_data, double *decoded_data);
