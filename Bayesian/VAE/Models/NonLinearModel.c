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
	model->decoder_fc2 = InitLinear(b,c); // 400, 784
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
	double y = 0.0;

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

	y = (exp(-logsigma))*-0.5;
	matrixSubtract(*flattened, *mu, res);
	matrix_pow(res,2);
	y = matrix_sum(res)*y;
	
	double cost = (392 * logsigma) - (y);

	/* Compute the logsumexp of y and take the mean
	 * then subtract "cost" with that mean.*/

	freeMatrix(mu);
	freeMatrix(res);
	freeMatrix(flattened);

	return cost;
}

double derivative_cost_function(Matrix *X, NLM *model)
{
	Matrix *res = allocateMatrix(
				model->decoder_fc2->output->rows, 
				model->decoder_fc2->output->columns);
	Matrix *flattened = allocateMatrix(
				1, 
				model->decoder_fc2->output->columns);
	Matrix *d_fz_z = allocateMatrix(
				model->decoder_fc2->output->rows, 
				model->decoder_fc2->output->columns);

	Matrix *d_z_w = allocateMatrix(
				model->decoder_fc2->output->rows, 
				model->decoder_fc2->output->columns);
	Matrix *dJ_fz = allocateMatrix(
				1, 
				model->decoder_fc2->output->columns);


	copyMatrix(*model->decoder_fc2->output, d_fz_z);
	copyMatrix(*model->decoder_fc2->output, d_z_w);
	
	double logsigma = model->logsigma2->data[0][0];
	flatten(X, flattened);

	
	int i;
	Matrix *x = allocateMatrix(1,1);
	double MSE = 0.0;
	for(i = 0; i < (flattened->columns); i++)
	{
		x->data[0][0] = flattened->data[0][i];
		matrixSubtract(*model->decoder_fc2->output, *x, res);
		const_mult_matrix(res,(1.0/logsigma));
		dJ_fz->data[0][i] = matrix_sum(res) * (-1.0);

	}

	//matrixSubtract(*model->decoder_fc2->output, *flattened, res);
	//const_mult_matrix(res,(1.0/logsigma));
	//double MSE = matrix_sum(res) * (-1.0);

	d_reLu_matrix(d_fz_z);
	reLu_matrix(d_z_w);
	
	//double g_x = (exp(-logsigma)) * -0.5 * MSE;
	//double g_prim_x = (exp(-logsigma)) * MSE;

	//double result = (exp(g_x)*g_prim_x)/exp(g_x);

	freeMatrix(res);
	freeMatrix(flattened);

	return 1.0;
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
