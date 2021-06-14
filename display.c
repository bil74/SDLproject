#ifndef _LINUX_
#include <windows.h>
#include <SDL.h>
#else
#include </usr/include/SDL2/SDL.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include "timer.h"
#include "display.h"
#include "control.h"
#include "render.h"
#include "config.h"

#define WINDOW_TITLE "sdl3d try"
//const char *font_file = { "C:\\Windows\\Fonts\\lucon.ttf" };
//const char *font_file = { "Vera.ttf" };
const char* font_file = { "lucon.ttf" };
TTF_Font* ttf_Font; //this opens a font style and sets a size
SDL_Color ttf_FColor;  // foreground color
SDL_Color ttf_BColor;  // background color

//definitions of external variables of this module (declared in module header)
int screen_res_x = 800;		//render res x
int screen_res_y = 600;		//render res y
Uint32* pixels;		//texture mem
Uint32* pixels_background;		//texture mem for sky and ground
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* texture;
//----------------------------------------------------------------------------


//local functions
int ttf_init_setup(char* font_fname);


// local variables
int screen_native_xres;
int screen_native_yres;
int screen_res_window_x;
int screen_res_window_y;

struct menuitem {
	int level;
	int selection;
	int parent_id;	//parent position in the array
	SDL_bool active;
	SDL_bool visible;
	char text[50];
	int conf_id;
	int conf_val;
};


struct menuitem menuitems[] = {
	/*00*/	{0, 0, -1, SDL_FALSE, SDL_FALSE, "main menu", -1, 0},
	/*01*/		{1, m_resume, 0, SDL_FALSE, SDL_FALSE, "resume", -1, 0 },
	/*02*/		{1, 0, 0, SDL_FALSE, SDL_FALSE, "video", -1, 0 },
	/*03*/			{2, 0, 2, SDL_FALSE, SDL_FALSE, "type", -1, 0 },
	/*04*/				{3, m_resume, 3, SDL_FALSE, SDL_FALSE, "fullscreen", fullscr, 1 },
	/*05*/				{3, m_resume, 3, SDL_FALSE, SDL_FALSE, "windowed", fullscr, 0 },
	/*06*/			{2, 0, 2, SDL_FALSE, SDL_FALSE, "window resolution", -1, 0 },
	/*07*/				{3, m_resume, 6, SDL_FALSE, SDL_FALSE, "resolution #1", sres_y, 0},
	/*08*/				{3, m_resume, 6, SDL_FALSE, SDL_FALSE, "resolution #2", sres_y, 0 },
	/*09*/				{3, m_resume, 6, SDL_FALSE, SDL_FALSE, "resolution #3", sres_y, 0 },
	/*10*/				{3, m_resume, 6, SDL_FALSE, SDL_FALSE, "resolution #4", sres_y, 0 },
	/*11*/				{3, m_resume, 6, SDL_FALSE, SDL_FALSE, "resolution #5", sres_y, 0 },
	/*12*/				{3, m_resume, 6, SDL_FALSE, SDL_FALSE, "resolution #6", sres_y, 0 },
	/*13*/				{3, m_resume, 6, SDL_FALSE, SDL_FALSE, "resolution #7", sres_y, 0 },
	/*14*/				{3, m_resume, 6, SDL_FALSE, SDL_FALSE, "resolution #8", sres_y, 0 },
	/*15*/				{3, m_resume, 6, SDL_FALSE, SDL_FALSE, "resolution #9", sres_y, 0 },
	/*16*/				{3, m_resume, 6, SDL_FALSE, SDL_FALSE, "resolution #a", sres_y, 0 },


	/*17*/			{2, 0, 2, SDL_FALSE, SDL_FALSE, "vsync", -1, 0 },
	/*18*/				{3, m_resume, 17, SDL_FALSE, SDL_FALSE, "on", vsync, 1},
	/*19*/				{3, m_resume, 17, SDL_FALSE, SDL_FALSE, "off", vsync, 0 },

