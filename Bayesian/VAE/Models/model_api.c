#include "model_api.h"

#define SIZE 5000

static void registerExitHandler(void)
{
    atexit(freeAllMatrices);
}

void encode_data(double *data, double *encoded_data)
{
    registerExitHandler();

    PTNLM *non_linear_model = InitTrainedNLM_Model();
    Matrix *mat_img = 		allocateMatrix(1,INPUT_DIM);
    Matrix *z = 		allocateMatrix(1,LATENT_DIM);
    Matrix *logvar = 		allocateMatrix(1,LATENT_DIM);

    fillMatrix(mat_img, data);
    load_trained_weights(non_linear_model, "../Bayesian/VAE/Models/vae_weights.bin");
    encode(*non_linear_model, mat_img, z, logvar);

    dump_matrix(z, encoded_data);

    freeMatrix(mat_img);
    freeMatrix(z);
    freeMatrix(logvar);
    freeTrainedNonLinear(non_linear_model);
}

void decode_data(double *encoded_data, double *decoded_data)
{
    registerExitHandler();

    PTNLM *non_linear_model = InitTrainedNLM_Model();
    Matrix *z = 		allocateMatrix(1,LATENT_DIM);
    Matrix *x_hat = 		allocateMatrix(1,INPUT_DIM);

    fillMatrix(z, encoded_data);
    load_trained_weights(non_linear_model, "../Bayesian/VAE/Models/vae_weights.bin");
    decode(*non_linear_model, z, x_hat);

    dump_matrix(x_hat, decoded_data);

    freeMatrix(z);
    freeMatrix(x_hat);
    freeTrainedNonLinear(non_linear_model);
}

void run_print_model(char * weights_file)
{
    registerExitHandler();
    srand ( time(NULL) );
    
    PTNLM *non_linear_model = InitTrainedNLM_Model();
    load_trained_weights(non_linear_model, "Bayesian/VAE/Models/vae_weights.bin");
   
    freeTrainedNonLinear(non_linear_model);
}

void run_trained_model(double *image)
{
    registerExitHandler();
    srand ( time(NULL) );
    FILE *file;
    int img[SIZE];
    double norm_img[28*28];
    
    file = fopen("data/img.bin", "rb");
    fread(img, sizeof(int), SIZE, file);
    fclose(file);
    
    int i;
    int j;
    for(i = 0; i < 28; i++)
    {
    	for(j = 0; j < 28; j++)
    	{
       		norm_img[j + (i*28)] = img[j + (i*28)]/255.0;
    	}
    }	
    
    Matrix *mat_img = allocateMatrix(1,784);
    Matrix *z = allocateMatrix(1,2);
    Matrix *logvar = allocateMatrix(1,2);
    fillMatrix(mat_img, image);
    //print_matrix(*mat_img);
    
    PTNLM *non_linear_model = InitTrainedNLM_Model();
    load_trained_weights(non_linear_model, "Bayesian/VAE/Models/vae_weights.bin");

    for(i = 0; i < ENCODER_LAYERS + DECODER_LAYERS; i++)
    {
	print_layer(non_linear_model, i);
    }
    
    encode(*non_linear_model, mat_img, z, logvar);
    
    print_matrix(*(non_linear_model->encoder_layer_1->output), "mu:");
    print_matrix(*(non_linear_model->encoder_layer_2->output), "logvar");
    freeTrainedNonLinear(non_linear_model);
    freeMatrix(mat_img);
    freeMatrix(z);
    freeMatrix(logvar);
}
