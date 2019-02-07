#ifndef _LINUX_
#include <windows.h>
#else
#include <time.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include "timer.h"


#ifndef _LINUX_
double PCFreq = 0.0;
//#define GET_NANO 1.00
#define GET_MICRO 1000000.0
#define GET_MILLIS 1000.00
#else
	#define GET_NANO 1.00
	#define GET_MICRO 1000.00
	#define GET_MILLIS 1000000.00
	struct timespec now;
#endif

void InitTimer()
{
#ifndef _LINUX_
	LARGE_INTEGER li;
	if (!QueryPerformanceFrequency(&li)) {
		printf("QueryPerformanceFrequency failed!\n");
	}

	PCFreq = double(li.QuadPart) / GET_MILLIS;		//millisec
#else
#endif
}

#ifndef _LINUX_
void StartCounter(INT64 *i64_counter)
{
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	*i64_counter = li.QuadPart;
}
#else
void StartCounter(INT64 *i64_counter)
{
	clock_gettime(CLOCK_MONOTONIC_RAW, &now);
	*i64_counter = now.tv_sec * 1000000000.0 + now.tv_nsec;
}
#endif

#ifndef _LINUX_
double GetCounter(INT64 *i64_counter)
{
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	return double(li.QuadPart - *i64_counter) / PCFreq;
}
#else
double GetCounter(INT64 *i64_counter)
{
	clock_gettime(CLOCK_MONOTONIC_RAW, &now);
	return ((now.tv_sec * 1000000000.0 + now.tv_nsec) - *i64_counter) / GET_MILLIS;
}
#endif