	/*20*/		{1, 0, 0, SDL_FALSE, SDL_FALSE, "render", -1, 0 },
	/*21*/			{2, 0, 20, SDL_FALSE, SDL_FALSE, "drawmode", -1, 0 },
	/*22*/				{3, m_resume, 21, SDL_FALSE, SDL_FALSE, "flat-shaded", drawmode, 0 },
	/*23*/				{3, m_resume, 21, SDL_FALSE, SDL_FALSE, "wireframe", drawmode, 1 },

	/*24*/			{2, 0, 20, SDL_FALSE, SDL_FALSE, "render resolution", -1, 0 },
	/*25*/				{3, m_resume, 24, SDL_FALSE, SDL_FALSE, "resolution #1", rres_y, 0 },
	/*26*/				{3, m_resume, 24, SDL_FALSE, SDL_FALSE, "resolution #2", rres_y, 0 },
	/*27*/				{3, m_resume, 24, SDL_FALSE, SDL_FALSE, "resolution #3", rres_y, 0 },
	/*28*/				{3, m_resume, 24, SDL_FALSE, SDL_FALSE, "resolution #4", rres_y, 0 },
	/*29*/				{3, m_resume, 24, SDL_FALSE, SDL_FALSE, "resolution #5", rres_y, 0 },
	/*30*/				{3, m_resume, 24, SDL_FALSE, SDL_FALSE, "resolution #6", rres_y, 0 },
	/*31*/				{3, m_resume, 24, SDL_FALSE, SDL_FALSE, "resolution #7", rres_y, 0 },
	/*32*/				{3, m_resume, 24, SDL_FALSE, SDL_FALSE, "resolution #8", rres_y, 0 },
	/*33*/				{3, m_resume, 24, SDL_FALSE, SDL_FALSE, "resolution #9", rres_y, 0 },
	/*34*/				{3, m_resume, 24, SDL_FALSE, SDL_FALSE, "resolution #a", rres_y, 0 },


	/*35*/		{1, 0, 0, SDL_FALSE, SDL_FALSE, "audio", -1, 0 },
	/*36*/		{1, 0, 0, SDL_FALSE, SDL_FALSE, "controls", -1, 0 },
	/*37*/		{1, 0, 0, SDL_FALSE, SDL_FALSE, "gameplay", -1, 0 },
	/*38*/		{1, m_save_conf, 0, SDL_FALSE, SDL_FALSE, "save setup", -1, 0 },
	/*39*/		{1, m_exit, 0, SDL_FALSE, SDL_FALSE, "exit", -1, 0 }
};


void menu_init(void)
{
	int i, j, k, parent;
	const int constint = 10;

	for (i = 0, j = k = constint; i < ARRAY_SIZE(menuitems); i++) {
		menuitems[i].visible = SDL_FALSE;
		menuitems[i].active = SDL_FALSE;
		parent = menuitems[i].parent_id;
		//fill window resolutions
		if (parent != -1 && (memcmp(menuitems[parent].text, "window resolution", strlen("window resolution")) == 0)) {
			snprintf(menuitems[i].text, strlen("resolution #1"), "y=%d", (screen_native_yres * j) / constint);
			//menuitems[i].selection = 10000 + (screen_native_yres * j) / constint;
			menuitems[i].conf_val = (screen_native_yres * j) / constint;
			j--;

		}
		//fill render resolutions
		if (parent != -1 && (memcmp(menuitems[parent].text, "render resolution", strlen("render resolution")) == 0)) {
			snprintf(menuitems[i].text, strlen("resolution #1"), "y=%d", (screen_native_yres * k) / constint);
			//menuitems[i].selection = 20000 + (screen_native_yres * k) / constint;
			menuitems[i].conf_val = (screen_native_yres * k) / constint;
			k--;
		}

	}

}

