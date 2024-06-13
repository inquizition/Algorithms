#ifndef M_LINEARMODEL_H
#define M_LINEARMODEL_H

#include "Matrix.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

typedef struct
{
	bool output_init;
	Matrix *A;
	Matrix *b;
	Matrix *output;
}LM;

void freeLinear(LM *linear);
LM *InitLinear(int a, int b);
void Linear(LM *m, Matrix *input);
void print_linear(LM *m);

#endif M_LINEARMODEL_H
