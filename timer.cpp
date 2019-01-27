//#include "stdafx.h"

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "timer.h"

double PCFreq = 0.0;
//__int64 CounterStart = { 0 };

void InitTimer()
{
	LARGE_INTEGER li;
	if (!QueryPerformanceFrequency(&li)) {
		printf("QueryPerformanceFrequency failed!\n");
	}

	PCFreq = double(li.QuadPart) / 1000.0;		//millisec
	//PCFreq = double(li.QuadPart) / 1000000.0;	//microsec
}

void StartCounter(__int64 *i64_counter)
{
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	*i64_counter = li.QuadPart;
}

double GetCounter(__int64 *i64_counter)
{
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	return double(li.QuadPart - *i64_counter) / PCFreq;
}
