#ifndef _TIMER_H_
#define _TIMER_H_

//public variables
//static variables

// public functions
extern void InitTimer(void);	//call once on start app
extern void StartCounter(__int64 *timer);	//
extern double GetCounter(__int64 *timer);
//static functions

#endif //_TIMER_H_