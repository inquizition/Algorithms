#include <stdio.h>
#include <sndfile.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <float.h>
#include "plot_lib.h"

#define	BLOCK_SIZE 4096

void plot_audio(char *filename)
{

	SF_INFO sfinfo;
	SNDFILE *snd_file;
	snd_file = sf_open(filename, SFM_READ, &sfinfo);
	
	int length = sfinfo.frames;
	double *buf = malloc(length * sizeof(double));
	double x[length];

	int i;
	for (i = 0; i < length; i++) {
		x[i] = i*(1.0/sfinfo.samplerate);
	}

	sf_readf_double(snd_file, buf, length);

	plot_fig(x, buf, length);
	show_plot();	
	free(buf);
}



int main(void)
{

	char *filename;
       	filename = "test.wav";

	plot_audio(filename);

	return 0;
}
