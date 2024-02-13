#include "stdint.h"
#include "stdio.h"

#define SIZE 5000;
//void VAE()

int main()
{
	FILE *file;
	int img[SIZE];
	
	file = fopen("data/img.bin", "rb");
	fread(img, sizeof(int), SIZE, file);
	fclose(file);
	
	int i;
	for(i = 0; i < SIZE; i++)
	{
	   printf("d%\n", img[i]);
	}
	
	return 1;
}

VAE *InitVAE( void )
{
	VAE *model;
	model = (VAE*) malloc(sizeof(VAE));
	model->encoder_fc1 = InitLinear(784, 400);
	model->encoder_mean_fc2 = InitLinear(400, 2);
	model->encoder_logsigma_2_fc2 = InitLinear(400, 2);

	model->decoder_fc1 = InitLinear(2,400); // 2, 400
	model->decoder_fc2 = InitLinear(400,784); // 400, 784

	model->logsigma2 = allocateMatrix(1,1);

	return model;
}

void decode(NLM model, Matrix *z)
{
	Linear(model.decoder_fc1, z);
	Matrix *output_relu = allocateMatrix(
			model.decoder_fc1->output->rows,
			model.decoder_fc1->output->columns);
	copyMatrix(*model.decoder_fc1->output, output_relu);
	reLu_matrix(output_relu);
	Linear(model.decoder_fc2, model.decoder_fc1->output);
}

void encode(NLM model, Matrix *x)
{
	Linear(model.encoder_fc1, x);
	Matrix *output_relu = allocateMatrix(
			model.encoder_fc1->output->rows,
			model.encoder_fc1->output->columns);
	copyMatrix(*model.encoder_fc1->output, output_relu);
	reLu_matrix(output_relu);
	Linear(model.encoder_mean_fc1, output_relu);
	Linear(model.encoder_logsigma_2, output_relu);
}

static double cost_function(Matrix *x, VAE *model)
{
	encode(*model, x);
	// Updates z_mu and z_logsigma2
	Matrix *z_mu = allocateMatrix(
			model->encoder_mean_fc2->output->rows,
			model->encoder_mean_fc2->output->columns);
	Matrix *z_logsigma2 = allocateMatrix(
			model->encoder_logsigma_2->output->rows,
			model->encoder_logsigma_2->output->columns);
	copyMatrix(*model->encoder_mean_fc2->output, z_mu);
	copyMatrix(*model->encoder_logsigma_2_fc2, z_logsigma2);

	kl = kl_divergence(, z_logsigma2);
	z = z_mu + randn(n_z) * exp(0.5 * z_logsigma2);

	decode(z);
	// Updates x_decoded
	
	neg_log_evidence = 784 * logsigma_2 + mean(sum(pow(x_decoded - x,2) * exp(-logsigma_2)));

	return ned_log_evidence + kl;
}

static double kl_divergence_term(double *z_mu, double *z_logsigma_2)
{
	double sigma2 = exp(z_logsigma_2);

	double term = mean(sum(sigma2) + sum(pow(z_mu,2)) - sum(z_logsigma_2));
	return term;
}

static void encode(double *x)
{
	double h1 = relu(model)
}

static void decode(double *z);
{

}
