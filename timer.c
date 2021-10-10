#if defined(windows)
#include <windows.h>
#elif defined(linux)
#include <time.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include "timer.h"
#include "utils.h"


#if defined(windows)
double PCFreq = 0.0;
//#define GET_NANO 1.00
#define GET_MICRO 1000000.0
#define GET_MILLIS 1000.00
#elif defined(linux)
#define GET_NANO 1.00
#define GET_MICRO 1000.00
#define GET_MILLIS 1000000.00
struct timespec now;
#endif

void InitTimer()
{
#if defined(windows)
	LARGE_INTEGER li;
	if (!QueryPerformanceFrequency(&li)) {
		printf("QueryPerformanceFrequency failed!\n");
	}

	PCFreq = (double)((li.QuadPart) / GET_MILLIS);		//millisec
#elif defined(linux)
#endif
}

#if defined(windows)
void StartCounter(INT64* i64_counter)
{
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	*i64_counter = li.QuadPart;
}
#elif defined(linux)
void StartCounter(INT64* i64_counter)
{
	clock_gettime(CLOCK_MONOTONIC_RAW, &now);
	*i64_counter = now.tv_sec * 1000000000.0 + now.tv_nsec;
}
#endif

#if defined(windows)
double GetCounter(INT64* i64_counter)
{
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	return (double)((li.QuadPart - *i64_counter) / PCFreq);
}
#elif defined(linux)
double GetCounter(INT64* i64_counter)
{
	clock_gettime(CLOCK_MONOTONIC_RAW, &now);
	return ((now.tv_sec * 1000000000.0 + now.tv_nsec) - *i64_counter) / GET_MILLIS;
}
#endif
