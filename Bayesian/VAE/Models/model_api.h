#include "Matrix.h"
#include "model.h"
#include "LinearModel.h"
#include <stdlib.h>
#include <time.h>

void Init_preTrained_VAE_model(char * weights_file_path);
void close_preTrained_VAE_model(void);
void encode_data(double *data, double *encoded_data, double *logvar_data);
void decode_data(double *encoded_data, double *decoded_data);
void get_model_weights(int layer, char ab, double *weights, int rows, int columns);
