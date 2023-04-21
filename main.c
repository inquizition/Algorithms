#include "Matrix.h"
#include "LinearModel.h"
#include "stdlib.h"
#include <stdio.h>
#include <stdbool.h>

int main(void)
{
	printf("Start application.\n");

	LM *linear = InitLinear(4,4);
	Matrix *input = allocateMatrix(1,4);
	double data[1][4] = {{-2, -1, 1, 2}};
	//double data[1][1] = {{2}};
    	fillMatrix(input, *data);
	transpose(&input);

	Linear(linear, input);
	
	print_matrix(*linear->output);

	freeLinear(linear);
	freeMatrix(input);

	printf("Application end.\n");
	return 1;
}
