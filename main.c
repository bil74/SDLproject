#include <stdio.h>
#include <math.h>
#include <time.h>
#include "utils.h"

#if defined(windows)
#include <windows.h>
#include <SDL.h>
#elif defined(linux)
#include <unistd.h>
//#include <SDL/SDL.h>
#include </usr/include/SDL2/SDL.h>
#include </usr/include/SDL2/SDL_ttf.h>
#include <inttypes.h>
#endif
#include "main.h"
#include "timer.h"
#include "display.h"
#include "render.h"
#include "control.h"
#include "movement.h"
#include "config.h"


//definitions of external variables of this module (declared in module header)
SDL_bool dbg_global = SDL_FALSE;
INT64 timer_main;
SDL_bool done = SDL_FALSE;
SDL_bool restart_sdl = SDL_FALSE;

//----------------------------------------------------------------------------

void do_menu_stuff(int menu_selection);

int main(int argc, char* argv[])
{
	/*
	testing();
	return 0;
	*/
	int menusel;

	config_load();
	if (sdl_lowlev_start() != 0)
		return -1;

	if (init_some_stuff() != 0) {
		printf("init_some_stuff failed!\n");
		return -1;
	}

	//init sdl
	if (sdl_start() != 0) {
		printf("sdl_start failed!\n");
		return -1;
	}

	if (start_render() != 0) {
		printf("start_render failed!\n");
		return -1;
	}


	while (!done)
	{
		//restart_sdl = SDL_FALSE;
		setup_screen();

		//render scene to texture
		render_scene(&scene, p_dpolys);

		//display the screen
		display_screen();

		//control
		switch (fill_controls()) {
		case (CNT_ESCAPE):
			menusel = menu_main();
			do_menu_stuff(menusel);
			break;
		default:
			movement();
			break;
		}
		StartCounter(&timer_main);
	}
	sdl_stop();
	close_some_stuff();
	sdl_lowlev_stop();
	return 0;
}


void do_menu_stuff(int menu_selection)
{
	int retval = 0;
	//printf("%s; menu_selection:%d\n", __func__, menu_selection);
	switch (menu_selection) {
	case m_exit:
		done = SDL_TRUE;
		break;
	case m_save_conf:
		config_save();
		break;
	case m_resume:
		if (is_config_changed(SUBSYS_SDL, FLAG_APPLY, SDL_TRUE)) {		//if sdl params changed, restart sdl
			sdl_stop();
			if (sdl_start() != 0)
				done = SDL_TRUE;
		}
		if (is_config_changed(SUBSYS_RND, FLAG_APPLY, SDL_TRUE)) {		//if render params changed, restart render
			stop_render();
			if (start_render() != 0)
				done = SDL_TRUE;
		}
		break;
	default:
		break;
	}
	return;

}
