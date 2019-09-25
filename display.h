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
extern int screen_res_x;
extern int screen_res_y;
extern Uint32 *pixels;		//texture mem
extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern SDL_Texture *texture;

typedef enum {
	m_resume = 1,
	m_fullscreen,
	m_windowed,
	m_vsync_on,
	m_vsync_off,
	m_sres,
	m_rres,
	m_save_conf,
	m_exit
} menuselect;


//static variables


// public functions
extern int sdl_start(void);
extern void sdl_lowlev_stop(void);
extern int sdl_lowlev_start(void);
extern void sdl_stop(void);
extern void setup_screen(void);
extern void display_screen(void);
extern void display_text(int screen_column, int screen_row, char *text2disp, SDL_Color color);
extern int menu_main(void);

/*
extern void InitTimer(void);	//call once on start app
extern void StartCounter(__int64 *timer);	//
extern double GetCounter(__int64 *timer);
//static functions
*/
#endif //_SDLPARTS_H_
