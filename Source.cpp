using namespace std;

#include <iostream>
#include <math.h>
#include <time.h>
#include <windows.h>
#include <timer.h>
#include <SDL.h>
#include <SDL_TTF.h>

#define WINDOW_TITLE "sdl3d try"
#define POINTS_COUNT 4
#define G_MAXOFFSET 100
#define G_MINOFFSET -100
#define PI 3.14159265
#define CUBES 1000
#define TARGETFPS 60


//timers
__int64 timer_main = 0;
__int64 timer_refr_txt = 0;
__int64 timer_frame = 0;

typedef struct {
	signed int x;
	signed int y;
	signed int z;
}t_point3d;
t_point3d *p_point3d;

typedef struct {
	signed int x;
	signed int y;
	bool valid;
}t_point2d;
t_point2d *p_point2d;

typedef struct {
	t_point3d points[3];
	int color;
	int draw_it;
	//normális, stb
}t_poly3d;
t_poly3d *p_poly3d;

typedef struct {
	t_point2d points[3];
	int color;
	int draw_it;
	//normális, stb
}t_poly2d;
t_poly2d *p_poly2d;



//object v2 parts
typedef struct {
	int point_id[3];
	int color;
	int draw_it;
	//normális, stb
}t_poly;

//object v2
typedef struct {
	int num_points;
	int num_polys;
	t_point3d *points_3d;
	t_point2d *points_2d;
	t_poly *polys;
}t_obj;

//scene v2
typedef struct t_scene {
	//some global shit
	//light sources
	//3d segment
	//drawing type (wireframe, solid, textured)

	int num_objects;
	t_obj *objects;
} t_scene;

//cube object v2
t_point3d cube_points_3d[8] = { { -100, -100, -100 }, { -100, 100, -100 }, { 100, 100, -100 }, { 100, -100, -100 },  /**/  { -100, -100,  100 }, { -100, 100,  100 }, { 100, 100,  100 }, { 100, -100,  100 } };
t_point2d cube_points_2d[8] = {0};
t_poly cube_polyconns[2 * 6] = { { 0,1,2 ,0,1 }, { 2,3,0 ,0,1},   { 4,7,6 ,0,1}, { 6,5,4 ,0,1},   { 5,1,0 ,0,1}, { 0,4,5 ,0,1},   { 3,2,6 ,0,1}, { 6,7,3 ,0,1},    { 7,4,0 ,0,1}, { 0,3,7 ,0,1},    { 1,4,5 ,0,1}, { 5,2,1 ,0,1} };

t_obj objs_v2[CUBES] = {
{8, 2 * 6, cube_points_3d, cube_points_2d, cube_polyconns}
};

t_scene scene = {
	CUBES,
	objs_v2
};


t_poly3d cube3d[2 * 6 * CUBES] = {
{ {{-100, 100, 500},{ 100, 100, 500 },{ -100, -100, 500 }}, 0, 1},	//poly1
{ {{ 100, 100, 500 },{ 100, -100, 500 },{ -100, -100, 500 }}, 0, 1 },	//poly2
{ {{ -100, 100, 300 },{ 100, 100, 300 },{ -100, -100, 300 }}, 0, 1 },	//poly3
{ {{ 100, 100, 300 },{ 100, -100, 300 },{ -100, -100, 300 }}, 0, 1 },	//poly4

{ {{ -100, 100, 500 },{ -100, 100, 300 },{ -100, -100, 300 }}, 0, 1 },	//poly5
{ {{ -100, 100, 500 },{ -100, -100, 500 },{ -100, -100, 300 }}, 0, 1 },	//poly6
{ {{ 100, 100, 500 },{ 100, 100, 300 },{ 100, -100, 300 }}, 0, 1 },	//poly7
{ {{ 100, 100, 500 },{ 100, -100, 500 },{ 100, -100, 300 }}, 0, 1 },	//poly8

{ {{ -100, 100, 300 },{ -100, 100, 500 },{ 100, 100, 500 }}, 0, 1 },	//poly9
{ {{ -100, 100, 300 },{ 100, 100, 300 },{ 100, 100, 500 }}, 0, 1 },	//poly10
{ {{ -100, -100, 300 },{ -100, -100, 500 },{ 100, -100, 500 }}, 0, 1 },	//poly11
{ {{ -100, -100, 300 },{ 100, -100, 300 },{ 100, -100, 500 }}, 0, 1 }	//poly12



};

