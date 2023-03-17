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
	int length =  end/step;
	printf("end: %f, step: %f, length: %d \n", end, step, length);
	
	sf_count_t frames;
	
	buf = malloc(length * sizeof(float));

	frames = length;
	
	printf("frames: %d", frames);
	sf_readf_float(snd_file, buf, frames);

	fp = fopen("commands.gplot", "w");
	fprintf ( fp, "plot [ %f : %f ] \'fn.dat\' w lines, 0 \n", start - step, end + step  );
    	fprintf ( fp, "pause mouse \n" );
    	fclose ( fp );

	fp = fopen ( "fn.dat", "w" );    /* Open data file. */
	
	double x = 0.0;
	int i = 0;

    	for ( i = 0;  i <= frames ;  i++ )
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

	printf("samplerate: %d, frames: %d, channels: %d, format: %d \n", sfinfo.samplerate, sfinfo.frames, sfinfo.channels, sfinfo.format);
	plot_wav(snd_file, 0.0, 0.6, 1/(double)sfinfo.samplerate);

	return 0;
}
