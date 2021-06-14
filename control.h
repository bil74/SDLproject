#ifndef _CONTROL_H_
#define _CONTROL_H_


#define MAX_CONTROLS 50
#define RET_MENU 1
#define RET_EXIT 2

// --------------------------- public declarations
extern Uint8 *pcntvals;
#define FIRST_EMPTY 10
//we should list all kinds of controls listed here
typedef enum
{
	CNT_CAM_GO_FORWARD = 1,
	CNT_CAM_GO_BACKWARD = 2,
	CNT_CAM_ROT_RIGHT = 3,
	CNT_CAM_ROT_LEFT = 4,
	CNT_CAM_GO_UP = 5,
	CNT_CAM_GO_DOWN = 6,
	CNT_ESCAPE = 7,
	CNT_BACKSPACE = 8,
	CNT_USE = 9,

	CNT_END_FILLER = MAX_CONTROLS
} controls;


int fill_controls(void);


#endif //_CONTROL_H_