t_poly3d cube3d_1[2 * 6 * CUBES] = {0};

t_poly2d cube2d[2 * 6 * CUBES] = { 0 };		//work on this magic number!

static SDL_Point points_orig[POINTS_COUNT] = {
	{ 320, 200 },
	{ 300, 240 },
	{ 340, 240 },
	{ 320, 200 }
};

SDL_Point points[POINTS_COUNT] = {
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 }
};

int sizeof_segment3d = 16000;	//mondjuk

typedef struct {
	double x;
	double y;
	double z;
	double roty;//-180 -> +180 ?
	double rotx;
	double rotz;
	double curr_segment_3d;		//felosztjuk a teret kockákra és csak az akuális plusz szomszédos szegmenseket rajzoljuk meg
}t_camera;

t_camera camera = {
	0,0,-500,0,0,0
};


int g_offset_x = 0;
int g_offset_y = 0;

int init_some_stuff(void);
void close_some_stuff(void);
void create_lot_cubes(void);
void render_scene(void);
void apply_camera(void);
void transform3d_to_2d(void);
void draw2d(void);

int create_lot_cubes_v2(void);
void render_scene_v2(t_scene *scene);
void do_3d_to_2d(t_scene *scene);
void draw2d_v2(t_scene *scene);

void display_text(int screen_x, int screen_y, char *text2disp);
unsigned long long getNanotime(void);
unsigned long long GetTimeULL();
void movement(void);
void move_camera(void);
void getkeys(void);



SDL_bool done_global = SDL_FALSE;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
static int screen_res_x = 640;
static int screen_res_y = 480;
int objects_to_draw = 1;		//must be set during loading scene

int scene_size_bytes;
void *p_scene_3d_orig;
void *p_scene_3d_conv;


typedef struct {
	int draw_it;	//visible or not
	int relative;	//0: a koordináták abszolutak, 0: a poly koordinátákhoz hozzá kell adni az obj koordinátákat (+forgatás)
	//int segment_3d;	//ez x,y,z koordináta lesz (pl. 10, 10, 10) es csak 9 db. szegmenst veszünk figyelembe renderelésnél
	int obj_x;
	int obj_y;
	int obj_z;
	int obj_rot_x;
	int obj_rot_y;
	int obj_rot_z;
	int num_of_polys;
	//polys here
}t_object_3d_header;
t_object_3d_header *p_object_3d_header;

t_object_3d_header cube_object_3d_header {
	1,	//must be drawn
	0,	//obj coords are absolute - static object

	0,	//coords
	0,
	0,
	0,	//rotation
	0,
	0,
	12	//num of polys
};

typedef struct {
	int num_of_objects;
	//objects here
}scene_segment_3d;



int main(int argc, char* argv[])
{
	if (init_some_stuff() != 0) {
		printf("init failed!\n");
		return -1;
	}
		

			SDL_bool done = SDL_FALSE;

			
			while (!done) 
			{
				//SDL_Event event;



				SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
				SDL_RenderClear(renderer);


				SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
				//draw to the screen

				render_scene_v2(&scene);
				//render_scene();
				{
					//aiming for 60 fps e! - TODO: wait for vsync
					/*
					while (GetCounter(&timer_main) < (1000/ (TARGETFPS + 5)))
						Sleep(1);
					*/

					//display the screen
					movement();
					done = done_global;
					SDL_RenderPresent(renderer);
					StartCounter(&timer_main);
				}
				//break;




			}
			close_some_stuff();

	return 0;
}

void getkeys(void) {
	//fill keytable
	static bool akarmi;
	enum keys {
		key_up,
		key_down,
		key_left,
		key_right
	};

}
void movement(void) {
	move_camera();
}

