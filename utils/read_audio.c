#include <stdio.h>
#include <sndfile.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <float.h>

#define	BLOCK_SIZE 4096

void plot_wav(SNDFILE *snd_file, double start, double end, double step)
{
	FILE *fp;
	float *buf;

	float channels = 2.0;
	
	sf_count_t frames;
	
	buf = malloc(BLOCK_SIZE * sizeof(float));

	frames = BLOCK_SIZE / channels;
	
	sf_readf_float(snd_file, buf, frames);

	fp = fopen("commands.gplot", "w");
	fprintf ( fp, "plot [ %f : %f ] \'fn.dat\' w lines, 0 \n", start - step, end + step  );
    	fprintf ( fp, "pause mouse \n" );
    	fclose ( fp );

	fp = fopen ( "fn.dat", "w" );    /* Open data file. */
	
	double x = 0.0;
	int i = 0;

    	for ( i = 0;  i <= BLOCK_SIZE ;  i++ )
    	{
        	x = start + i * step;
        	fprintf ( fp, "%25.15f  %25.15f\n", x, buf[i] );
    	}  
                  

    	fclose( fp );

    	system("gnuplot commands.gplot");


	sf_close(snd_file);
	free(buf);
}

int main(void)
{

	char *filename;
       	filename = "test.wav";
	SF_INFO sfinfo;
	SNDFILE *snd_file;
	snd_file = sf_open(filename, SFM_READ, &sfinfo);

	printf("samplerate: %d, frames: %d, channels: %d, format: %d", sfinfo.samplerate, sfinfo.frames, sfinfo.channels, sfinfo.format);
	plot_wav(snd_file, 0.0, 2048.0, 1);

	return 0;
}
