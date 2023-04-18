#include "Matrix.h"

typedef struct
{
	Matrix *A;
	Matrix *b;
}LM;

LM *InitLinear()
{
	LM *model_ptr;
	model_ptr = (LM*) malloc(sizeof(LM));
	
	model_ptr->A = allocateMatrix;	
}


