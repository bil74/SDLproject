#ifndef _SDLPARTS_H_
#define _SDLPARTS_H_
#include <SDL.h>
#include <SDL_TTF.h>

//public variables


//static variables

// public functions
extern int sdl_open(void);
extern void sdl_close(void);
extern void setup_screen(void);
extern void display_screen(void);
void display_text(int screen_x, int screen_y, char *text2disp);
void display_text_init();

/*
extern void InitTimer(void);	//call once on start app
extern void StartCounter(__int64 *timer);	//
extern double GetCounter(__int64 *timer);
//static functions
*/
#endif //_SDLPARTS_H_
