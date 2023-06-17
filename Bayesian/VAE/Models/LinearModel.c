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
	Matrix *ones_temp = allocateMatrix(input->columns, m->A->columns);
	Matrix *res_temp = allocateMatrix(input->rows, m->A->columns);
	Matrix *A_T = allocateMatrix(m->A->rows, m->A->columns);

	copyMatrix(*m->A, A_T);
	//transpose(&A_T);
	ones(ones_temp);
	
	//dot(*input, *ones_temp, (union Result *)res_temp);
	//matMult(*res_temp, *A_T, m->output);
	matMult(*input,*A_T,m->output);
	freeMatrix(ones_temp);
	freeMatrix(res_temp);
	freeMatrix(A_T);
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


