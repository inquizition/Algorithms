#include "Matrix.h"
#include "LinearModel.h"
#include "stdlib.h"
#include <stdio.h>
#include <stdbool.h>

// Run by typing: cd build
// cmake ../main.c
// cmake --build .

#define SIZE 5000

int main(void)
{
	FILE *file;
	int img[SIZE];
	
	file = fopen("Bayesian/VAE/data/img.bin", "rb");
	fread(img, sizeof(int), SIZE, file);
	fclose(file);
	
	int i;
	for(i = 0; i < SIZE; i++)
	{
	   printf("d%\n", img[i]);
	}	

//	printf("Start application.\n");

//	LM *linear = InitLinear(4,4);
//	Matrix *input = allocateMatrix(1,4);
//	double data[1][4] = {{-2, -1, 1, 2}};
//	//double data[1][1] = {{2}};
//    	fillMatrix(input, *data);
//	transpose(&input);
//
//	Linear(linear, input);
//	
//	print_matrix(*linear->output);
//
//	freeLinear(linear);
//	freeMatrix(input);
//
//	printf("Application end.\n");
//	return 1;
}