void move_camera(void) {
#define MILLIS_PER_SEC 1000.00
	static __int64 timer_move;
	const double speed_cam_move = 600;			//pixel per sec
	const double speed_cam_rot = 90;			//degree per sec
	double frametime;							//
	double val;
	val = PI / 180;
	const Uint8 *keystates = SDL_GetKeyboardState(NULL);	//get key states

	frametime = GetCounter(&timer_move) / MILLIS_PER_SEC;
	getkeys();	//later

	SDL_PumpEvents();	//actualize events

	if (keystates[SDL_SCANCODE_LEFT]) {
		camera.roty -= frametime * speed_cam_rot;	//% 360
		if (camera.roty < 0)
			camera.roty += 360;
	}
	if (keystates[SDL_SCANCODE_RIGHT]) {
		camera.roty += frametime * speed_cam_rot;	//% 360
		if (camera.roty > 360)
			camera.roty -= 360;
	}
	if (keystates[SDL_SCANCODE_UP]) {
		camera.z += (cos(camera.roty*val) * (frametime*speed_cam_move));
		camera.x += (sin(camera.roty*val) * (frametime*speed_cam_move));
	}
	if (keystates[SDL_SCANCODE_DOWN]) {
		camera.z -= (cos(camera.roty*val) * (frametime*speed_cam_move));
		camera.x -= (sin(camera.roty*val) * (frametime*speed_cam_move));
	}
	if (keystates[SDL_SCANCODE_ESCAPE])
		done_global = SDL_TRUE;

		StartCounter(&timer_move);


}


void create_lot_cubes(void)
{
	#define BLOCKSIZE 10		//10^3 cubes
	#define CUBESIDE 200
	#define CUBEPOLYS (2 * 6)
	#define DISTANCE CUBESIDE
	t_poly3d *ppoly;
	int i,j,k;


		for (k = 1, ppoly = cube3d; k < BLOCKSIZE; k++) {	//1 row of cubes
			for (i = 0; i < CUBEPOLYS; i++) {	//one cube copy x direction
				for (j = 0; j < 3; j++) {
					ppoly[i + CUBEPOLYS*k].points[j].x = ppoly[i].points[j].x + (CUBESIDE + DISTANCE)*k;
					ppoly[i + CUBEPOLYS*k].points[j].y = ppoly[i].points[j].y;
					ppoly[i + CUBEPOLYS*k].points[j].z = ppoly[i].points[j].z;
				}
			}
		}

		for (k = 1, ppoly = cube3d; k < BLOCKSIZE; k++) {	//1 row of cubes
			for (i = 0; i < (CUBEPOLYS*BLOCKSIZE); i++) {	//one cube copy x direction
				for (j = 0; j < 3; j++) {
					ppoly[i + (CUBEPOLYS*BLOCKSIZE) * k].points[j].x = ppoly[i].points[j].x;
					ppoly[i + (CUBEPOLYS*BLOCKSIZE) * k].points[j].y = ppoly[i].points[j].y + (CUBESIDE + DISTANCE)*k;
					ppoly[i + (CUBEPOLYS*BLOCKSIZE) * k].points[j].z = ppoly[i].points[j].z;
				}
			}
		}
		
		for (k = 1, ppoly = cube3d; k < BLOCKSIZE; k++) {	//1 row of cubes
			for (i = 0; i < (CUBEPOLYS*BLOCKSIZE*BLOCKSIZE); i++) {	//one cube copy x direction
				for (j = 0; j < 3; j++) {
					ppoly[i + (CUBEPOLYS*BLOCKSIZE*BLOCKSIZE) * k].points[j].x = ppoly[i].points[j].x;
					ppoly[i + (CUBEPOLYS*BLOCKSIZE*BLOCKSIZE) * k].points[j].y = ppoly[i].points[j].y;
					ppoly[i + (CUBEPOLYS*BLOCKSIZE*BLOCKSIZE) * k].points[j].z = ppoly[i].points[j].z + (CUBESIDE + DISTANCE)*k;
				}
			}
		}

	//t_poly3d cube3d[2 * 6 * CUBES] = {
}

int init_some_stuff(void)
{
	//init sdl
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		printf(__FUNCTION__ " SDL_Init failed: %s\n", SDL_GetError());
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

	//random number seed
	srand(time(NULL));

	//create_lot_cubes();
	if (create_lot_cubes_v2() == -1)
		return -1;

	//init timer
	InitTimer();
	StartCounter(&timer_frame);
	StartCounter(&timer_main);

	//init SDL TrueType fonts to write to screen
	if (TTF_Init() == -1)
	{
		printf("TTF_Init failed!\n");
		return -1;
	}
	

	//load scene - NOT YET!!

	//calculate needed memory for scene
	//scene_size_bytes = ...calculated above
	scene_size_bytes = sizeof(p_object_3d_header) + sizeof(cube3d);		//temporary solution

	//allocate memory for orig scene
	p_scene_3d_orig = malloc(scene_size_bytes);
	if (p_scene_3d_orig == NULL) {
		return -1;
	}

	//allocate memory for camera-applied scene
	p_scene_3d_conv = malloc(scene_size_bytes);
	if (p_scene_3d_conv == NULL) {
		return -1;
	}

	//set p_object_3d_header to cube_object_3d_header
	p_object_3d_header = &cube_object_3d_header;

	
	//set to p_poly3d to cube3d - not needed
	//p_poly3d = cube3d;
	
	return 0;
}

