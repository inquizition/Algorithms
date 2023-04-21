#include "Matrix.h"
#include "LinearModel.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define TEST 1

LM *InitLinear(int a, int b)
{
	LM *model_ptr;
	model_ptr = (LM*) malloc(sizeof(LM));
	model_ptr->A = allocateMatrix(a, b);
	model_ptr->b = allocateMatrix(b,1);
#ifdef TEST
	printf("InitLinear \n");
	print_dim(*model_ptr->A);
	print_dim(*model_ptr->b);
	ones(model_ptr->A);
	ones(model_ptr->b);
#else
	InitRandomMatrix(model_ptr->A);
	InitRandomMatrix(model_ptr->b);
#endif
	model_ptr->output_init = false;

	return model_ptr; 
}

void Linear(LM *m, Matrix *input)
{	
	printf("Linear \n");
	if(m->output_init)
	{
		free(m->output);
	}
	m->output = allocateMatrix(input->rows, m->A->columns);
	Matrix *ones_temp = allocateMatrix(1, m->A->rows);
	Matrix *res_temp = allocateMatrix(input->rows, m->A->columns);
	ones(ones_temp);

	print_dim(*ones_temp);
	print_dim(*res_temp);
	print_dim(*m->output);
	dot(*input, *ones_temp, (union Result *)res_temp);
	matMult(*res_temp, *m->A, m->output);
	m->output_init = true;	

	freeMatrix(ones_temp);
	freeMatrix(res_temp);
}

void forward(LM* linear)
{

}

void freeLinear(LM *linear)
{
	freeMatrix(linear->A);
	freeMatrix(linear->b);
	if(linear->output_init)
	{
		freeMatrix(linear->output);
	}
	free(linear);
}


