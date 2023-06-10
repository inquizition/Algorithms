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
}

void decode(NLM model, Matrix *z)
{
	Linear(model.decoder_fc1, z);
	reLu_matrix(model.decoder_fc1->output);
	Linear(model.decoder_fc2, model.decoder_fc1->output);
}

void sample_decode(NLM *model, int k)
{
	Matrix *z = allocateMatrix(k, 2);
	InitRandomMatrix(z);
	decode(*model, z);
}

void cost_function(Matrix *X, NLM *model, int K)
{
	// compute y with y[j, i] = -1/(2 * sigma_theta^2) * || x_j - mu_theta(z_i)) ||^2_2
	sample_decode(model, K);
	Matrix *y = allocateMatrix(model->logsigma2->rows, model->logsigma2->columns);
	Matrix *logsigma1 = allocateMatrix(model->decoder_fc2->output->rows, model->decoder_fc2->output->columns);
	Matrix *logsigma2 = allocateMatrix(model->decoder_fc2->output->rows, model->decoder_fc2->output->columns);
	copyMatrix(*model->decoder_fc2->output, logsigma1);
	copyMatrix(*model->decoder_fc2->output, logsigma2);

	const_mult_matrix(logsigma1, -1.0);
	exp_matrix(logsigma1);
	const_mult_matrix(logsigma1, -0.5);

	//matrix_pow(logsigma2);
	double a = matrix_sum(logsigma2);

	freeMatrix(y);
	freeMatrix(logsigma1);
	freeMatrix(logsigma2);
}

void freeNonLinear(NLM *nonLinear)
{
	freeLinear(nonLinear->decoder_fc1);
	freeLinear(nonLinear->decoder_fc2);
	freeMatrix(nonLinear->logsigma2);
	free(nonLinear);
}
