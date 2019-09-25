#ifndef _LINUX_
using namespace std;
#include <iostream>
#include <windows.h>
#include <SDL.h>
#else
#include </usr/include/SDL2/SDL.h>
#include </usr/include/SDL2/SDL_ttf.h>
#include <unistd.h>
//#include <SDL/SDL.h>
#include <inttypes.h>
#endif
#include "control.h"

//control method
#define CMETHOD_KEYBOARD 0
#define CMETHOD_GAMEPAD 1
int control_method = CMETHOD_KEYBOARD;		//0-key, 1-gamepad
Uint8 cntvals[MAX_CONTROLS];

//definitions of external variables of this module (declared in module header)
Uint8 *pcntvals = cntvals;
//unsigned char *pcntvals = cntvals;
//----------------------------------------------------------------------------


int fill_controls(void)
{
	int retval;
	const Uint8 *keystates = SDL_GetKeyboardState(NULL);	//get key states
	int key_multiplier = 1;		//key should use 100 here and gamepad could use values below (later)

	SDL_PumpEvents();	//actualize events

	if (control_method == CMETHOD_KEYBOARD) {
		cntvals[CNT_CAM_ROT_RIGHT] = keystates[SDL_SCANCODE_RIGHT] * key_multiplier;
		cntvals[CNT_CAM_ROT_LEFT] = keystates[SDL_SCANCODE_LEFT] * key_multiplier;
		cntvals[CNT_CAM_GO_FORWARD] = keystates[SDL_SCANCODE_UP] * key_multiplier;
		cntvals[CNT_CAM_GO_BACKWARD] = keystates[SDL_SCANCODE_DOWN] * key_multiplier;
		cntvals[CNT_CAM_GO_UP] = keystates[SDL_SCANCODE_W] * key_multiplier;
		cntvals[CNT_CAM_GO_DOWN] = keystates[SDL_SCANCODE_S] * key_multiplier;

		cntvals[CNT_USE] = keystates[SDL_SCANCODE_RETURN];
		cntvals[CNT_ESCAPE] = keystates[SDL_SCANCODE_ESCAPE];
		cntvals[CNT_BACKSPACE] = keystates[SDL_SCANCODE_BACKSPACE];
	}
	else {

	}

	for (retval = 1; !cntvals[retval] && retval < FIRST_EMPTY; retval++)
		;
	/*
	if (retval && retval != FIRST_EMPTY)
		printf("retval:%d\n", retval);
	*/
	return (retval == FIRST_EMPTY) ? 0 : retval;
}