//invoke main menu 
int menu_main(void)
{
#define MENULEVEL_MAX 10
	int selected = 0;
	int currlevel = 0;
	int keypressed;
	int activelist[MENULEVEL_MAX] = { 0 };


	//wait for key-release
	while (pcntvals[CNT_ESCAPE]) {
		fill_controls();
	}

	//init menu
	//fill necessary values
	menu_init();
	//set position to first item on the first menu level
	activelist[currlevel++] = 0;
	activelist[currlevel] = 1;

	//main menu cycle
	while (!selected) {
		int i, ypos, activeindex, j;
		SDL_Color col_txt_inactive = { 130, 130, 130, 255 };		//grey-ish
		SDL_Color col_txt_active = { 255, 255, 255, 255 };			//white

		//display menu
		setup_screen();
		//update texture
		//SDL_UpdateTexture(texture, NULL, pixels, screen_res_x * sizeof(Uint32));	//enable this to delete scene
		//copy texture to renderer
		SDL_RenderCopy(renderer, texture, NULL, NULL);

		activeindex = activelist[currlevel];

		//display menu items
		for (i = 0, ypos = 0; i < ARRAY_SIZE(menuitems); i++) {
			char tmpstr[50];
			SDL_bool used = SDL_FALSE;

			//set defaults
			menuitems[i].visible = SDL_FALSE;
			menuitems[i].active = SDL_FALSE;

			//set visible if its in the active history
			for (j = 0; j < currlevel; j++) {
				if (i == activelist[j])
					menuitems[i].visible = SDL_TRUE;
			}

			//set visible if parent is the last in activelist
			if (menuitems[i].parent_id == activelist[currlevel - 1])
				menuitems[i].visible = SDL_TRUE;

			//set active
			if (activeindex == i)
				menuitems[i].active = SDL_TRUE;

			//set used flag for remarking current values
			if (get_conf_val(menuitems[i].conf_id) == menuitems[i].conf_val)
				used = SDL_TRUE;

			//display if visible
			if (menuitems[i].visible) {
				snprintf(tmpstr, sizeof(tmpstr), "%s%s", menuitems[i].text, used ? "(*)" : "");
				display_text(menuitems[i].level, ypos++, tmpstr, menuitems[i].active ? col_txt_active : col_txt_inactive);
			}

		}
		display_screen();

		//wait for keypress-release
		while ((keypressed = fill_controls()) == 0)
			;

		//controls for menu
		switch (keypressed) {
		case CNT_USE:			//go down into child-menu
		case CNT_CAM_ROT_RIGHT:
			if (menuitems[activeindex + 1].parent_id == activeindex) {
				activelist[++currlevel] = ++activeindex;
			}
			else if (menuitems[activeindex].selection) {
				//change config values if needed
				if (menuitems[activeindex].conf_id != -1) {
					set_conf_val(menuitems[activeindex].conf_id, menuitems[activeindex].conf_val);
				}
				//return selection
				selected = menuitems[activeindex].selection;
			}
			break;
		case CNT_ESCAPE:		//go up into parent-menu
		case CNT_CAM_ROT_LEFT:
			if (currlevel == 0)
				selected = m_resume;
			else {
				if (menuitems[activeindex].parent_id == 0)
					selected = menuitems[1].selection;
				else
					currlevel--;
			}
			break;
		case CNT_CAM_GO_BACKWARD:	//down
			for (i = activeindex + 1; i < ARRAY_SIZE(menuitems); i++)
				if (menuitems[activeindex].parent_id == menuitems[i].parent_id) {
					activelist[currlevel] = i;
					break;
				}
			break;
		case CNT_CAM_GO_FORWARD:	//up
			for (i = activeindex - 1; i >= 0; i--)
				if (menuitems[activeindex].parent_id == menuitems[i].parent_id) {
					activelist[currlevel] = i;
					break;
				}
			break;
		}

		//wait for keypress-release
		while (fill_controls() == keypressed)
			;



	}

	//wait for key-release
	while (pcntvals[CNT_ESCAPE]) {
		fill_controls();
	}

	return selected;
}

