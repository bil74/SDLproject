#ifndef _LINUX_
#include <windows.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include "timer.h"
#include "sdlparts.h"

#define WINDOW_TITLE "sdl3d try"
extern SDL_Window* window = NULL;
extern SDL_Renderer* renderer = NULL;
int screen_res_x = 640;
int screen_res_y = 480;
//const char *font_file = { "C:\\Windows\\Fonts\\lucon.ttf" };
const char *font_file = { "Vera.ttf" };
//const char *font_file = { "lucon.ttf" };
TTF_Font *ttf_Font; //this opens a font style and sets a size
SDL_Color ttf_FColor;  // foreground color
SDL_Color ttf_BColor;  // background color


//local functions
int ttf_init_setup(char *font_fname);

int sdl_open(void) {
	//init sdl
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		printf(" SDL_Init failed: %s\n", SDL_GetError());
		return -1;
	}

	window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen_res_x, screen_res_y, SDL_WINDOW_FULLSCREEN_DESKTOP);
	//window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen_res_x, screen_res_y, 0);
	// Check that the window was successfully created
	if (window == NULL) {
		printf("Could not create window: %s\n", SDL_GetError());
		return -1;
	}

	//get desktop resolution for fullscreen mode
	SDL_GetWindowSize(window, &screen_res_x, &screen_res_y);


	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	//renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	// Check that the renderer was successfully created
	if (renderer == NULL) {
		printf("Could not create renderer: %s\n", SDL_GetError());
		return -1;
	}

	//init SDL TrueType fonts to write to screen
	if (TTF_Init() == -1)
	{
		printf("TTF_Init failed!\n");
		return -1;
	}
	if (ttf_init_setup((char *)font_file) == -1) {
	    printf("TTF load font \"%s\" failed!\n",(char *)font_file);
	    return -1;
	}
	return 0;
}


void sdl_close(void) {
	if (renderer) {
		SDL_DestroyRenderer(renderer);
	}
	if (window) {
		SDL_DestroyWindow(window);
	}
	TTF_CloseFont(ttf_Font);
	SDL_Quit();

}

void setup_screen(void) {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
}

void display_screen(void) {
	SDL_RenderPresent(renderer);
}

int ttf_init_setup(char *font_file) {
	//set defaults
	ttf_Font = TTF_OpenFont(font_file, 24); //this opens a font style and sets a size
	ttf_FColor = { 255, 162, 0, SDL_ALPHA_OPAQUE };  // foreground color
	ttf_BColor = { 0, 0, 0, SDL_ALPHA_OPAQUE };  // background color
	if (ttf_Font == NULL) {
	    return -1;
	}
	return 0;

}

void display_text(int screen_column, int screen_row, char *text2disp)
{
	const int font_width = 10;
	const int font_height = 15;

	SDL_Surface *surfaceMessage = TTF_RenderText_Shaded(ttf_Font, text2disp, ttf_FColor, ttf_BColor); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first

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
}
