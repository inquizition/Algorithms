#include "Matrix.h"
#include "LinearModel.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define TEST

LM *InitLinear(int a, int b)
{
	LM *model_ptr;
	model_ptr = (LM*) malloc(sizeof(LM));
	model_ptr->A = allocateMatrix(a, b);
	model_ptr->b = allocateMatrix(b,1);
#ifdef TEST
	ones(model_ptr->A);
	ones(model_ptr->b);
	const_mult_matrix(model_ptr->A, 0.1);
	const_mult_matrix(model_ptr->b, 0.1);
#else
	InitRandomMatrix(model_ptr->A);
	InitRandomMatrix(model_ptr->b);
#endif
	model_ptr->output_init = false;

	return model_ptr; 
}

void Linear(LM *m, Matrix *input)
{	
	//printf("Attempting Linear transformation.\n");
	if(m->output_init)
	{
		free(m->output);
	}
	m->output = allocateMatrix(input->rows, m->A->columns);
	m->output_init = true;	
	Matrix *res_temp = allocateMatrix(m->output->rows, m->output->columns);

	matMult(*input,*m->A,res_temp);
	transpose(&m->b);
	matrixAdd(*res_temp, *m->b, m->output);
	transpose(&m->b);
	freeMatrix(res_temp);
}

/* Prints linear model */
void print_linear(LM *m)
{
	printf("A: ");
	print_dim(*m->A);
	printf("b: ");
	print_dim(*m->b);
	if(m->output_init)
	{
		printf("output: ");
		print_dim(*m->output);
	}
}

void backward(LM* linear, Matrix *dz)
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