void close_some_stuff(void)
{
	free(p_scene_3d_orig);
	free(p_scene_3d_conv);


	if (renderer) {
		SDL_DestroyRenderer(renderer);
	}
	if (window) {
		SDL_DestroyWindow(window);
	}
	SDL_Quit();

}

void render_scene(void)
{
	#define DSPTIME 1000						//refresh interval for frametime (millisec)
	static double frametime, sleeptime;
	char text2print[100];
	static int num_of_frames;



	//render cycle start
	apply_camera();
	transform3d_to_2d();
	draw2d();
	//render cycle end

	num_of_frames++;
	//refresh frametime value
	if (GetCounter(&timer_refr_txt) > DSPTIME) {
		frametime = GetCounter(&timer_frame) / num_of_frames;
		StartCounter(&timer_frame);
		num_of_frames = 0;
		StartCounter(&timer_refr_txt);
	}
	
	//print some stuff to the screen
	//camera data
	sprintf_s(text2print, sizeof(text2print), "cam x=%.0f, y=%.0f, z=%.0f, rot-x:%.0f, rot-y:%.0f, rot-z:%.0f", camera.x, camera.y, camera.z, camera.rotx, camera.roty, camera.rotz);
	display_text(0, 0, text2print);
	//fps
	//sprintf_s(text2print, sizeof(text2print), "fps %d", (int)roundf((float)(1000.00 / frametime)));
	sprintf_s(text2print, sizeof(text2print), "fps %.2f", (float)(1000.00 / frametime));
	display_text(0, 15, text2print);
}

void apply_camera(void)
{


	//camera rotation is not yet used
	int i, j;
	double val;
	val = PI / 180;

	//memcpy(cube3d_1, cube3d, sizeof(cube3d));

	//printf("apply_camera --- camera.x=%d, camera.y=%d, camera.z=%d\n", camera.x, camera.y, camera.z);
	//printf("apply_camera --- camera.rotx=%d, camera.roty=%d, camera.rotz=%d\n", camera.rotx, camera.roty, camera.rotz);
	for (i = 0; i < sizeof(cube3d) / sizeof(t_poly3d); i++) {
		cube3d_1[i].draw_it = 1;
		for (j = 0; j < sizeof(cube3d[j].points) / sizeof(t_point3d); j++) {
			int x, y, z, tx, ty, tz;	//temporary values

			//apply camera position
			x = cube3d_1[i].points[j].x = cube3d[i].points[j].x - (int)camera.x;
			y = cube3d_1[i].points[j].y = cube3d[i].points[j].y - (int)camera.y;
			z = cube3d_1[i].points[j].z = cube3d[i].points[j].z - (int)camera.z;

			
			//apply camera rotation
			//y axis
			tx = x; tz = z;
			x = (int)((cos(camera.roty*val) * tx) - (sin(camera.roty*val) * tz));
			z = (int)((cos(camera.roty*val) * tz) + (sin(camera.roty*val) * tx));

			//x axis
			ty = y; tz = z;
			z = (int)((cos(camera.rotx*val) * tz) + (sin(camera.rotx*val) * ty));	//talan -
			y = (int)((cos(camera.rotx*val) * ty) - (sin(camera.rotx*val) * tz));	//talan +

			//z axis
			ty = y; tx = x;
			x = (int)((cos(camera.rotz*val) * tx) + (sin(camera.rotz*val) * ty));	//talan -
			y = (int)((cos(camera.rotz*val) * ty) - (sin(camera.rotz*val) * tx));	//talan +



			if (z <= 0)
				cube3d_1[i].draw_it = 0;
				
			cube3d_1[i].points[j].x = x;
			cube3d_1[i].points[j].y = y;
			cube3d_1[i].points[j].z = z;

		}

	}

}

