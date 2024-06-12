#include "Matrix.h"
#include "LinearModel.h"
//#include "NonLinearModel.h"
#include "model.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>

// Run by typing: cd build
// cmake ..
// cmake --build .

#define SIZE 5000

void registerExitHandler(void)
{
    atexit(freeAllMatrices);
}

int main(void)
{
	registerExitHandler();
	srand ( time(NULL) );
	FILE *file;
	int img[SIZE];
	double norm_img[28*28];

	file = fopen("data/img.bin", "rb");
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

	Matrix *mat_img = allocateMatrix(784,1);
	fillMatrix(mat_img, norm_img);
	//print_matrix(*mat_img);

	//NLM *non_linear_model = InitNLM(2, 400, 784);
	//NLM *non_linear_model = InitNLM_Model();
	PTNLM *non_linear_model = InitTrainedNLM_Model();
	//double cost = cost_function(mat_img, non_linear_model, 1);
	//printf("Cost: %f\n", cost);

	//double res = derivative_cost_function(mat_img, non_linear_model);
	encode(*non_linear_model, mat_img);

	//printf("Res: %f\n", res);
	print_matrix(*(non_linear_model->encoder_layer_2->output), "RES");
	freePTNonLinear(non_linear_model);
	freeMatrix(mat_img);

	return 0;
}
