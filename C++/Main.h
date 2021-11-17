//////////////////////////////////////////////////////////////////
#ifndef MAIN_H													//
#define MAIN_H													//
// begin of Main.h												//
//////////////////////////////////////////////////////////////////
#include "PartClass.h"
#include "WallClass.h"
#include "InputFile.h"
#include "Output.h"
#include "Space.h"
#include "Calculation.h"
#include "General.h"
#include "SearchMethod.h"
#include <stdlib.h>			// for random()

NUMT Random()
/*
	return a random between -1 and 1
*/
{return 2*((rand()/(NUMT)RAND_MAX)-0.5);}

#if defined(_WIN32)
#include <sys/timeb.h>
#define CLK_TCK 1000
#else
#include <limits.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/times.h>
#endif

float TimeDelta(void)
{
/* 
	timedelta: returns the number of seconds that have elapsed since
	the previous call to the function. 
*/
    static long begin = 0;
    static long finish, difference;
#if defined(_WIN32)
    static struct timeb tb;
    ftime(&tb);
    finish = tb.time*1000+tb.millitm;
#else
    static struct tms tb;
    finish = times(&tb);
#endif
    difference = finish - begin;
    begin = finish;
    return (float)difference/(float)CLK_TCK;
}

class Timer
/*
	keeps track of the time consumed by each proces
*/
	{	
	public:	
		Timer(){ini= detectw= detectp= movep= movew= update= save= output= 0;};
		float   ini, detectw, detectp, movep, movew, update, save, output;	
	};

//////////////////////////////////////////////////////////////////
#endif															//
// end of Main.h												//
//////////////////////////////////////////////////////////////////