#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <float.h>
#include <math.h>
#include <complex.h>
#include "plot_lib.h"

void DFT(double *x, int size, int dftSize)
{
	double _Complex res[size];
	double real;
	double imag;

	int n;
	int k;
	for (k = 0; k < dftSize; k++) {
		real = 0.0;
		imag = 0.0;
		for (n = 0; n < size; n++) {
			real = real + x[n] * cos(2 * 3.1415 * k *(n/dftSize));	
			printf("sin: %f \n",sin(2 * 3.1415 * k *(n/dftSize)) );
			imag = imag - x[n] * sin(2 * 3.1415 * k *(n/dftSize));	
			res[k] = real + imag*_Complex_I;	 
		}	
		printf("(%f) + j(%f)\n", creal(res[k]), cimag(res[k]));
	}

}


int main(int argc, char *argv[])
{

	double input[10] = {1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0,10.0};	

	DFT(input, 10, 10);
	return 0;
}
 
