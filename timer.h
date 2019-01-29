#ifndef _TIMER_H_
#define _TIMER_H_

//public variables
//static variables

// public functions
#ifndef _LINUX_
extern void InitTimer(void);	//call once on start app
extern void StartCounter(__int64 *timer);	//
extern double GetCounter(__int64 *timer);
#else
extern void InitTimer(void);	//call once on start app
extern void StartCounter(int64_t *timer);	//
extern double GetCounter(int64_t *timer);
#endif
//static functions

#endif //_TIMER_H_
