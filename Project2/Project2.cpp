#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <omp.h>

// setting the number of threads:
#ifndef NUMT
#define NUMT		8
#endif

// setting the number of nodes:
#ifndef NUMNODES
#define NUMNODES	5000
#endif

//ranges for the X Y numbers:
#define XMIN     -1.
#define XMAX      1.
#define YMIN     -1.
#define YMAX      1.

#define N	0.7
// function prototypes:
float Height(int, int);

// main program:
int main(int argc, char* argv[])
{
	#ifndef _OPENMP
		fprintf(stderr, "No OpenMP support!\n");
		return 1;
	#endif

	omp_set_num_threads(NUMT);	// set the number of threads to use in the for-loop:`

	//float maxPerformance = 0.;      // must be declared outside the NUMTRIES loop
	float volume = 0;
	double time0 = omp_get_wtime();
	#pragma omp parallel for default(none),reduction(+:volume)
	for (int i = 0; i < NUMNODES*NUMNODES; i++)
	{
			
		int iu = i % NUMNODES;
		int iv = i / NUMNODES;
		float z = Height(iu, iv);
		float temp = 0;

		float fullTileArea = ( ( ( XMAX - XMIN ) / (float)(NUMNODES-1) ) * 
							   ( ( YMAX - YMIN ) / (float)(NUMNODES-1) ) );

		int border = NUMNODES - 1;
		if (iu == 0. && iv == 0. || iu == 0. && iv == border || iu == border && iv == 0 || iu == border && iv == border)
		{
			temp = 2. * z * fullTileArea / 4.;
		}
		else if (iu == 0. || iv == 0. || iu == border || iv == border)
		{
			temp = 2. * z * fullTileArea / 2.;
		}
		else
		{
			temp = 2. * z * fullTileArea;
		}

		volume += temp;
	}

	double time1 = omp_get_wtime();
	double maxPerformance = (double)NUMNODES * (double)NUMNODES / (time1 - time0) / 1000000.;
	//if (megaHeightsPerSecond > maxPerformance)
		//maxPerformance = megaHeightsPerSecond;
	printf("Volume = %f,\tthe number of threads = %d,\tthe number of nodes = %d,\tMaxPerformance = %8.2lf\t\n",volume, NUMT, NUMNODES, maxPerformance);
	return 0;
}


float Height( int iu, int iv )	// iu,iv = 0 .. NUMNODES-1
{
	float x = -1.  +  2.*(float)iu /(float)(NUMNODES-1);	// -1. to +1.
	float y = -1.  +  2.*(float)iv /(float)(NUMNODES-1);	// -1. to +1.

	float xn = pow( fabs(x), (double)N );
	float yn = pow( fabs(y), (double)N );
	float r = 1. - xn - yn;
	if( r <= 0. )
	        return 0.;
	float height = pow( r, 1./(float)N );
	return height;
}