int sdl_lowlev_start(void)
{
	int display_count = 0, display_index = 0, mode_index = 0;
	SDL_DisplayMode mode = { SDL_PIXELFORMAT_UNKNOWN, 0, 0, 0, 0 };

	//init sdl
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {	//SDL_INIT_EVERYTHING
		printf(" SDL_Init failed: %s\n", SDL_GetError());
		return -1;
	}

	//get display mode for resolution
	if (SDL_GetDisplayMode(display_index, mode_index, &mode) != 0) {
		printf("SDL_GetDisplayMode failed: %s\n", SDL_GetError());
		return 1;
	}
	/*
	printf("SDL_GetDisplayMode(0, 0, &mode):\t\t%i bpp\t%i x %i\n",
		SDL_BITSPERPIXEL(mode.format), mode.w, mode.h);
	*/

	//printf("%s; SDL_GetNumVideoDisplays():%d\n", __func__, SDL_GetNumVideoDisplays());
	//printf("%s; SDL_GetNumDisplayModes():%d\n", __func__, SDL_GetNumDisplayModes(0));

	//save screen native resolution
	screen_native_xres = mode.w;
	screen_native_yres = mode.h;


	//init SDL TrueType fonts to write to screen
	if (TTF_Init() == -1)
	{
		printf("TTF_Init failed!\n");
		return -1;
	}
	if (ttf_init_setup((char*)font_file) == -1) {
		printf("TTF load font \"%s\" failed!\n", (char*)font_file);
		return -1;
	}

	return 0;
}

void sdl_lowlev_stop(void)
{
	TTF_CloseFont(ttf_Font);
	SDL_Quit();

}

int sdl_start(void) {


	//calculate window resolution
	screen_res_window_y = get_conf_val(sres_y);
	screen_res_window_x = (get_conf_val(sres_y) * screen_native_xres) / screen_native_yres;
	//printf("%s; #1 screen_res_window_x:%d, screen_res_window_y:%d\n", __func__, screen_res_window_x, screen_res_window_y);

	//calculate render resolution
	screen_res_y = get_conf_val(rres_y);
	screen_res_x = (get_conf_val(rres_y) * screen_native_xres) / screen_native_yres;

	/*
	printf("%s; screen_native_xres:%d, screen_native_yres:%d\n", __func__, screen_native_xres, screen_native_yres);
	printf("%s; screen_res_window_x:%d, screen_res_window_y:%d\n", __func__, screen_res_window_x, screen_res_window_y);
	printf("%s; screen_res_x:%d, screen_res_y:%d\n", __func__, screen_res_x, screen_res_y);
	*/

	if (get_conf_val(fullscr) == 1)
		window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen_native_xres, screen_native_yres, SDL_WINDOW_FULLSCREEN_DESKTOP);
	else
		window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen_res_window_x, screen_res_window_y, 0);
	// Check that the window was successfully created
	if (window == NULL) {
		printf("Could not create window: %s\n", SDL_GetError());
		return -1;
	}

	if (get_conf_val(vsync) == 1)
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	else
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	// Check that the renderer was successfully created
	if (renderer == NULL) {
		printf("Could not create renderer: %s\n", SDL_GetError());
		return -1;
	}

	//set render res
	SDL_RenderSetLogicalSize(renderer, screen_res_x, screen_res_y);

	//get actual desktop resolution
	SDL_GetWindowSize(window, &screen_res_window_x, &screen_res_window_y);
	//SDL_RenderGetLogicalSize(renderer, &screen_res_x, &screen_res_y);
	//printf("%s; #2 screen_res_window_x:%d, screen_res_window_y:%d\n", __func__, screen_res_window_x, screen_res_window_y);

	//calc persp_value for render module
	set_persp_value((screen_res_x * 100) / 120);

	//printf("screen_res_x:%d, screen_res_y:%d, persp_val:%d\n", screen_res_x, screen_res_y, (screen_res_x * 100) / 120);

	//now we will draw to texture
	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_STREAMING, screen_res_x, screen_res_y);
	if (texture == NULL) {
		printf("creating texture failed!\n");
		return -1;
	}

	//allocate mem for it
	pixels = (Uint32*)malloc(sizeof(Uint32) * screen_res_x * screen_res_y);	//Uint32[640 * 480];
	if (pixels == NULL) {
		printf("malloc pixels failed!\n");
		return -1;
	}

	//allocate mem for it
	pixels_background = (Uint32*)malloc(sizeof(Uint32) * screen_res_x * screen_res_y);	//Uint32[640 * 480];
	if (pixels_background == NULL) {
		printf("malloc pixels_background failed!\n");
		return -1;
	}
	else {
		static SDL_Color sky = { 70, 0, 0, 255 };		//blue
		static SDL_Color ground = { 0, 70, 30, 255 };		//green
		Uint32* u32mem = pixels_background;
		int i, cnt;

		cnt = screen_res_x * screen_res_y / 2;
		//sky
		for (i = 0; i < cnt; i++) {
			memcpy(u32mem++, &sky, sizeof(Uint32));
		}
		//ground
		for (i = 0; i < cnt; i++) {
			memcpy(u32mem++, &ground, sizeof(Uint32));
		}
	}


	return 0;
}


