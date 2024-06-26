#include "model_api.h"

#define SIZE 5000

static void registerExitHandler(void)
{
    atexit(freeAllMatrices);
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
    fillMatrix(mat_img, image);
    //print_matrix(*mat_img);
    
    PTNLM *non_linear_model = InitTrainedNLM_Model();
    load_trained_weights(non_linear_model, "Bayesian/VAE/Models/vae_weights.bin");

    for(i = 0; i < ENCODER_LAYERS + DECODER_LAYERS; i++)
    {
	print_layer(non_linear_model, i);
    }
    
    encode(*non_linear_model, mat_img);
    
    print_matrix(*(non_linear_model->encoder_layer_1->output), "mu:");
    print_matrix(*(non_linear_model->encoder_layer_2->output), "logvar");
    freeTrainedNonLinear(non_linear_model);
    freeMatrix(mat_img);
}
