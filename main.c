#include "Matrix.h"
#include "LinearModel.h"
#include "stdlib.h"
#include <stdio.h>
#include <stdbool.h>


int main(void)
{
	printf("Start application.\n");

	LM *model_ptr = (LM*) malloc(sizeof(LM));
	model_ptr->A = allocateMatrix(4,4);
	InitRandomMatrix(model_ptr->A);
	model_ptr->b = allocateMatrix(4,1);
	InitRandomMatrix(model_ptr->b);
	model_ptr->output_init = false;
	
//	LM *linear = InitLinear(4,4);
//
//	print_matrix(*linear->A);
//	printf("blub");
//	print_matrix(*linear->b);
//	printf("hellu");	
	Matrix *input = allocateMatrix(1,4);
	double data[1][4] = {{1,2,3,4}};
//	printf("hej");
    	fillMatrix(input, *data);
//
//	//printf("initoutput: %d", linear->output_init);
	Linear(model_ptr, input);
//
//	freeLinear(linear);
	freeMatrix(input);
	freeMatrix(model_ptr->A);
	freeMatrix(model_ptr->b);
	free(model_ptr);

	printf("Application end.");
	return 1;
}
