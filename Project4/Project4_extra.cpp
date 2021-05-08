#include <stdio.h>
#include <stdlib.h> 
#include <xmmintrin.h>
#include <time.h>
#include <omp.h>
#include <fstream>
#include <iostream>
using namespace std;

#define SSE_WIDTH	4

#ifndef ARRAY_SIZE
#define ARRAY_SIZE      1024
#endif

#ifndef NUMT
#define NUMT      1
#endif

#define NUM_ELEMENTS_PER_CORE   ARRAY_SIZE/NUMT

#ifndef NUMTRIES
#define NUMTRIES	100
#endif

float SimdMulSum( float *a, float *b, int len );
float nonSimdMulSum(float * a, float * b, int len);
float * Generate_Array(int Size);

int main(){
    float *a = Generate_Array(ARRAY_SIZE);
    float *b = Generate_Array(ARRAY_SIZE);
    
    omp_set_num_threads(NUMT);
    
    float MaxSIMDPerformance = .0;
    float MaxNonSIMDPerformance = .0;

    for(int i = 0; i < NUMTRIES; i++ ){
        double time0 = omp_get_wtime();
        #pragma omp parallel
        {
            int first = omp_get_thread_num( ) * NUM_ELEMENTS_PER_CORE;
            SimdMulSum(&a[first], &b[first], NUM_ELEMENTS_PER_CORE);
        }
        double time1 = omp_get_wtime();
        
        double MegaMulsPerSecond_SIMD = (float)ARRAY_SIZE / (time1 - time0) / 1000000.0;
        if (MegaMulsPerSecond_SIMD > MaxSIMDPerformance){
			MaxSIMDPerformance = MegaMulsPerSecond_SIMD;
		}

        double time3 = omp_get_wtime();
        nonSimdMulSum(a, b, ARRAY_SIZE);
        double time4 = omp_get_wtime();
        
        double MegaMulsPerSecond_nonSIMD = (float)ARRAY_SIZE / (time4 - time3) / 1000000.0;
        if (MegaMulsPerSecond_nonSIMD > MaxNonSIMDPerformance){
			MaxNonSIMDPerformance = MegaMulsPerSecond_nonSIMD;
		}
    }
    float SpeedUp = MaxSIMDPerformance/MaxNonSIMDPerformance;
    printf("NUMT: %d, ARRAY_SIZE: %d, SIMD: %lf, Non-SIMD: %1f, SpeedUp: %1f\n", NUMT, ARRAY_SIZE, MaxSIMDPerformance, MaxNonSIMDPerformance,SpeedUp);
    //printf("%d,%d,%lf\n", NUMT, ARRAY_SIZE, MaxNonSIMDPerformance);

    ofstream outfile;
    outfile.open("result2.csv",ios::app);	
    outfile << NUMT << ',' << ARRAY_SIZE << ',' << MaxSIMDPerformance << ',' << MaxNonSIMDPerformance << ',' << SpeedUp << endl;
    outfile.close();

    delete[] a;
    delete[] b;
    return 0;
}

float
SimdMulSum( float *a, float *b, int len )
{
	float sum[4] = { 0., 0., 0., 0. };
	int limit = ( len/SSE_WIDTH ) * SSE_WIDTH;
	register float *pa = a;
	register float *pb = b;
    
	__m128 ss = _mm_loadu_ps( &sum[0] );
	for( int i = 0; i < limit; i += SSE_WIDTH )
	{
		ss = _mm_add_ps( ss, _mm_mul_ps( _mm_loadu_ps( pa ), _mm_loadu_ps( pb ) ) );
		pa += SSE_WIDTH;
		pb += SSE_WIDTH;
	}
	_mm_storeu_ps( &sum[0], ss );

	for( int i = limit; i < len; i++ )
	{
		sum[0] += a[i] * b[i];
	}

	return sum[0] + sum[1] + sum[2] + sum[3];
}

float nonSimdMulSum(float * a, float * b, int len){
    float sum = 0.;
    //#pragma omp parallel for reduction(+:sum)
    for( int i = 0; i < len; i++){
        sum += a[i] * b[i];
    }
   return sum;
}

float * Generate_Array(int Size){
    srand(time(0));
    
    float * random = (float *)malloc(sizeof(float) * Size);
    
    for (int i = 0; i < Size; i++) {
        random[i] = ((float)rand()/(float)(RAND_MAX)) * 100;
    }
    return random;
}