void transform3d_to_2d(void)
{
	const int persp_value = 500;
	int i,j;

	for (i = 0; i < sizeof(cube3d_1) / sizeof(t_poly3d); i++) {

		cube2d[i].draw_it = 1;
		if (cube3d_1[i].draw_it == 0) {
			cube2d[i].draw_it = 0;
			continue;
		}

		for (j = 0; j < sizeof(cube3d_1[j].points) / sizeof(t_point3d); j++) {
			//int ztemp, xytemp;

			/*
			int div0 = 0;
			if ((ztemp = (cube3d_1[i].points[j].z)) == 0)
				div0 = 1;
			*/
			//ztemp = cube3d_1[i].points[j].z;

			/*
			xytemp = (int)sqrt(abs(cube3d[i].points[j].x - camera.x) + abs(cube3d[i].points[j].y - camera.y));
			cube2d[i].points[j].x = ((cube3d[i].points[j].x - camera.x) * persp_value) / (ztemp + (xytemp * 5));
			cube2d[i].points[j].y = ((cube3d[i].points[j].y - camera.y) * persp_value) / (ztemp + (xytemp * 5));
			*/
			
			cube2d[i].points[j].x = ((cube3d_1[i].points[j].x) * persp_value) / cube3d_1[i].points[j].z;
			cube2d[i].points[j].y = ((cube3d_1[i].points[j].y) * persp_value) / cube3d_1[i].points[j].z;
			}
		}
		
}

void draw2d(void)
{
	SDL_Point points_tmp[3+1] = { 0	};
	int i, j;
	int out_of_screen_points;


	for (i = 0; i < sizeof(cube2d) / sizeof(t_poly2d); i++) {
		out_of_screen_points = 0;
		
		if (cube2d[i].draw_it == 0)
			continue;
		
		for (j = 0; j < sizeof(cube2d[j].points) / sizeof(t_point2d); j++) {
			points_tmp[j].x = cube2d[i].points[j].x + (screen_res_x / 2);
			points_tmp[j].y = -cube2d[i].points[j].y + (screen_res_y / 2);		//-: sdl y has positive downwards
			
			if (points_tmp[j].x < 0 || points_tmp[j].x > screen_res_x)
				out_of_screen_points++;
			else if (points_tmp[j].y < 0 || points_tmp[j].y > screen_res_y)
				out_of_screen_points++;

			
		}
		//dont draw triangle if all points are outside of the window

		if (out_of_screen_points < 3) {
			points_tmp[3].x = points_tmp[0].x;
			points_tmp[3].y = points_tmp[0].y;
			SDL_RenderDrawLines(renderer, points_tmp, 3 + 1);
		}
	}



}

void display_text(int screen_x, int screen_y, char *text2disp)
{

	TTF_Font *Font = TTF_OpenFont("C:\\Windows\\Fonts\\lucon.ttf", 24); //this opens a font style and sets a size

	//printf("font pointer:%p\n", Sans);

	SDL_Color Color = { 155, 255, 055, SDL_ALPHA_OPAQUE };  // this is the color in rgb format, maxing out all would give you the color white, and it will be your text's color


	SDL_Surface *surfaceMessage = TTF_RenderText_Solid(Font, text2disp, Color); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first

	//printf("surfaceMessage:%p\n", surfaceMessage);

	SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage); //now you can convert it into a texture

	SDL_Rect Message_rect; //create a rect
	Message_rect.x = screen_x;  //controls the rect's x coordinate 
	Message_rect.y = screen_y; // controls the rect's y coordinte
	Message_rect.w = strlen(text2disp) * 10; // controls the width of the rect
	Message_rect.h = 15; // controls the height of the rect

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
	TTF_CloseFont (Font);
}

/**
 * Returns the current time in microseconds.
 */
unsigned long long getNanotime() {
	FILETIME ft;
	GetSystemTimeAsFileTime(&ft);
	unsigned long long tt = ft.dwLowDateTime;
	/*
	unsigned long long tt = ft.dwHighDateTime;
	tt <<= 32;
	tt |= ft.dwLowDateTime;
	tt /= 10;
	tt -= 11644473600000000ULL;
	*/
	return tt/10;
}

unsigned long long GetTimeULL()
{
	/* Windows */
	FILETIME ft;
	LARGE_INTEGER li;

	/* Get the amount of 100 nano seconds intervals elapsed since January 1, 1601 (UTC) and copy it
	 * to a LARGE_INTEGER structure. */
	GetSystemTimeAsFileTime(&ft);
	li.LowPart = ft.dwLowDateTime;
	li.HighPart = ft.dwHighDateTime;

	unsigned long long ret = li.QuadPart;
	ret -= 116444736000000000LL; /* Convert from file time to UNIX epoch time. */
	ret /= 10; /* From 100 nano seconds (10^-7) to 1 millisecond (10^-3) intervals */

	return ret;
}





