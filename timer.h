#ifndef _TIMER_H_
#define _TIMER_H_
#include "utils.h"

//public variables
//static variables

// public functions
extern void InitTimer(void);	//call once on start app
extern void StartCounter(INT64 *timer);	//
extern double GetCounter(INT64 *timer);
//static functions

#endif //_TIMER_H_
