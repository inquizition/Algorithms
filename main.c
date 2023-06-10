#include "Matrix.h"
#include "LinearModel.h"
#include "NonLinearModel.h"
#include "stdlib.h"
#include <stdio.h>
#include <stdbool.h>

// Run by typing: cd build
// cmake ..
// cmake --build .

#define SIZE 5000

int main(void)
{
	FILE *file;
	int img[SIZE];
	double norm_img[28*28];

	file = fopen("../data/img.bin", "rb");
	fread(img, sizeof(int), SIZE, file);
	fclose(file);

	
	int i;
	int j;
	for(i = 0; i < 28; i++)
	{
		for(j = 0; j < 28; j++)
		{
	   		norm_img[j + (i*28)] = img[j + (i*28)]/255.0;
			//printf("%d  ", img[j + (i*28)]);
		}
		//printf("\n");
	}	

	Matrix *mat_img = allocateMatrix(28,28);
	fillMatrix(mat_img, norm_img);
	//print_matrix(*mat_img);

	NLM *non_linear_model = InitNLM(2, 400, 784);
	cost_function(norm_img,non_linear_model, 1);
	freeNonLinear(non_linear_model);
	freeMatrix(mat_img);

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