void sdl_stop(void) {

	//close texture stuff
	if (pixels)
		free(pixels);
	if (pixels_background)
		free(pixels_background);
	if (texture)
		SDL_DestroyTexture(texture);

	if (renderer) {
		SDL_DestroyRenderer(renderer);
	}
	if (window) {
		SDL_DestroyWindow(window);
	}

}

void setup_screen(void)
{
	//memset(pixels, 0, screen_res_x * screen_res_y * sizeof(Uint32));
	memcpy(pixels, pixels_background, screen_res_x * screen_res_y * sizeof(Uint32));
}

void display_screen(void) {
	SDL_RenderPresent(renderer);
}

int ttf_init_setup(char* font_file) {
	//set defaults
	ttf_Font = TTF_OpenFont(font_file, 24); //this opens a font style and sets a size
	ttf_FColor = (SDL_Color){ 255, 162, 0, SDL_ALPHA_OPAQUE };  // foreground color
	ttf_BColor = (SDL_Color){ 0, 0, 0, SDL_ALPHA_OPAQUE };  // background color
	if (ttf_Font == NULL) {
		return -1;
	}
	return 0;

}

void display_text(int screen_column, int screen_row, char* text2disp, SDL_Color color)
{
	//font sizes are fix
	int font_height = 15;
	int font_width = 10;

	//set render resolution for any text displayed
	if (get_conf_val(fullscr) == 1)
		SDL_RenderSetLogicalSize(renderer, screen_native_xres, screen_native_yres);
	else
		SDL_RenderSetLogicalSize(renderer, screen_res_window_x, screen_res_window_y);

	//printf("%s; font_height:%d, font_width:%d\n", __func__, font_height, font_width);


	SDL_Surface* surfaceMessage = TTF_RenderText_Shaded(ttf_Font, text2disp, color, ttf_BColor); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first

																								 //printf("surfaceMessage:%p\n", surfaceMessage);

	SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage); //now you can convert it into a texture

	SDL_Rect Message_rect; //create a rect
	Message_rect.x = screen_column * font_width;  //controls the rect's x coordinate 
	Message_rect.y = screen_row * font_height; // controls the rect's y coordinte
	Message_rect.w = strlen(text2disp) * font_width; // controls the width of the rect
	Message_rect.h = font_height; // controls the height of the rect

								  //Mind you that (0,0) is on the top left of the window/screen, 
								  // think a rect as the text's box, that way it would be very simple to understance

								  //Now since it's a texture, you have to put RenderCopy in your game loop area, the area where the whole code executes

								  //you put the renderer's name first, 
								  // the Message, 
								  // the crop size(you can ignore this if you don't want to dabble with cropping), and the rect which is the size and coordinate of your texture

	SDL_RenderCopy(renderer, Message, NULL, &Message_rect);
	//SDL_RenderCopy(renderer, Message, &Message_rect, NULL);

	//Don't forget too free your surface and texture
	SDL_DestroyTexture(Message);
	SDL_FreeSurface(surfaceMessage);


	//restore render resolution
	SDL_RenderSetLogicalSize(renderer, screen_res_x, screen_res_y);


}

