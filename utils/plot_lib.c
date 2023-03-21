#include <float.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

static bool allocated = false;
static double x_start = 0.0;
static double x_end = 100.0;

static double y_start = -1.0;
static double y_end = 1.0;

static int length = 0;

static double *x_axis_ptr;
static double *y_axis_ptr;

void show_plot( void )
{
	FILE *fp;

	fp = fopen("commands.gplot", "w");
	fprintf ( fp, "plot [ %f : %f ] \'fn.dat\' w lines, 0 \n", x_axis_ptr[0], x_axis_ptr[length - 1]);
    	fprintf ( fp, "pause mouse \n" );
    	fclose ( fp );
	fp = fopen ( "fn.dat", "w" );    /* Open data file. */
	
	int i = 0;
    	for ( i = 0;  i <= length;  i++ )
    	{
        	fprintf ( fp, "%25.15f  %25.15f\n", x_axis_ptr[i], y_axis_ptr[i] );
    	}  
                  
    	fclose( fp );

	system("gnuplot commands.gplot");

	free(x_axis_ptr);
	free(y_axis_ptr);
	allocated = false;
}

void set_xlim( double x1, double x2)
{
	x_start = x1;
	x_end = x2;
}

void set_ylim(double y1, double y2)
{
	y_start = y1;
	y_end = y2;
}

void plot_fig( double* x, double* y, int len)
{
	length = len;	


	set_xlim(x[0], x[length]);
	set_ylim(y[0], y[length]);

	if(allocated)
	{
		free(x_axis_ptr);
		free(y_axis_ptr);
	}

	x_axis_ptr = (double*) malloc(length*sizeof(double));
	y_axis_ptr = (double*) malloc(length*sizeof(double));
	allocated = true;

	memcpy(x_axis_ptr, x, length*sizeof(double));	
	memcpy(y_axis_ptr, y, length*sizeof(double));	
}

int main(void)
{
	double x[5] = {0.0, 1.0, 2.0, 3.0, 4.0};
	double y[5] = {0.0, 1.0, 0.0, 1.0, 0.0};

	plot_fig(x, y, 5);
	show_plot();


	return 0;
}
