#include <stdio.h>
#include <stdlib.h>
#include <sndfile.h>
#include <string.h>
#include <ctype.h>
#include <float.h>
#include <math.h>
#include <complex.h>
#include "plot_lib.h"

void DFT(double *x, double *abs_res, int size, int dft_size)
{
	double _Complex res[size];
	double real;
	double imag;

	int n;
	int k;
	for (k = 0; k < dft_size; k++) {
		real = 0.0;
		imag = 0.0;
		for (n = 0; n < size; n++) {
			real = real + x[n] * cos(2.0 * 3.1415 * (double)k *((double)n/(double)dft_size));	
			imag = imag - x[n] * sin(2.0 * 3.1415 * (double)k *((double)n/(double)dft_size));	
			res[k] = real + imag*_Complex_I;	 
		}	
		abs_res[k] = cabs(res[k]);
	}
}
static int count = 0;

void _fft(double _Complex buf[], double _Complex out[], int n, int step)
{
	count++;	
	printf("count: %d, step: %d, n: %d\n", count, step, n);
	if(step < n)
	{
		_fft(out, buf, n, step * 2);
		_fft(out + step, buf + step, n, step * 2);

		for (int i = 0; i < n; i += (2 * step)) {
			double _Complex t = cexp(-I * 3.1415 * i/n) * out[i + step];
			buf[i/2] = out[i] + t;
			buf[(i+n)/2] = out[i] - t;
		}
	}
	return;
}

void FFT(double *buf, double *res, int n)
{	
	double _Complex out[n];
	double _Complex complex_buf[n];

	int not_even = 0;
	if((n % 2) != 0)
	{
		n = n + 1;
		not_even = 1;
		complex_buf[n-1] = 0.0;
		out[n-1] = 0.0;
	}

	int i;
	for(i = 0; i < n - not_even; i++)
	{
		complex_buf[i] = buf[i];
		out[i] = complex_buf[i];
	}	
	
	//  if(N <= 32)
	//  {
	//  	DFT(buf, res, length, N);
	//  }
	//  else
	//  {
	//  	FFT(buf, res, n, step*2);
	//  	FFT(buf + step, res + step, n, step*2);
	//  }


	_fft(complex_buf, out, n, 1);
	//FFT_buffer(complex_buf, N, 1, out, scratch);
	for(i = 0; i < n - not_even; i++)
	{
		res[i] = cabs(out[i]);
	}
}

int main(int argc, char *argv[])
{
	char *filename;
	filename = "test.wav";
	SF_INFO sfinfo;
	SNDFILE *snd_file;
	snd_file = sf_open(filename, SFM_READ, &sfinfo);

	int length = sfinfo.frames;
	double *buf = malloc(length * sizeof(double));
	double dft_res[length];
	double x[length];

	int i;
	for (i = 0; i < length; i++) {
		x[i] = i*(1.0/sfinfo.samplerate);
	}

	sf_readf_double(snd_file, buf, length);
	
	plot_fig(x, buf, length);
	show_plot();	

	// DFT(buf, dft_res, length, 2048);
	//length = 2048;
	double fft_res[length];
	double x_2[length];
	for (i = 0; i < length; i++) {
		x_2[i] = i*(1.0/sfinfo.samplerate);
	}

	// plot_fig(x, dft_res, 2048);
	// show_plot();	
	printf("before fft");
	FFT(buf, fft_res, length);

	plot_fig(x_2, fft_res, length);
	show_plot();	

	free(buf);
	return 0;
}
 
