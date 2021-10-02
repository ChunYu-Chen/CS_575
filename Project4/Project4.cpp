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

#ifndef NUMTRIES
#define NUMTRIES	100
#endif

void SimdMul( float *a, float *b, float *c, int len );
void nonSimdMul(float * a, float * b, float *c, int len);
float * Generate_Array(int Size);

int main(){
    float *a = Generate_Array(ARRAY_SIZE);
    float *b = Generate_Array(ARRAY_SIZE);
    float *c = Generate_Array(ARRAY_SIZE);
   
    float MaxSIMDPerformance = .0;
    float MaxNonSIMDPerformance = .0;
    
    for(int i = 0; i < NUMTRIES; i++ ){    
    	double time0 = omp_get_wtime();
	SimdMul(a, b, c, ARRAY_SIZE);           
        double time1 = omp_get_wtime();
		
	double time3 = omp_get_wtime();
        nonSimdMul(a, b, c, ARRAY_SIZE);
        double time4 = omp_get_wtime();

        double MegaMulsPerSecond_SIMD = (double)ARRAY_SIZE / (time1 - time0) / 1000000.0;
        if (MegaMulsPerSecond_SIMD > MaxSIMDPerformance){
			MaxSIMDPerformance = MegaMulsPerSecond_SIMD;
		}    

		double MegaMulsPerSecond_nonSIMD = (double)ARRAY_SIZE / (time4 - time3) / 1000000.0;
        if (MegaMulsPerSecond_nonSIMD > MaxNonSIMDPerformance){
			MaxNonSIMDPerformance = MegaMulsPerSecond_nonSIMD;
		}
    }
/*
    for(int i = 0; i < NUMTRIES; i++ ){	
		double time3 = omp_get_wtime();
        nonSimdMulSum(a, b, c, ARRAY_SIZE);
        double time4 = omp_get_wtime();
	
		double MegaMulsPerSecond_nonSIMD = (double)ARRAY_SIZE / (time4 - time3) / 1000000.0;
        if (MegaMulsPerSecond_nonSIMD > MaxNonSIMDPerformance){
			MaxNonSIMDPerformance = MegaMulsPerSecond_nonSIMD;
		}
    }
*/
	

    float SpeedUp = MaxSIMDPerformance/MaxNonSIMDPerformance;
    printf("%d,%lf\n", ARRAY_SIZE, SpeedUp);

    ofstream outfile;
    outfile.open("result.csv",ios::app);	
    outfile << ARRAY_SIZE << ',' << MaxSIMDPerformance << ',' << MaxNonSIMDPerformance << ',' << SpeedUp << endl;
    outfile.close();

    delete[] a;
    delete[] b;
    return 0;
}
/*
float SimdMulSum( float *a, float *b, int len )
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
    float sum[4] = { 0., 0., 0., 0. };
    
    for( int i = 0; i < len; i++){
        sum[0] += a[i] * b[i];
    }
    
    return sum[0] + sum[1] + sum[2] + sum[3];
}



void SimdMul( float *a, float *b,   float *c,   int len )
{
	int limit = ( len/SSE_WIDTH ) * SSE_WIDTH;
	register float *pa = a;
	register float *pb = b;
	register float *pc = c;
	for( int i = 0; i < limit; i += SSE_WIDTH )
	{
		_mm_storeu_ps( pc,  _mm_mul_ps( _mm_loadu_ps( pa ), _mm_loadu_ps( pb ) ) );
		pa += SSE_WIDTH;
		pb += SSE_WIDTH;
		pc += SSE_WIDTH;
	}

	for( int i = limit; i < len; i++ )
	{
		c[i] = a[i] * b[i];
	}
}
*/

void nonSimdMul(float * a, float * b, float * c,int len){
    //float sum[4] = { 0., 0., 0., 0. };

    for( int i = 0; i < len; i++){
        c[i] += a[i] * b[i];
    }
}


void
SimdMul( float *a, float *b, float *c, int len )
{
	int limit = ( len/SSE_WIDTH ) * SSE_WIDTH;
	__asm
	(
		".att_syntax\n\t"
		"movq -24(%rbp), %r8\n\t" // a
		"movq -32(%rbp), %rcx\n\t" // b
		"movq -40(%rbp), %rdx\n\t" // c
	);
	for( int i = 0; i < limit; i += SSE_WIDTH )
	{
		__asm
		(
			".att_syntax\n\t"
			"movups (%r8), %xmm0\n\t" // load the first sse register
			"movups (%rcx), %xmm1\n\t" // load the second sse register
			"mulps %xmm1, %xmm0\n\t" // do the multiply
			"movups %xmm0, (%rdx)\n\t" // store the result
			"addq $16, %r8\n\t"
			"addq $16, %rcx\n\t"
			"addq $16, %rdx\n\t"
		);
	}

	for( int i = limit; i < len; i++ )
	{
		c[ i ] = a[ i ] * b[ i ];
	}
}


float * Generate_Array(int Size){
    srand(time(0));
    
    float * random = (float *)malloc(sizeof(float) * Size);
    
    for (int i = 0; i < Size; i++) {
        random[i] = ((float)rand()/(float)(RAND_MAX)) * 100;
    }
    return random;
}
