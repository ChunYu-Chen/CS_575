#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifndef NUMT 
#define NUMT 4
#endif

//#define NUMT	        4
#define SIZE       	160000	// you decide
#define NUMTRIES        400	// you decide

float A[SIZE];
float B[SIZE];
float C[SIZE];

int main(int argv, char *argc[])
{
#ifndef _OPENMP
    fprintf(stderr, "OpenMP is not supported here -- sorry.\n");
    return 1;
#endif

    // inialize the arrays:
    for (int i = 0; i < SIZE; i++)
    {
        A[i] = 1.;
        B[i] = 2.;
    }

    omp_set_num_threads(NUMT);
    fprintf(stderr, "Using %d threads\n", NUMT);

    double maxMegaMults = 0.;
    double sumMegaMults = 0.;

    for (int t = 0; t < NUMTRIES; t++)
    {
        double time0 = omp_get_wtime();

#pragma omp parallel for
        for (int i = 0; i < SIZE; i++)
        {
            C[i] = A[i] * B[i];
        }

        double time1 = omp_get_wtime();
        double megaMults = (double)SIZE / (time1 - time0) / 1000000.;
        sumMegaMults += megaMults;        
	if (megaMults > maxMegaMults)
            maxMegaMults = megaMults;
    }
    
    double avgMegaMults = sumMegaMults/(double)NUMTRIES;
    printf("Peak Performance = %8.2lf MegaMults/Sec\n", maxMegaMults);
    printf("Average Performance = %8.2lf MegaMults/Sec\n", avgMegaMults );
    // note: %lf stands for "long float", which is how printf prints a "double"
    //        %d stands for "decimal integer", not "double"

    return 0;
}