//v2 stuff



void render_scene_v2(t_scene *scene_rsv2)
{
#define DSPTIME 1000						//refresh interval for frametime (millisec)
	static double frametime, sleeptime;
	char text2print[100];
	static int num_of_frames;



	//render cycle start
	do_3d_to_2d(scene_rsv2);
	draw2d_v2(scene_rsv2);
	//render cycle end

	num_of_frames++;
	//refresh frametime value
	if (GetCounter(&timer_refr_txt) > DSPTIME) {
		frametime = GetCounter(&timer_frame) / num_of_frames;
		StartCounter(&timer_frame);
		num_of_frames = 0;
		StartCounter(&timer_refr_txt);
	}

	//print some stuff to the screen
	//camera data
	sprintf_s(text2print, sizeof(text2print), "cam x=%.0f, y=%.0f, z=%.0f, rot-x:%.0f, rot-y:%.0f, rot-z:%.0f", camera.x, camera.y, camera.z, camera.rotx, camera.roty, camera.rotz);
	display_text(0, 0, text2print);
	//fps
	//sprintf_s(text2print, sizeof(text2print), "fps %d", (int)roundf((float)(1000.00 / frametime)));
	sprintf_s(text2print, sizeof(text2print), "fps %.2f", (float)(1000.00 / frametime));
	display_text(0, 15, text2print);
}




void do_3d_to_2d(t_scene *scene_3to2v2) {

	int i, j;
	double val;
	val = PI / 180;
	const int persp_value = 500;

	
	//1 object
	for (i = 0; i < scene_3to2v2->num_objects; i++) {
		for (j = 0; j < scene_3to2v2->objects[i].num_points; j++) {
			int x, y, z, tx, ty, tz;	//temporary values

			//apply camera position
			x = scene_3to2v2->objects[i].points_3d[j].x - (int)camera.x;
			y = scene_3to2v2->objects[i].points_3d[j].y - (int)camera.y;
			z = scene_3to2v2->objects[i].points_3d[j].z - (int)camera.z;
			//printf("do_3d_to_2d x:%d, y:%d, z:%d", x, y, z);
			/*
			x = cube3d_1[i].points[j].x = cube3d[i].points[j].x - (int)camera.x;
			y = cube3d_1[i].points[j].y = cube3d[i].points[j].y - (int)camera.y;
			z = cube3d_1[i].points[j].z = cube3d[i].points[j].z - (int)camera.z;
			*/

			//apply camera rotation
			//y axis
			tx = x; tz = z;
			x = (int)((cos(camera.roty*val) * tx) - (sin(camera.roty*val) * tz));
			z = (int)((cos(camera.roty*val) * tz) + (sin(camera.roty*val) * tx));
			//x axis
			ty = y; tz = z;
			z = (int)((cos(camera.rotx*val) * tz) - (sin(camera.rotx*val) * ty));	//talan -
			y = (int)((cos(camera.rotx*val) * ty) + (sin(camera.rotx*val) * tz));	//talan +
			//z axis
			ty = y; tx = x;
			x = (int)((cos(camera.rotz*val) * tx) - (sin(camera.rotz*val) * ty));	//talan -
			y = (int)((cos(camera.rotz*val) * ty) + (sin(camera.rotz*val) * tx));	//talan +
			//printf("; trans x:%d, y:%d, z:%d", x, y, z);

			//transform 3d to 2d

			if (z <= 0)
				scene_3to2v2->objects[i].points_2d[j].valid = false;
			else {
				//transform 3d to 2d
				scene_3to2v2->objects[i].points_2d[j].valid = true;
				scene_3to2v2->objects[i].points_2d[j].x = (x * persp_value) / z;
				scene_3to2v2->objects[i].points_2d[j].y = (y * persp_value) / z;
				//printf("; 2d x:%d, y:%d (z:%d)\n", scene_3to2v2.objects[0].points_2d[j].x, scene_3to2v2.objects[0].points_2d[j].y, z);

			}

			/*
			if (z <= 0)
				cube2d[i].draw_it = 0;
			else {
				//transform 3d to 2d
				cube2d[i].draw_it = 1;
				cube2d[i].points[j].x = (x * persp_value) / z;
				cube2d[i].points[j].y = (y * persp_value) / z;

			}
			*/
		}	

	}

}





