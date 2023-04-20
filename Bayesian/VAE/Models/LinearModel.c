#include "Matrix.h"
#include "LinearModel.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

LM *InitLinear(int a, int b)
{
	LM *model_ptr;
	model_ptr = (LM*) malloc(sizeof(LM));
	printf("before matrix alloc");	
	model_ptr->A = allocateMatrix(a, b);
	InitRandomMatrix(model_ptr->A);
	model_ptr->b = allocateMatrix(b,1);
	InitRandomMatrix(model_ptr->b);
	model_ptr->output_init = false;
	printf("model output init: %d", model_ptr->output_init);

	return model_ptr; 
}

void Linear(LM *m, Matrix *input)
{
//	printf("1");
//	Matrix *ones_temp = allocateMatrix(1, input->columns);
//	printf("2");
//	Matrix *res_temp = allocateMatrix(m->output->rows, m->output->columns);
//	printf("3");
//	ones(ones_temp);
//	printf("4");
	if(m->output_init)
	{
		free(m->output);
	}
//	printf("5");
//	m->output = allocateMatrix(input->rows, m->A->columns);
//	printf("ones_temp: r=%d, c=%d", ones_temp->rows, ones_temp->columns);
//	printf("res_temp: r=%d, c=%d", res_temp->rows, res_temp->columns);
//	printf("ones_temp: r=%d, c=%d", m->output->rows, m->output->columns);
//	printf("ones_temp: r=%d, c=%d", input->rows, input->columns);
//
//	
//	dot(*input, *ones_temp, (union Result *)res_temp);
//	matMult(*res_temp, *input, m->output);
//	print_matrix(*m->output);
	m->output_init = true;	

//	freeMatrix(ones_temp);
//	freeMatrix(res_temp);
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


