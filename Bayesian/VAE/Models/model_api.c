#include "model_api.h"

#define SIZE 5000

static PTNLM *non_linear_model;
static bool initiated = false;

static void registerExitHandler(void)
{
    atexit(freeAllMatrices);
}

void Init_preTrained_VAE_model(char * weights_file_path)
{
    if(initiated)
    {
        return;
    }
    non_linear_model = InitTrainedNLM_Model();
    load_trained_weights(non_linear_model, "../Bayesian/VAE/Models/vae_weights.bin");
    load_trained_weights(non_linear_model, weights_file_path);
    initiated = true;
}

void close_preTrained_VAE_model(void)
{
    if(!initiated)
    {
        return;
    }
    freeTrainedNonLinear(non_linear_model);
    initiated = false;
}

void encode_data(double *data, double *encoded_data, double *logvar_data)
{
    if(!initiated)
    {
        printf("Model is not initiated!");
        exit(1);
    }
    registerExitHandler();

    Matrix *mat_img = 		allocateMatrix(1,INPUT_DIM);
    Matrix *z = 		allocateMatrix(1,LATENT_DIM);
    Matrix *logvar = 		allocateMatrix(1,LATENT_DIM);

    fillMatrix(mat_img, data);
    encode(*non_linear_model, mat_img, z, logvar);

    dump_matrix(z, encoded_data);
    dump_matrix(logvar, logvar_data);

    freeMatrix(mat_img);
    freeMatrix(z);
    freeMatrix(logvar);
}

void decode_data(double *encoded_data, double *decoded_data)
{
    if(!initiated)
    {
        printf("Model is not initiated!");
        exit(1);
    }
    registerExitHandler();

    Matrix *z = 		allocateMatrix(1,LATENT_DIM);
    Matrix *x_hat = 		allocateMatrix(1,INPUT_DIM);

    fillMatrix(z, encoded_data);
    load_trained_weights(non_linear_model, "../Bayesian/VAE/Models/vae_weights.bin");
    decode(*non_linear_model, z, x_hat);

    dump_matrix(x_hat, decoded_data);

    freeMatrix(z);
    freeMatrix(x_hat);
}

void get_model_weights(int layer, char ab, double *weights, int rows, int columns)
{
    int i = 0;
    for(int r; r < rows; r++)
    {
	for(int c; c < columns; c++)
	{
            weights[i] = dump_layer_weights(non_linear_model, layer, ab, r, c);
	    i++;
	}
    }
}

void run_print_model(char * weights_file)
{
    if(!initiated)
    {
        printf("Model is not initiated!");
        exit(1);
    }
    registerExitHandler();
    srand ( time(NULL) );
    
    load_trained_weights(non_linear_model, "Bayesian/VAE/Models/vae_weights.bin");
   
    freeTrainedNonLinear(non_linear_model);
}
