#include <stdio.h>
#include <sndfile.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <float.h>

#define	BLOCK_SIZE 4096

void read_wav_stat(void)
{
	FILE *fp;
	float *buf;
	char *filename;
       	filename = "test.wav";

	float channels = 2.0;
	
	SNDFILE *snd_file;
	SF_INFO sfinfo;
	sf_count_t frames;
	
	buf = malloc(BLOCK_SIZE * sizeof(float));

	frames = BLOCK_SIZE / channels;
	snd_file = sf_open(filename, SFM_READ, &sfinfo);
	
	sf_readf_float(snd_file, buf, frames);
	printf("buf val: %f, %f, %f", buf[0],buf[1], buf[2]);


	double a = 0;
	double b = 300;
	double step = 1;	
	fp = fopen("commands.gplot", "w");
	fprintf ( fp, "plot [ %f : %f ] \'fn.dat\' w lines, 0 \n", a - step, b + step  );
    	fprintf ( fp, "pause mouse \n" );
    	fclose ( fp );

	fp = fopen ( "fn.dat", "w" );    /* Open data file. */
	
	double x = 0.0;
	int i = 0;

    	for ( i = 0;  i <= 300 ;  i++ )
    	{
        	x = x + i;
        	fprintf ( fp, "%25.15f  %25.15f\n", x, buf[i] );
    	}  
                  

    	fclose( fp );

    	system("gnuplot commands.gplot");


	sf_close(snd_file);
	free(buf);
}

int main(void)
{
		read_wav_stat();
	return 0;
}
