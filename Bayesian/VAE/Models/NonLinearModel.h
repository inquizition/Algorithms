#ifndef M_NONLINEARMODEL_H
#define M_NONLINEARMODEL_H

#include "Matrix.h"
#include "LinearModel.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
// Non linear model
typedef struct
{
	bool output_init;
	LM* decoder_fc1;
	LM* decoder_fc2;
	Matrix* logsigma2;
}NLM;

NLM *InitNLM(int a, int b, int c);

void freeNonLinear(NLM *nonLinear);
void decode(NLM model, Matrix *z);
void printNonLinear(NLM *nonLinear);
double cost_function(Matrix *X, NLM *model, int K);
double derivative_cost_function(Matrix *X, NLM *model);

#endif // M_NONLINEARMODEL_H
