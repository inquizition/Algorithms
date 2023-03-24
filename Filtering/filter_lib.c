#include <float.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

void butterworth(double* amp, double co, double fs, int order)
{

}

double getNormalizedFrequency(double co, double fs)
{
	double nf;
	double ts;
	double wcd;
	//assert(ca < (sf/2));

	ts = 1.0/fs;
	wcd = (2*3.1415*co)/fs;
	nf = (2.0/ts)*tan(wcd/2.0);

	return nf;
}

void bilinear_transform()
{

}