void draw2d_v2(t_scene *scene_d2v2)
{
	SDL_Point points_tmp[3 + 1] = { 0 };
	int i, j, k;
	int out_of_screen_points;

	//scene_3to2v2.objects->points_2d[j].x

	//printf("draw2d_v2 scene_d2v2.objects[0].num_polys:%d\n", scene_d2v2.objects[0].num_polys);
	for (k = 0; k < scene_d2v2->num_objects; k++) {
		for (i = 0; i < scene_d2v2->objects[k].num_polys; i++) {
			out_of_screen_points = 0;

			/*
			if (cube2d[i].draw_it == 0)
				continue;
			*/
			int p;
			//for (j = 0; j < sizeof(cube2d[j].points) / sizeof(t_point2d); j++) {
			for (j = 0; j < 3; j++) {
				p = scene_d2v2->objects[k].polys[i].point_id[j];
				//printf("draw2d_v2 p:%d, ",p);
				if (scene_d2v2->objects[k].points_2d[p].valid == true) {
					points_tmp[j].x = scene_d2v2->objects[k].points_2d[p].x + (screen_res_x / 2);
					points_tmp[j].y = -(scene_d2v2->objects[k].points_2d[p].y) + (screen_res_y / 2);		//-: sdl y has positive downwards;
					//printf("draw2d_v2 p%dx:%d, p%dy:%d; ", p, points_tmp[j].x, p, points_tmp[j].y);
					if (points_tmp[j].x < 0 || points_tmp[j].x > screen_res_x)
						out_of_screen_points++;
					else if (points_tmp[j].y < 0 || points_tmp[j].y > screen_res_y)
						out_of_screen_points++;
				}
				else {
					out_of_screen_points += 3;
				}
				/*
				points_tmp[j].x = cube2d[i].points[j].x + (screen_res_x / 2);
				points_tmp[j].y = -cube2d[i].points[j].y + (screen_res_y / 2);		//-: sdl y has positive downwards
				*/



			}
			//dont draw triangle if all points are outside of the window

			if (out_of_screen_points < 3) {
				points_tmp[3].x = points_tmp[0].x;
				points_tmp[3].y = points_tmp[0].y;
				SDL_RenderDrawLines(renderer, points_tmp, 3 + 1);
				//printf("draw2d_v2 p3x:%d, p3y:%d; ", points_tmp[3].x, points_tmp[3].y);
			}
			//printf("\n");
		}
	}



}
int create_lot_cubes_v2(void)
{
#define BLOCKSIZE 10		//10^3 cubes
#define CUBESIDE 200
#define CUBEPOLYS (2 * 6)
#define DISTANCE CUBESIDE
	t_poly3d *ppoly;
	int i, j, k;

	//v2
	t_obj *p_objs_v2;
	t_point3d *p_cube_points_3d;
	t_point2d *p_cube_points_2d;
	//t_poly *cube_polyconns;
	int num_objects = 1;
	int rand_x, rand_y, rand_z;
	#define MAXDISTANCE (BLOCKSIZE * DISTANCE * 2)

	//randomnumber = rand() % MAXDISTANCE;
	for (k = 1; k < CUBES; k++) {
		objs_v2[k].num_points = 8;
		objs_v2[k].num_polys = 12;
		objs_v2[k].polys = cube_polyconns;	//global stuff
		objs_v2[k].points_2d = cube_points_2d;
		p_cube_points_3d = (t_point3d *)malloc(8 * sizeof(t_point3d));
		p_cube_points_2d = (t_point2d *)malloc(8 * sizeof(t_point2d));
		if (p_cube_points_3d == NULL || p_cube_points_2d == NULL)
			return -1;
		objs_v2[k].points_2d = p_cube_points_2d;
		objs_v2[k].points_3d = p_cube_points_3d;
		rand_x = rand() % MAXDISTANCE;
		rand_y = rand() % MAXDISTANCE;
		rand_z = rand() % MAXDISTANCE;
		for (i = 0; i < 8; i++) {
			objs_v2[k].points_3d[i].x = objs_v2[0].points_3d[i].x + rand_x;
			objs_v2[k].points_3d[i].y = objs_v2[0].points_3d[i].y + rand_y;
			objs_v2[k].points_3d[i].z = objs_v2[0].points_3d[i].z + rand_z;
		}
	}

	return 0;
}
