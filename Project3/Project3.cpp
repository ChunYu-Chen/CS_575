#include <iostream>
#include <fstream>
#include <string>
#include <omp.h>
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

int	NowYear;		// 2021 - 2026
int	NowMonth;		// 0 - 11

float	NowPrecip;		// inches of rain per month
float	NowTemp;		// temperature this month
float	NowHeight;		// grain height in inches
int	NowNumDeer;		// number of deer in the current population
int     NowHunt;                // number of deer was hunted this month
int     Day = 0;      

const float GRAIN_GROWS_PER_MONTH =		9.0;
const float ONE_DEER_EATS_PER_MONTH =		1.0;

const float AVG_PRECIP_PER_MONTH =		7.0;	// average
const float AMP_PRECIP_PER_MONTH =		6.0;	// plus or minus
const float RANDOM_PRECIP =			2.0;	// plus or minus noise

const float AVG_TEMP =				60.0;	// average
const float AMP_TEMP =				20.0;	// plus or minus
const float RANDOM_TEMP =			10.0;	// plus or minus noise

const float MIDTEMP =				40.0;
const float MIDPRECIP =				10.0;

const int CONSERVATION_ZONE =                   2.0;    //Protection policy

unsigned int seed = 0;

float Ranf(unsigned int* seedp, float low, float high);
int Ranf(unsigned int* seedp, int ilow, int ihigh);
float SQR(float x);
float set_Angle(int NowMonth);
float set_Temp(unsigned int* seed, float ang);
float set_Precip(unsigned int* seed, float ang);
void Deer(unsigned int* seed);
void Grain(unsigned int* seed);
void Watcher(unsigned int* seed);
void MyAgent(unsigned int* seed);

int main()
{

	// starting date and time:
	NowMonth =    0;
	NowYear  = 2021;
	
	// starting state (feel free to change this if you want):
	NowNumDeer = 1;
	NowHeight =  1.;

	#ifndef _OPENMP
    		fprintf(stderr, "OpenMP is not supported on this machine. \n");
    		return 1;
	#endif
		
	ofstream outfile;
	outfile.open("result.csv", ios::app);
	outfile << Day << ',' << "Temp" << ',' << "Precip" << ',' << "Height" << ',' << "NumDeer" << ',' << "DeerHunt" << endl;	
	float ang = set_Angle(NowMonth);
	
	NowTemp = set_Temp(&seed, ang);
	
	NowPrecip = set_Precip(&seed, ang);

	omp_set_num_threads( 4 );	// same as # of sections
	
		
	#pragma omp parallel sections
	{
		#pragma omp section
		{
			Deer(&seed);
		}

		#pragma omp section
		{
			Grain(&seed);
		}

		#pragma omp section
		{
			Watcher(&seed);
		}

		#pragma omp section
		{
			MyAgent(&seed);	// your own
		}
	}       // implied barrier -- all functions must return in order
	// to allow any of them to get past here
	return 0;
}

float Ranf(unsigned int* seedp, float low, float high)
{
    float r = (float)rand_r(seedp); // 0-RAND_MAX
    return (low + r * (high - low) / (float)RAND_MAX);
}

int Ranf(unsigned int* seedp, int ilow, int ihigh)
{
    float low = (float)ilow;
    float high = (float)ihigh + 0.9999f;
    return (int)(Ranf(seedp, low, high));
}

float SQR(float x)
{
    return x * x;
}

float set_Angle(int NowMonth)
{
	float ang = (  30.*(float)NowMonth + 15.  ) * ( M_PI / 180. );
	return ang;
}

float set_Temp(unsigned int* seed, float ang)
{
	float temp = AVG_TEMP - AMP_TEMP * cos(ang);
	NowTemp = (temp + Ranf(seed, -RANDOM_TEMP, RANDOM_TEMP));
	return NowTemp;
}


float set_Precip(unsigned int* seed, float ang)
{
    float precip = AVG_PRECIP_PER_MONTH + AMP_PRECIP_PER_MONTH * sin(ang);
    NowPrecip = precip + Ranf(seed, -RANDOM_PRECIP, RANDOM_PRECIP);
    if (NowPrecip < 0.)
    	NowPrecip = 0.;
    return NowPrecip;
}

void Deer(unsigned int* seed)
{
	while (NowYear < 2027)
    	{
		int nextNumDeer = NowNumDeer;
		int carryingCapacity = (int)( NowHeight );
		
		if( nextNumDeer < carryingCapacity )
			nextNumDeer++;
		else if( nextNumDeer > carryingCapacity )
			nextNumDeer--;
		if( nextNumDeer < 0 )
			nextNumDeer = 0;
		  
		#pragma omp barrier
		NowNumDeer = nextNumDeer;

		#pragma omp barrier


		#pragma omp barrier


		#pragma omp barrier
    	}		

}

void Grain(unsigned int* seed)
{
	while (NowYear < 2027)
     	{
		float tempFactor = exp(-SQR((NowTemp - MIDTEMP) / 10.));
		float precipFactor = exp(-SQR((NowPrecip - MIDPRECIP) / 10.));
		float nextHeight = NowHeight;
		nextHeight += tempFactor * precipFactor * GRAIN_GROWS_PER_MONTH;
		nextHeight -= (float)NowNumDeer * ONE_DEER_EATS_PER_MONTH;
		if (nextHeight < 0)
		    nextHeight = 0.;

		#pragma omp barrier
		NowHeight = nextHeight;

		#pragma omp barrier


		#pragma omp barrier


		#pragma omp barrier
    	}
}

void Watcher(unsigned int* seed)
{
	while (NowYear < 2027)
    	{

		#pragma omp barrier


		#pragma omp barrier


		#pragma omp barrier
		fprintf(stderr, "Year: %d, Month %d\n Temperature:     %.2f\n Precipitation:   %.2f\n Grain Height:    %.2f\n Number of Deer:  %d\n Deer Hunted:     %d\n\n", NowYear, NowMonth, (5./9.)*(NowTemp-32), NowPrecip*2.54, NowHeight*2.54, NowNumDeer, NowHunt);
		
		ofstream outfile;
		outfile.open("result.csv",ios::app);	
		outfile << ++Day << ',' 
			<< (5./9.)*(NowTemp-32) << ',' 
			<< NowPrecip*2.54 << ',' 
			<< NowHeight*2.54 << ',' 
			<< NowNumDeer << ',' 
			<< NowHunt << endl; 	
		outfile.close();

		NowMonth++;
		if (NowMonth == 12) {
		    NowMonth = 0;
		    NowYear++;
		}
	       
		float ang = set_Angle(NowMonth);
		NowTemp = set_Temp(seed, ang);
		NowPrecip = set_Precip(seed, ang);
	      
		#pragma omp barrier
    	}
}

void MyAgent(unsigned int* seed)
{
	while (NowYear < 2027)
    	{
		int NewHunt = Ranf(seed, 0, NowNumDeer/2);
		
		#pragma omp barrier
		NowHunt = NewHunt;
		#pragma omp barrier
		NowNumDeer -= NowHunt;
		#pragma omp barrier
		
		#pragma omp barrier
    	}
}


