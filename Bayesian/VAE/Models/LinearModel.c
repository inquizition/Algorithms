#include "LinearModel.h"

#define TEST

LM *InitLinear(int a, int b)
{
	LM *model_ptr;
	model_ptr = (LM*) malloc(sizeof(LM));
	model_ptr->A = allocateMatrix(b, a);
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
	
        assert(m->A->columns == input->columns);
	if(m->output_init)
	{
		free(m->output);
	}
	m->output = allocateMatrix(input->rows, m->A->rows);
	m->output_init = true;	
	Matrix *res_temp = allocateMatrix(m->output->rows, m->output->columns);

	transpose(&m->A);
	transpose(&m->b);
	dot(*input,*m->A, (union Result*)res_temp);
	matrixAdd(*res_temp, *m->b, m->output);
	transpose(&m->A);
	transpose(&m->b);
	freeMatrix(res_temp);
}

void derivate_linear(LM *m, Matrix *error)
{
	// x = points[i, 0]
	// y = points[i, 1]
	// b_gradient += -(2/N)*(y - ((m_current*x)+b_current))
	// m_gradient += -(2/M)* x * (y - ((m_current * x) + b_current))
	// update grad*learningrate
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


