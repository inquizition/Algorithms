#include "Matrix.h"

typedef struct
{
	Matrix *A;
	Matrix *b;
}LM;

LM *InitLinear(int a, int b)
{
	LM *model_ptr;
	model_ptr = (LM*) malloc(sizeof(LM));
	
	model_ptr->A = allocateMatrix(a, b);
	InitRandomMatrix(model_ptr->A);
	model_ptr->b = allocateMatrix(b,1);
	InitRandomMatrix(model_ptr->b);	
}

void freeLinear(LM *linear)
{
	freeMatrix(linear->A);
	freeMatrix(linear->b);
	free(linear);
}


