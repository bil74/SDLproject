#ifndef _TIMER_H_
#define _TIMER_H_

//public variables
//static variables

// public functions
#ifndef _LINUX_
#define INT64 __int64
#else
#define INT64 int64_t
#endif
extern void InitTimer(void);	//call once on start app
extern void StartCounter(INT64 *timer);	//
extern double GetCounter(INT64 *timer);
//static functions

#endif //_TIMER_H_
