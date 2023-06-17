#include "Matrix.h"
#include "LinearModel.h"
#include "NonLinearModel.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

//static NLM *model;

NLM *InitNLM(int a, int b, int c)
{
	NLM *model;
	model = (NLM*) malloc(sizeof(NLM));
	model->decoder_fc1 = InitLinear(a,b); // 2, 400
	model->decoder_fc2 = InitLinear(b, c); // 400, 784
	model->logsigma2 = allocateMatrix(1,1);
	return model;
}

void decode(NLM model, Matrix *z)
{
	Linear(model.decoder_fc1, z);
	// print linear model
	reLu_matrix(model.decoder_fc1->output);
	Linear(model.decoder_fc2, model.decoder_fc1->output);
}

void sample_decode(NLM *model, int k)
{
	Matrix *z = allocateMatrix(k, 2);
	ones(z);
	//InitRandomMatrix(z);
	decode(*model, z);
	freeMatrix(z);
}

double cost_function(Matrix *X, NLM *model, int K)
{
	// compute y with y[j, i] = -1/(2 * sigma_theta^2) * || x_j - mu_theta(z_i)) ||^2_2
	sample_decode(model, K);
	double logsigma = model->logsigma2->data[0][0];
	Matrix *y = allocateMatrix(
				model->logsigma2->rows, 
				model->logsigma2->columns);
	Matrix *mu = allocateMatrix(
				model->decoder_fc2->output->rows, 
				model->decoder_fc2->output->columns);
	Matrix *res = allocateMatrix(
				model->decoder_fc2->output->rows, 
				model->decoder_fc2->output->columns);
	Matrix *flattened = allocateMatrix(
				1, 
				model->decoder_fc2->output->columns);

	copyMatrix(*model->decoder_fc2->output, mu);
	flatten(X, flattened);

	logsigma = (exp(-logsigma))*-0.5;
	printf("logsigma: %f\n", logsigma);
	matrixSubtract(*flattened, *mu, res);
	matrix_pow(res,2);
	print_matrix(*res, "res");
	double cost = matrix_sum(res);
	
	printf("cost: %f\n", cost);
	cost = cost * logsigma;

	/* Compute the logsumexp of y and take the mean
	 * then subtract "cost" with that mean.*/

	freeMatrix(y);
	freeMatrix(mu);
	freeMatrix(res);
	freeMatrix(flattened);

	return cost;
}

/* Prints Non linear model */
void printNonLinear(NLM *nonLinear)
{
	printf("NonLinear Model:\n");
	printf("Decoder FC1:\n");
	print_linear(nonLinear->decoder_fc1);
	printf("Decoder FC2:\n");
	print_linear(nonLinear->decoder_fc2);
	printf("LogSigma2:\n");
	print_matrix(*nonLinear->logsigma2, "logsigma2");
}

void freeNonLinear(NLM *nonLinear)
{
	freeLinear(nonLinear->decoder_fc1);
	freeLinear(nonLinear->decoder_fc2);
	freeMatrix(nonLinear->logsigma2);
	free(nonLinear);
}
