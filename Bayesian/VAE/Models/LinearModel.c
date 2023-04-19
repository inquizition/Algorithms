#include "Matrix.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct
{
	Matrix *A;
	Matrix *b;
	bool output_init;
	Matrix *output;
}LM;

LM *InitLinear(int a, int b)
{
	LM *model_ptr;
	model_ptr = (LM*) malloc(sizeof(LM));
	
	model_ptr->A = allocateMatrix(a, b);
	InitRandomMatrix(model_ptr->A);
	model_ptr->b = allocateMatrix(b,1);
	InitRandomMatrix(model_ptr->b);
	model_ptr->output_init = false;

	return model_ptr; 
}

void forward(LM* linear)
{

}

void freeLinear(LM *linear)
{
	freeMatrix(linear->A);
	freeMatrix(linear->b);
	free(linear);
}


