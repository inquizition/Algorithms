#ifndef MODEL_H
#define MODEL_H

#include "Matrix.h"
#include "LinearModel.h"
#include <stdbool.h>
#include <stdlib.h>

#define INPUT_DIM 784
#define ENCODER_HIDDEN_DIM 70
#define DECODER_HIDDEN_DIM 50
#define LATENT_DIM 2
#define ENCODER_LAYERS 3
#define DECODER_LAYERS 2


typedef struct
{
    bool output_init;
    LM* encoder_layer_0;
    LM* encoder_layer_1;
    LM* encoder_layer_2;
    LM* decoder_layer_0;
    LM* decoder_layer_1;
    Matrix* logsigma2;
}PTNLM;


typedef struct
{
    char *name;
    void (*print_func)(PTNLM *);
    double (*dump_weights_func)(PTNLM *, char matrix, int r, int c);
    double (*dump_outputs_func)(PTNLM *, int r, int c);
}layer_func_t;

void decode(PTNLM model, Matrix *z, Matrix *output);

void encode(PTNLM model, Matrix *x, Matrix *output_mu, Matrix *output_logvar);

void print_layer(PTNLM *model, int layer);
double dump_layer_weights(PTNLM *model, int layer, char matrix, int r, int c);
double dump_layer_outputs(PTNLM *model, int layer, int r, int c);
PTNLM *InitTrainedNLM_Model(void);

void load_trained_weights(PTNLM *model, char *filename);

void freeTrainedNonLinear(PTNLM *model);



#endif // MODEL_H
