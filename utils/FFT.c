#include <stdio.h>
#include <stdlib.h>
#include <sndfile.h>
#include <string.h>
#include <ctype.h>
#include <float.h>
#include <math.h>
#include <complex.h>
#include "plot_lib.h"

void compute(double data_re[], double data_im[], const unsigned int N);
void rearrange(double data_re[], double data_im[], const unsigned int N);
void plot_fft(double buf[], int N, int sample_rate);
void fft(double data_re[], double data_im[], const unsigned int N);

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

void fft(double data_re[], double data_im[], const unsigned int N)
{
	rearrange(data_re, data_im, N);
	compute(data_re, data_im, N);
}

void plot_fft(double buf[], int N, int sample_rate)
{
	int length = N;
	if(N%2 != 0)
	{
		length = N + 1;
	}
	
	double buf_real[length];
	double buf_img[length];

	for (int i = 0; i < N; ++i) {
		buf_real[i] = buf[i];
		buf_img[i] = 0.0;
	}
	
	if(length != N)
	{
		buf_real[length-1] = 0.0;
		buf_img[length-1] = 0.0;
	}

	fft(buf_real, buf_img, length);

	double abs_res[(int)(length/2)];
	double frequency_axis[(int)(length/2)];

	for(int i = 0; i < (int)(length/2); ++i) {
		frequency_axis[i] = i*(1.0/sample_rate);
		abs_res[i] = sqrt(buf[i]*buf[i] + buf_img[i]*buf_img[i]);	
	}

	printf("length fft: %d", length);
	plot_fig(frequency_axis, abs_res, (int)(length/2));
	show_plot();
}

void rearrange(double data_re[], double data_im[], const unsigned int N)
{
	unsigned int target = 0;
	int i;
	for (i = 0; i < N; ++i) {
		if(target>i)
		{
			const double temp_re = data_re[target];	
			const double temp_im = data_im[target];	
			data_re[target] = data_re[i];
			data_im[target] = data_im[i];
			data_re[i] = temp_re;
			data_im[i] = temp_im;
		}
		unsigned int mask = N;
		while(target & (mask >>=1))
		{
			target &= ~mask;
		}
		target |= mask;
	}
}

void compute(double data_re[], double data_im[], const unsigned int N)
{
	const double pi = 3.1415926535898;

	for(unsigned int step=1; step<N; step <<=1)
	{
		const unsigned int jump = step << 1;
		const double step_d = (double) step;
		double twiddle_re = 1.0;
		double twiddle_im = 0.0;
		for(unsigned int group=0; group<step; group++)
		{
			for(unsigned int pair=group; pair<N; pair+=jump)
			{
				const unsigned int match = pair + step;
				const double product_re = twiddle_re*data_re[match]-twiddle_im*data_im[match];
				const double product_im = twiddle_im*data_re[match]+twiddle_re*data_im[match];
				data_re[match] = data_re[pair]-product_re;
				data_im[match] = data_im[pair]-product_im;
				data_re[pair] += product_re;
				data_im[pair] += product_im;
			}
			
			// we need the factors below for the next iteration
			// if we don't iterate then don't compute
			if(group+1 == step)
			{
			  continue;
			}
			
			double angle = pi*((double) group+1)/step_d;
			twiddle_re = cos(angle);
			twiddle_im = sin(angle);
		}
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

	DFT(buf, dft_res, length, 2048);
	//length = 2048;
	double fft_res[length];
	double x_2[length];
	double buf_short[length];
	for (i = 0; i < length; i++) {
		x_2[i] = i*(1.0/sfinfo.samplerate);
		buf_short[i] = buf[i];
	}
	
	plot_fig(x_2, dft_res, 2048);
	show_plot();	

	// plot_fig(x, dft_res, 2048);
	// show_plot();	
	printf("before fft");
	//FFT(buf, fft_res, length);
	plot_fft(buf_short, length, sfinfo.samplerate);

	free(buf);
	return 0;
}
 
