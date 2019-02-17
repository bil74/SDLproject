#ifndef _SDLPARTS_H_
#define _SDLPARTS_H_
#ifndef _LINUX_
#include <SDL.h>
#include <SDL_TTF.h>
#else
//#include <SDL/SDL.h>
//#include <SDL2/SDL_ttf.h>
#include </usr/include/SDL2/SDL.h>
#include </usr/include/SDL2/SDL_ttf.h>
#endif

//public variables


//static variables

// public functions
extern int sdl_open(void);
extern void sdl_close(void);
extern void setup_screen(void);
extern void display_screen(void);
extern void display_text(int screen_column, int screen_row, char *text2disp, SDL_Color color);

/*
extern void InitTimer(void);	//call once on start app
extern void StartCounter(__int64 *timer);	//
extern double GetCounter(__int64 *timer);
//static functions
*/
#endif //_SDLPARTS_H_
