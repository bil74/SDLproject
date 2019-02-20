#ifndef _LINUX_
using namespace std;
#include <iostream>
#include <windows.h>
#include <SDL.h>
#else
#include <unistd.h>
//#include <SDL/SDL.h>
#include <inttypes.h>
#endif
#include <math.h>
#include <time.h>
#include "timer.h"
#include "sdlparts.h"
#include "Source.h"


#define PI 3.14159265
#define OBJS_V2 100

extern int screen_res_x;
extern int screen_res_y;
extern Uint32 *pixels;		//texture mem
extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern SDL_Texture *texture;

//timers
INT64 timer_main = 0;
INT64 timer_refr_txt = 0;
INT64 timer_frame = 0;



// ----------------------------------------------------- videoship object v2
t_point3d obj_points_3d[55] = {
	{ 100  ,    -100  ,    -100 },
	{ 100  ,    100   ,    -100 },
	{ 100  ,    -100  ,    100 },
	{ 100  ,    100   ,    100 },
	{ -100 ,    -100  ,    -100 },
	{ -100 ,    100   ,    -100 },
	{ -100 ,    -100  ,    100 },
	{ -100 ,    100   ,    100 },
	{ -72  ,    12    ,    -140 },
	{ 30   ,    0     ,    500 },
	{ -60  ,    -60   ,    -140 },
	{ -30  ,    0     ,    500 },
	{ -120 ,    20    ,    100 },
	{ -60  ,    60    ,    -140 },
	{ -120 ,    -20   ,    -100 },
	{ -120 ,    20    ,    -100 },
	{ 120  ,    -20   ,    100 },
	{ 120  ,    -20   ,    -100 },
	{ 120  ,    20    ,    -100 },
	{ 120  ,    20    ,    100 },
	{ -120 ,    -20   ,    100 },
	{ 60   ,    60    ,    -140 },
	{ 60   ,    -60   ,    -140 },
	{ -420 ,    6     ,    100 },
	{ -420 ,    -6    ,    100 },
	{ -420 ,    -6    ,    -100 },
	{ -420 ,    6     ,    -100 },
	{ 420  ,    -6    ,    100 },
	{ 420  ,    -6    ,    -100 },
	{ 420  ,    6     ,    -100 },
	{ 420  ,    6     ,    100 },
	{ 420  ,    -18   ,    100 },
	{ 420  ,    -18   ,    -100 },
	{ 420  ,    18    ,    -100 },
	{ 420  ,    18    ,    100 },
	{ 450  ,    -18   ,    100 },
	{ 450  ,    -18   ,    -100 },
	{ 450  ,    18    ,    -100 },
	{ 450  ,    18    ,    100 },
	{ -420 ,    18    ,    100 },
	{ -420 ,    -18   ,    100 },
	{ -420 ,    -18   ,    -100 },
	{ -420 ,    18    ,    -100 },
	{ -450 ,    18    ,    100 },
	{ -450 ,    -18   ,    100 },
	{ -450 ,    -18   ,    -100 },
	{ -450 ,    18    ,    -100 },
	{ 435  ,    -18   ,    300 },
	{ 435  ,    18    ,    300 },
	{ -435 ,    18    ,    300 },
	{ -435 ,    -18   ,    300 },
	{ 0    ,    -70   ,    300 },
	{ -72  ,    -12   ,    -140 },
	{ 72   ,    -12   ,    -140 },
	{ 72   ,    12    ,    -140 }

};

t_point2d_tf obj_points_2d[55] = { 0 };

t_poly obj_polys[98] = {
	{ 20, 51, 11  ,{ 255,255,255,0 },0,{ 0,0,0 } },
	{ 5 , 12, 7   ,{ 255,255,255,0 },0,{ 0,0,0 } },
	{ 4 , 22, 0   ,{ 255,255,255,0 },0,{ 0,0,0 } },
	{ 6 , 0 , 2   ,{ 255,255,255,0 },0,{ 0,0,0 } },
	{ 3 , 5 , 7   ,{ 255,255,255,0 },0,{ 0,0,0 } },
	{ 3 , 11, 9   ,{ 255,255,255,0 },0,{ 0,0,0 } },
	{ 16, 19, 9   ,{ 255,255,255,0 },0,{ 0,0,0 } },
	{ 19, 3 , 9   ,{ 255,255,255,0 },0,{ 0,0,0 } },
	{ 16, 51, 2   ,{ 255,255,255,0 },0,{ 0,0,0 } },
	{ 6 , 2 , 51  ,{ 255,255,255,0 },0,{ 0,0,0 } },
	{ 15, 13, 8   ,{ 255,255,255,0 },0,{ 0,0,0 } },
	{ 6 , 14, 4   ,{ 255,255,255,0 },0,{ 0,0,0 } },
	{ 19, 29, 18  ,{ 255,255,255,0 },0,{ 0,0,0 } },
	{ 17, 22, 53  ,{ 255,255,255,0 },0,{ 0,0,0 } },
	{ 3 , 18, 1   ,{ 255,255,255,0 },0,{ 0,0,0 } },
	{ 0 , 16, 2   ,{ 255,255,255,0 },0,{ 0,0,0 } },
	{ 12, 24, 20  ,{ 255,255,255,0 },0,{ 0,0,0 } },
	{ 12, 20, 11  ,{ 255,255,255,0 },0,{ 0,0,0 } },
	{ 11, 51, 9   ,{ 255,255,255,0 },0,{ 0,0,0 } },
	{ 7 , 12, 11  ,{ 255,255,255,0 },0,{ 0,0,0 } },
	{ 14, 26, 15  ,{ 255,255,255,0 },0,{ 0,0,0 } },
	{ 20, 25, 14  ,{ 255,255,255,0 },0,{ 0,0,0 } },
	{ 15, 23, 12  ,{ 255,255,255,0 },0,{ 0,0,0 } },
	{ 30, 33, 29  ,{ 255,255,255,0 },0,{ 0,0,0 } },
	{ 17, 27, 16  ,{ 255,255,255,0 },0,{ 0,0,0 } },
	{ 16, 30, 19  ,{ 255,255,255,0 },0,{ 0,0,0 } },
	{ 18, 28, 17  ,{ 255,255,255,0 },0,{ 0,0,0 } },
	{ 31, 48, 34  ,{ 255,255,255,0 },0,{ 0,0,0 } },
	{ 28, 31, 27  ,{ 255,255,255,0 },0,{ 0,0,0 } },
	{ 37, 35, 36  ,{ 255,255,255,0 },0,{ 0,0,0 } },
	{ 33, 36, 32  ,{ 255,255,255,0 },0,{ 0,0,0 } },
	{ 34, 37, 33  ,{ 255,255,255,0 },0,{ 0,0,0 } },
	{ 32, 35, 31  ,{ 255,255,255,0 },0,{ 0,0,0 } },
	{ 42, 43, 39  ,{ 255,255,255,0 },0,{ 0,0,0 } },
	{ 24, 41, 25  ,{ 255,255,255,0 },0,{ 0,0,0 } },
	{ 26, 39, 23  ,{ 255,255,255,0 },0,{ 0,0,0 } },
	{ 43, 45, 44  ,{ 255,255,255,0 },0,{ 0,0,0 } },
	{ 39, 43, 49  ,{ 255,255,255,0 },0,{ 0,0,0 } },
	{ 41, 46, 42  ,{ 255,255,255,0 },0,{ 0,0,0 } },
	{ 40, 45, 41  ,{ 255,255,255,0 },0,{ 0,0,0 } },
	{ 43, 46, 45  ,{ 255,255,255,0 },0,{ 0,0,0 } },
	{ 31, 35, 47  ,{ 255,255,255,0 },0,{ 0,0,0 } },
	{ 38, 34, 48  ,{ 255,255,255,0 },0,{ 0,0,0 } },
	{ 38, 47, 35  ,{ 255,255,255,0 },0,{ 0,0,0 } },
	{ 44, 50, 49  ,{ 255,255,255,0 },0,{ 0,0,0 } },
	{ 39, 50, 40  ,{ 255,255,255,0 },0,{ 0,0,0 } },
	{ 44, 40, 50  ,{ 255,255,255,0 },0,{ 0,0,0 } },
	{ 44, 49, 43  ,{ 255,255,255,0 },0,{ 0,0,0 } },
	{ 17, 28, 27  ,{ 255,255,255,0 },0,{ 0,0,0 } },
	{ 16, 27, 30  ,{ 255,255,255,0 },0,{ 0,0,0 } },
	{ 3 , 1 , 5   ,{ 255,255,255,0 },0,{ 0,0,0 } },
	{ 17, 54, 18  ,{ 255,255,255,0 },0,{ 0,0,0 } },
	{ 14, 10, 4   ,{ 255,255,255,0 },0,{ 0,0,0 } },
	{ 18, 21, 1   ,{ 255,255,255,0 },0,{ 0,0,0 } },
	{ 1 , 13, 5   ,{ 255,255,255,0 },0,{ 0,0,0 } },
	{ 15, 52, 14  ,{ 255,255,255,0 },0,{ 0,0,0 } },
	{ 52, 8 , 53  ,{ 255,255,255,0 },0,{ 0,0,0 } },
	{ 18, 29, 28  ,{ 255,255,255,0 },0,{ 0,0,0 } },
	{ 14, 25, 26  ,{ 255,255,255,0 },0,{ 0,0,0 } },
	{ 15, 26, 23  ,{ 255,255,255,0 },0,{ 0,0,0 } },
	{ 12, 23, 24  ,{ 255,255,255,0 },0,{ 0,0,0 } },
	{ 20, 24, 25  ,{ 255,255,255,0 },0,{ 0,0,0 } },
	{ 6 , 20, 14  ,{ 255,255,255,0 },0,{ 0,0,0 } },
	{ 6 , 4 , 0   ,{ 255,255,255,0 },0,{ 0,0,0 } },
	{ 20, 6 , 51  ,{ 255,255,255,0 },0,{ 0,0,0 } },
	{ 0 , 17, 16  ,{ 255,255,255,0 },0,{ 0,0,0 } },
	{ 16, 9 , 51  ,{ 255,255,255,0 },0,{ 0,0,0 } },
	{ 3 , 19, 18  ,{ 255,255,255,0 },0,{ 0,0,0 } },
	{ 19, 30, 29  ,{ 255,255,255,0 },0,{ 0,0,0 } },
	{ 3 , 7 , 11  ,{ 255,255,255,0 },0,{ 0,0,0 } },
	{ 42, 46, 43  ,{ 255,255,255,0 },0,{ 0,0,0 } },
	{ 5 , 15, 12  ,{ 255,255,255,0 },0,{ 0,0,0 } },
	{ 39, 49, 50  ,{ 255,255,255,0 },0,{ 0,0,0 } },
	{ 40, 44, 45  ,{ 255,255,255,0 },0,{ 0,0,0 } },
	{ 41, 45, 46  ,{ 255,255,255,0 },0,{ 0,0,0 } },
	{ 1 , 21, 13  ,{ 255,255,255,0 },0,{ 0,0,0 } },
	{ 18, 54, 21  ,{ 255,255,255,0 },0,{ 0,0,0 } },
	{ 17, 53, 54  ,{ 255,255,255,0 },0,{ 0,0,0 } },
	{ 17, 0 , 22  ,{ 255,255,255,0 },0,{ 0,0,0 } },
	{ 4 , 10, 22  ,{ 255,255,255,0 },0,{ 0,0,0 } },
	{ 14, 52, 10  ,{ 255,255,255,0 },0,{ 0,0,0 } },
	{ 15, 8 , 52  ,{ 255,255,255,0 },0,{ 0,0,0 } },
	{ 15, 5 , 13  ,{ 255,255,255,0 },0,{ 0,0,0 } },
	{ 8 , 13, 21  ,{ 255,255,255,0 },0,{ 0,0,0 } },
	{ 21, 54, 8   ,{ 255,255,255,0 },0,{ 0,0,0 } },
	{ 54, 53, 8   ,{ 255,255,255,0 },0,{ 0,0,0 } },
	{ 53, 22, 10  ,{ 255,255,255,0 },0,{ 0,0,0 } },
	{ 10, 52, 53  ,{ 255,255,255,0 },0,{ 0,0,0 } },
	{ 33, 37, 36  ,{ 255,255,255,0 },0,{ 0,0,0 } },
	{ 37, 38, 35  ,{ 255,255,255,0 },0,{ 0,0,0 } },
	{ 38, 48, 47  ,{ 255,255,255,0 },0,{ 0,0,0 } },
	{ 34, 38, 37  ,{ 255,255,255,0 },0,{ 0,0,0 } },
	{ 32, 36, 35  ,{ 255,255,255,0 },0,{ 0,0,0 } },
	{ 24, 40, 41  ,{ 255,255,255,0 },0,{ 0,0,0 } },
	{ 26, 42, 39  ,{ 255,255,255,0 },0,{ 0,0,0 } },
	{ 30, 34, 33  ,{ 255,255,255,0 },0,{ 0,0,0 } },
	{ 28, 32, 31  ,{ 255,255,255,0 },0,{ 0,0,0 } },
	{ 31, 47, 48  ,{ 255,255,255,0 },0,{ 0,0,0 } }
};

t_obj objs[OBJS_V2] = {
	{ 55, 98, obj_points_3d, obj_points_2d, obj_polys }
};

t_scene scene = {
	{ -1000,-1000,-1000 },
	OBJS_V2,
	objs
};










/*
// ----------------------------------------------------- cube object v2
t_point3d obj_points_3d[8] = { { -100, -100, -100 }, { -100, 100, -100 }, { 100, 100, -100 }, { 100, -100, -100 },  
								{ -100, -100,  100 }, { -100, 100,  100 }, { 100, 100,  100 }, { 100, -100,  100 } };
t_point2d_tf obj_points_2d[8] = {0};
t_poly obj_polys[2 * 6] = { { 0,1,2 ,{255,255,255,0},0, {0,0,0}}, { 2,3,0 ,{ 255,255,255,0 },0,{ 0,0,0 } },  { 6,5,4 ,{ 255,255,255,0 },0,{ 0,0,0 } }, { 4,7,6 ,{ 255,255,255,0 },0,{ 0,0,0 } },
							{ 2,6,7 ,{ 255,255,255,0 },0, { 0,0,0 } }, { 3,2,7 ,{ 255,255,255,0 },0,{ 0,0,0 } },  { 4,5,1 ,{ 255,255,255,0 },0,{ 0,0,0 } }, { 1,0,4 ,{ 255,255,255,0 },0,{ 0,0,0 } },
							{ 5,6,2 ,{ 255,255,255,0 },0,{ 0,0,0 } }, { 2,1,5 ,{ 255,255,255,0 },0,{ 0,0,0 } },  { 3,7,4 ,{ 255,255,255,0 },0,{ 0,0,0 } }, { 4,0,3 ,{ 255,255,255,0 },0,{ 0,0,0 } } };

t_obj objs[OBJS_V2] = {
{8, 2 * 6, obj_points_3d, obj_points_2d, obj_polys}
};

t_scene scene = {
	{0,0,0 },
	OBJS_V2,
	objs
};
*/


//not used
int sizeof_segment3d = 16000;	//mondjuk


t_camera camera = {
	0,0,-1500,   0,0,0,   0,0,0,0,0,0,   0, { 0,0,1 }, { 0,0,1 }
};

double sindeg[360+1];
double cosdeg[360+1];

void testing(void);

int init_some_stuff(void);
void close_some_stuff(void);


//vector functions
t_vector vector_unitize(t_vector input);
t_vector vector_crossprod(t_vector inputv1, t_vector inputv2);
double vector_dotprod(t_vector inputv1, t_vector inputv2);

void movement(void);
void move_camera(void);
void getkeys(void);

int create_lot_objs_v2(void);
void display_scene(t_scene *scene);
void transform_object(t_obj *object, t_point3d *lightpos);
int prepare_poly(t_poly *p_poly, t_obj *object, t_point3d *p_lightpos, t_draw *draw_data);
void sort_polys(void);
void draw_polys(void);
void drawpoly_fill_v3(t_draw *draw);
int draw_memline(SDL_Point point_a, SDL_Point point_b, SDL_Point *memloc);
int backface_culling(t_poly *p_poly, t_point3d *p_points3d);
void transform_points(t_obj *object);
int get_screen_coords(t_poly *p_poly, t_point2d_tf *p_points2d_tf, t_draw *p_draw_data);
void set_color(t_poly *p_poly, t_point3d *p_points3d, t_point3d *p_lightpos, t_draw *p_draw_data);
void TextureFill(Uint32 *pixelmem, SDL_Color color, SDL_Point *points);
int sign(int x);

//global stuff for displaying result
t_draw *p_glb_polys2sort;
int glb_max_polys;
int glb_polycount;


SDL_bool done_global = SDL_FALSE;
SDL_bool dbg_global = SDL_FALSE;


//some colors
//SDL_Color col_objects = { 100, 100, 100, 255 };		//grey
SDL_Color col_objects = { 255, 255, 255, 255 };		//white
													//SDL_Color col_objects = { 0, 0, 0, 255 };		//black
SDL_Color col_texts = { 255, 159, 114, 255 };			//brown



int main(int argc, char* argv[])
{
	/*
	testing();
	return 0;
	*/

	if (init_some_stuff() != 0) {
		printf("init failed!\n");
		return -1;
	}
		

			SDL_bool done = SDL_FALSE;

			
			while (!done) 
			{
				//SDL_Event event;
				setup_screen();

				//draw to the screen

				display_scene(&scene);

				//display the screen
				movement();
				done = done_global;
				display_screen();
				StartCounter(&timer_main);
				//break;

			}
			close_some_stuff();

	return 0;
}

void getkeys(void) {
	//fill keytable
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
#ifndef _LINUX_
	static __int64 timer_move;
#else
	static int64_t timer_move;
#endif
	const double speed_cam_move = 1000;			//pixel per sec
	const double speed_cam_rot = 90;			//degree per sec
	double frametime;							//
	double val;
	val = PI / 180;
	const Uint8 *keystates = SDL_GetKeyboardState(NULL);	//get key states
	double rot_step;										//rotation step in degree
	double tx = camera.uvect.vx;
	double ty = camera.uvect.vy;
	double tz = camera.uvect.vz;

	frametime = GetCounter(&timer_move) / MILLIS_PER_SEC;
	getkeys();	//later

	rot_step = frametime * speed_cam_rot;
	SDL_PumpEvents();	//actualize events

	//use camera unit vectors


	if (keystates[SDL_SCANCODE_RIGHT] || keystates[SDL_SCANCODE_LEFT]) {
		if (keystates[SDL_SCANCODE_RIGHT]) {
			camera.roty += rot_step;	//% 360
			if (camera.roty > 360)
				camera.roty -= 360;

		}

		if (keystates[SDL_SCANCODE_LEFT]) {
			rot_step = -rot_step;
			camera.roty += rot_step;	//% 360
			if (camera.roty < 0)
				camera.roty += 360;
		}

		//computing camera unit vector based on steps
		//camera.uvect.vx = (cos(rot_step*val) * tx) + (sin(rot_step*val) * tz);
		//camera.uvect.vz = (cos(rot_step*val) * tz) - (sin(rot_step*val) * tx);
		
		//computing camera unit vector based on default camera unit vector
		camera.uvect.vx = (cos(camera.roty*val) * camera.uvect_default.vx) + (sin(camera.roty*val) * camera.uvect_default.vz);
		camera.uvect.vz = (cos(camera.roty*val) * camera.uvect_default.vz) - (sin(camera.roty*val) * camera.uvect_default.vx);
		camera.cx = cos(camera.rotx*val);
		camera.sx = sin(camera.rotx*val);
		camera.cy = cos(camera.roty*val);
		camera.sy = sin(camera.roty*val);
		camera.cz = cos(camera.rotz*val);
		camera.sz = sin(camera.rotz*val);

	}
	if (keystates[SDL_SCANCODE_UP]) {
		//working with degrees of camera
		/*
		camera.z += (cos(camera.roty*val) * (frametime*speed_cam_move));
		camera.x += (sin(camera.roty*val) * (frametime*speed_cam_move));
		*/
		//working with unit vector of camera
		camera.z += (camera.uvect.vz * (frametime*speed_cam_move));
		camera.x += (camera.uvect.vx * (frametime*speed_cam_move));
	}
	if (keystates[SDL_SCANCODE_DOWN]) {
		/*
		camera.z -= (cos(camera.roty*val) * (frametime*speed_cam_move));
		camera.x -= (sin(camera.roty*val) * (frametime*speed_cam_move));
		*/

		camera.z -= (camera.uvect.vz * (frametime*speed_cam_move));
		camera.x -= (camera.uvect.vx * (frametime*speed_cam_move));
	}

	if (keystates[SDL_SCANCODE_W]) {
		camera.y += (frametime*speed_cam_move);
	}
	if (keystates[SDL_SCANCODE_S]) {
		camera.y -= (frametime*speed_cam_move);
	}


	if (keystates[SDL_SCANCODE_ESCAPE]) {
		done_global = SDL_TRUE;
	}
	//debug stuff
	if (keystates[SDL_SCANCODE_RETURN]) {
		dbg_global = SDL_TRUE;
	}
	else
		dbg_global = SDL_FALSE;




	//lightpos = camera
	/*
	scene3.lightpos.x = camera.x;
	scene3.lightpos.y = camera.y;
	scene3.lightpos.z = camera.z;
	*/

	StartCounter(&timer_move);

	
	StartCounter(&timer_move);


}



int init_some_stuff(void)
{

	//init sdl
	if (sdl_open() != 0)
		return -1;

	//random number seed
	srand((unsigned int)time(NULL));

	//create_lot_cubes();
	
	if (create_lot_objs_v2() == -1)
		return -1;
	
	

	//init timer
	InitTimer();
	StartCounter(&timer_frame);
	StartCounter(&timer_main);


	//fill sin-cos tables
	if (0)	//not used at this time
	{
		int i;
		double val = PI / 180;

		for (i = 0; i <= 360; i++) {
			sindeg[i] = sin(i*val);
			cosdeg[i] = cos(i*val);
		}
	}
	
	//calculate normal vectors of initial object
	//don't forget to duplicate them in create_lot_objs_v2!!!
	{
		int i, j;
		double ax,ay,az, bx,by,bz, cx,cy,cz;
		t_vector vect1, vect2, crossvect, normvect;
		for (j = 0; j < scene.num_objects; j++) {
			for (i = 0; i < scene.objects[j].num_polys; i++) {
				//fill 3 point's coordinates
				ax = scene.objects[0].points_3d[scene.objects[j].polys[i].point_id[0]].x;
				ay = scene.objects[0].points_3d[scene.objects[j].polys[i].point_id[0]].y;
				az = scene.objects[0].points_3d[scene.objects[j].polys[i].point_id[0]].z;

				bx = scene.objects[0].points_3d[scene.objects[j].polys[i].point_id[1]].x;
				by = scene.objects[0].points_3d[scene.objects[j].polys[i].point_id[1]].y;
				bz = scene.objects[0].points_3d[scene.objects[0].polys[i].point_id[1]].z;

				cx = scene.objects[0].points_3d[scene.objects[j].polys[i].point_id[2]].x;
				cy = scene.objects[0].points_3d[scene.objects[j].polys[i].point_id[2]].y;
				cz = scene.objects[0].points_3d[scene.objects[j].polys[i].point_id[2]].z;

				//point a to point b
				vect1.vx = bx - ax;
				vect1.vy = by - ay;
				vect1.vz = bz - az;
				//point b to point c
				vect2.vx = cx - bx;
				vect2.vy = cy - by;
				vect2.vz = cz - bz;


				crossvect = vector_crossprod(vect1, vect2);
				normvect = vector_unitize(crossvect);

				if (normvect.vx == -0)
					normvect.vx = 0;
				if (normvect.vy == -0)
					normvect.vy = 0;
				if (normvect.vz == -0)
					normvect.vz = 0;

				scene.objects[j].polys[i].uvect_normal.vx = normvect.vx;
				scene.objects[j].polys[i].uvect_normal.vy = normvect.vy;
				scene.objects[j].polys[i].uvect_normal.vz = normvect.vz;

				if (0) {	//testing
					printf("poly %2d: (%d,%d,%d), (%d,%d,%d), (%d,%d,%d)\n", i, (int)ax, (int)ay, (int)az, (int)bx, (int)by, (int)bz, (int)cx, (int)cy, (int)cz);
					//printf("poly %2d: crossvect.vx=%f, crossvect.vy=%f, crossvect.vz=%f\n", i, crossvect.vx, crossvect.vy, crossvect.vz);
					printf("poly %2d: normvect.vx=%f, normvect.vy=%f, normvect.vz=%f\n", i, normvect.vx, normvect.vy, normvect.vz);
					printf("\n");
				}

			}
		}
	}


	//malloc mem for polys to sort
	//count max polys
	{
		int j;
		for (j = 0, glb_max_polys = 0; j < scene.num_objects; j++) {
			glb_max_polys += scene.objects[j].num_polys;
		}

		p_glb_polys2sort = (t_draw *)malloc(glb_max_polys * sizeof(t_draw));
		if (p_glb_polys2sort == NULL) {
			printf("malloc mem for polys to sort failed!\n");
			return -1;
		}

	}

	{
		double val = PI / 180;

		//init cos and sin
		camera.cx = cos(camera.rotx*val);
		camera.sx = sin(camera.rotx*val);
		camera.cy = cos(camera.roty*val);
		camera.sy = sin(camera.roty*val);
		camera.cz = cos(camera.rotz*val);
		camera.sz = sin(camera.rotz*val);
	}

	return 0;
}

void close_some_stuff(void)
{
	int k;

	//free mallocs
	for (k = 1; k < OBJS_V2; k++) {
		free (objs[k].points_2d_tf);
		free (objs[k].points_3d);
		free(objs[k].polys);
	}

	//free space for sorting polys
	free(p_glb_polys2sort);

}





















//v2 stuff
void display_scene(t_scene *scene)
{
#define DSPTIME 1000						//refresh interval for frametime (millisec)
	static double frametime, sleeptime;
	static int num_of_frames;

	t_obj *p_obj;
	glb_polycount = 0;

	//go through objects
		for (p_obj = scene->objects; p_obj < (scene->objects + scene->num_objects); p_obj++) {
		transform_object(p_obj, &scene->lightpos);
	}
	//sorting_polys();
	//display_polys();
	sort_polys();
	draw_polys();

	//update texture
	SDL_UpdateTexture(texture, NULL, pixels, screen_res_x * sizeof(Uint32));
	SDL_RenderCopy(renderer, texture, NULL, NULL);


	num_of_frames++;
	//refresh frametime value
	if (GetCounter(&timer_refr_txt) > DSPTIME) {
		frametime = GetCounter(&timer_frame) / num_of_frames;
		StartCounter(&timer_frame);
		num_of_frames = 0;
		StartCounter(&timer_refr_txt);
	}

	//print some stuff to the screen
	{
		int ypos = 0;
		char text2print1[100];
		char text2print2[100];
		char text2print3[100];
		char text2print4[100];
#ifndef _LINUX_
		sprintf_s(text2print1, sizeof(text2print1), "cam x=%.0f, y=%.0f, z=%.0f, rot-x:%.0f, rot-y:%.0f, rot-z:%.0f", camera.x, camera.y, camera.z, camera.rotx, camera.roty, camera.rotz);
		sprintf_s(text2print2, sizeof(text2print2), "fps: %.2f, objects: %d, polys drawn: %d", (float)(1000.00 / frametime), OBJS_V2, glb_polycount);				//float
		sprintf_s(text2print3, sizeof(text2print3), "camera unit vector x=%.2f, y=%.2f, z=%.2f", camera.uvect.vx, camera.uvect.vy, camera.uvect.vz);
		sprintf_s(text2print4, sizeof(text2print4), "lightpos = %d, %d, %d", scene->lightpos.x, scene->lightpos.y, scene->lightpos.z);
#else
		sprintf(text2print1, "cam x=%.0f, y=%.0f, z=%.0f, rot-x:%.0f, rot-y:%.0f, rot-z:%.0f", camera.x, camera.y, camera.z, camera.rotx, camera.roty, camera.rotz);
		sprintf(text2print2, "fps: %.2f, objects: %d, polys drawn: %d", (float)(1000.00 / frametime), OBJS_V2, glb_polycount);				//float
		sprintf(text2print3, "camera unit vector x=%.2f, y=%.2f, z=%.2f", camera.uvect.vx, camera.uvect.vy, camera.uvect.vz);
		sprintf(text2print4, "lightpos = %d, %d, %d", scene->lightpos.x, scene->lightpos.y, scene->lightpos.z);
#endif
		//display_text(0, ypos++, text2print1, col_texts);	//camera data
		display_text(0, ypos++, text2print2, col_texts);	//fps & co
		//display_text(0, ypos++, text2print3, col_texts);	//OTHER
		//display_text(0, ypos++, text2print4, col_texts);	//light source
	}


}




void transform_object(t_obj *object, t_point3d *lightpos)
{
	t_poly *p_poly;
	t_draw draw_data;

	//points belong to objects, so transform them here
	transform_points(object);

	for (p_poly = object->polys; p_poly < (object->polys + object->num_polys); p_poly++) {
		if (prepare_poly(p_poly, object, lightpos, &draw_data) == 0) {
			//save draw data if poly can be drawn
			*(p_glb_polys2sort + glb_polycount) = draw_data;
			glb_polycount++;
		}

	}

}


void transform_points(t_obj *object)
{
	int i, x, y, z, tx, ty, tz;
	const int persp_value = 500;

	for (i = 0; i < object->num_points; i++) {
		//apply camera position
		
		x = object->points_3d[i].x - (int)camera.x;
		y = object->points_3d[i].y - (int)camera.y;
		z = object->points_3d[i].z - (int)camera.z;

		//apply camera rotation
		tx = x; tz = z;	//y axis
		x = (int)((camera.cy * tx) - (camera.sy * tz));
		z = (int)((camera.cy * tz) + (camera.sy * tx));
		ty = y; tz = z;	//x axis
		z = (int)((camera.cx * tz) - (camera.sx * ty));
		y = (int)((camera.cx * ty) + (camera.sx * tz));
		ty = y; tx = x;	//z axis
		x = (int)((camera.cz * tx) - (camera.sz * ty));
		y = (int)((camera.cz * ty) + (camera.sz * tx));

		//translate to 2d
		if (z > 0) {
			object->points_2d_tf[i].valid = true;
			object->points_2d_tf[i].x = (int)((x * persp_value) / z);
			object->points_2d_tf[i].y = (int)((y * persp_value) / z);
		}
		else 
			object->points_2d_tf[i].valid = false;

		//save depth for transformed 2d point (uses for sorting)
		object->points_2d_tf[i].z = z;
	}


}



int prepare_poly(t_poly *p_poly, t_obj *object, t_point3d *p_lightpos, t_draw *draw_data)
//return value
//0: must be drawn
{
	
	if (backface_culling(p_poly, object->points_3d) != 0)
		return -1;		//not visible
	

	if (get_screen_coords(p_poly, object->points_2d_tf, draw_data) != 0)
		return -1;		//out of screen

	//fade colors based on screen direction
	set_color(p_poly, object->points_3d, p_lightpos, draw_data);

	return 0;

}

int backface_culling(t_poly *p_poly, t_point3d *p_points3d)
{
	t_vector vect2poly;
	double dotprod;

	//camera to face vector (no need to find center of the polygon, so first point is OK)
	vect2poly.vx = p_points3d[p_poly->point_id[0]].x - (int)camera.x;
	vect2poly.vy = p_points3d[p_poly->point_id[0]].y - (int)camera.y;
	vect2poly.vz = p_points3d[p_poly->point_id[0]].z - (int)camera.z;

	//get dot product
	dotprod = vector_dotprod(vect2poly, p_poly->uvect_normal);
	
	return (dotprod > 0.0f) ? 1 : 0;
}




int get_screen_coords(t_poly *p_poly, t_point2d_tf *p_points2d_tf, t_draw *p_draw_data)
{
	int i;
	int out_of_screen_points = 0;
	int p,z,x,y;
	int xsign = 0;
	int ysign = 0;


	for (i = 0; i < 3; i++) {
		p = p_poly->point_id[i];
		if (p_points2d_tf[p].valid) {
			z = p_points2d_tf[p].z;
			x = p_points2d_tf[p].x + (screen_res_x / 2);
			y = -(p_points2d_tf[p].y) + (screen_res_y / 2);

			//check visibility based on screen coordinates
			if (x < 0 || x >= screen_res_x)
				out_of_screen_points++;
			else if (y < 0 || y >= screen_res_y)
				out_of_screen_points++;

			//add sign
			ysign += sign(y);
			xsign += sign(x);

			//save x,y
			p_draw_data->points[i].x = x;
			p_draw_data->points[i].y = y;

			//save depth for screen point
			if (i == 0)
				p_draw_data->zdist = z;
			else
				p_draw_data->zdist = (p_draw_data->zdist + (int)z) / 2;
		}
		else 
			out_of_screen_points+=10;
	}

	//poly is not valid (before this function)
	if (out_of_screen_points > 3)
		return 1;

	//if all points are outside of screen, we have to check if it is visible or not
	//all points are outside of the same side: not visible
	//points are on different sides of the screen: visible
	if (out_of_screen_points == 3) {
		if (abs(xsign) == 3 || abs(ysign) == 3)
		return 1;
	}
	return 0;
}

//get sign (1 for positive, -1 for negative)
int sign(int x) {
	return (x > 0) - (x < 0);
}



void set_color(t_poly *p_poly, t_point3d *p_points3d, t_point3d *p_lightpos, t_draw *p_draw_data)
{

	t_vector vect2poly;
	double dotprod;
	float lightmag;

	//test
	/*
	p_draw_data->color.r = p_poly->color.r;
	p_draw_data->color.g = p_poly->color.g;
	p_draw_data->color.b = p_poly->color.b;
	return;
	*/

	//camera to face vector (no need to find center of the polygon, so first point is OK)
	vect2poly.vx = p_points3d[p_poly->point_id[0]].x - p_lightpos->x;
	vect2poly.vy = p_points3d[p_poly->point_id[0]].y - p_lightpos->y;
	vect2poly.vz = p_points3d[p_poly->point_id[0]].z - p_lightpos->z;
	vect2poly = vector_unitize(vect2poly);

	//get dot product
	dotprod = vector_dotprod(vect2poly, p_poly->uvect_normal);
	lightmag = (float)(dotprod + 2.00);


	//calc color
	p_draw_data->color.r = (int)((float)p_poly->color.r / lightmag);
	p_draw_data->color.g = (int)((float)p_poly->color.g / lightmag);
	p_draw_data->color.b = (int)((float)p_poly->color.b / lightmag);

}






int cmpfunc(const void * a, const void * b) {
	//v2 leiroval nem megy...
	t_draw *polyA = (t_draw *)a;
	t_draw *polyB = (t_draw *)b;
	return (polyB->zdist - polyA->zdist);
}
void sort_polys() {
	//t_poly3 *p_glb_polys2sort;
	//int glb_max_polys;
	//int glb_polycount;
	qsort(p_glb_polys2sort, glb_polycount, sizeof(t_draw), cmpfunc);

	if (0 && (dbg_global == SDL_TRUE)) {
		printf("sort_polys | glb_polycount=%d\n", glb_polycount);
	}
}

void draw_polys(void)
{
	t_draw *p_draw;
	int i;

	for (i = 0; i < glb_polycount; i++) {
		p_draw = (p_glb_polys2sort + i);
		drawpoly_fill_v3(p_glb_polys2sort + i);
		if (0 && dbg_global == SDL_TRUE) {
			printf("draw_polys | p_draw %d (%d,%d), (%d,%d), (%d,%d)\n",i,  p_draw->points[0].x, p_draw->points[0].y, p_draw->points[1].x, p_draw->points[1].y, p_draw->points[2].x, p_draw->points[2].y);
		}
	}
}



void drawpoly_fill_v3(t_draw *draw)
//flat color draw
{
	SDL_Point p_ord[3] = { 0 };
	SDL_Point p_tmp;
	int i, j = 0;
	int ymin = 0;
	SDL_Point *line1;
	SDL_Point *line2;
	int ymax;

	//set color
	SDL_SetRenderDrawColor(renderer, draw->color.r, draw->color.g, draw->color.b, draw->color.a);
	memcpy(p_ord, draw->points, sizeof(SDL_Point) * 3);

	if (0 && dbg_global == SDL_TRUE) {
		printf("point ORIG:(%d,%d), point b:(%d,%d), point c:(%d,%d), ymax %d\n", p_ord[0].x, p_ord[0].y, p_ord[1].x, p_ord[1].y, p_ord[2].x, p_ord[2].y, ymax);
	}

	//order points
	{
		//put upmost point in first pos
		for (i = 1; i < 3; i++) {
			if (p_ord[i].y < p_ord[0].y) {
				p_tmp = p_ord[0];	//swap
				p_ord[0] = p_ord[i];
				p_ord[i] = p_tmp;
			}
		}
		//swap two other two points if needed
		if (p_ord[2].y < p_ord[1].y) {
			p_tmp = p_ord[1];	//swap
			p_ord[1] = p_ord[2];
			p_ord[2] = p_tmp;
		}
	}

	ymax = p_ord[2].y - p_ord[0].y;
	if (0 && dbg_global == SDL_TRUE) {
		printf("point SORT:(%d,%d), point b:(%d,%d), point c:(%d,%d), ymax %d\n", p_ord[0].x, p_ord[0].y, p_ord[1].x, p_ord[1].y, p_ord[2].x, p_ord[2].y, ymax);
	}


	//allocate memory for two virtual lines
	if (ymax > (screen_res_y * 10))
		return;

	line1 = (SDL_Point *)malloc((ymax + 2) * sizeof(SDL_Point));
	line2 = (SDL_Point *)malloc((ymax + 2) * sizeof(SDL_Point));
	if (line1 == NULL || line2 == NULL) {
		printf("drawpoly_fill malloc failed! ymax=%d\n",ymax);
		if (line1)
			free(line1);
		if (line2)
			free(line2);
		return;
	}

	//draw lines in memory
	{
		int points_written;
		points_written = draw_memline(p_ord[0], p_ord[2], line1);
		points_written = draw_memline(p_ord[0], p_ord[1], line2);
		points_written = draw_memline(p_ord[1], p_ord[2], (line2 + points_written - 1));
	}


	//draw lines on screen
	{
		int ydiff = p_ord[2].y - p_ord[0].y;
		SDL_Point tmpoints[2];
		for (i = 0; i < ydiff; i++) {
			tmpoints[0] = line1[i];
			tmpoints[1] = line2[i];
			if (0 && dbg_global == SDL_TRUE)
				printf("line drawing | point a:(%d,%d), point b:(%d,%d)\n", tmpoints[0].x, tmpoints[0].y, tmpoints[1].x, tmpoints[1].y);
				TextureFill(pixels, draw->color, tmpoints);
		}
		if (0 && dbg_global == SDL_TRUE)
			printf("\n");
	}


	//free mallocs
	free(line1);
	free(line2);

	return;
}

void TextureFill(Uint32 *pixelmem, SDL_Color color, SDL_Point *points)
{
	int i;
	int tmpx1;
	int tmpx2;
	int cnt;			//points to draw
	Uint32 *u32mem = pixelmem;

	//exit if y is out of screen
	if (points[0].y < 0 || points[0].y >= screen_res_y)
		return;

	//fill starting and ending x coordinate
	if (points[0].x <= points[1].x) {
		tmpx1 = points[0].x;
		tmpx2 = points[1].x;
	}
	else {
		tmpx1 = points[1].x;
		tmpx2 = points[0].x;
	}

	//exit if x is out of screen
	if (tmpx1 >= screen_res_x || tmpx2 < 0)
		return;

	//clipping x
	tmpx1 = (tmpx1 >= 0) ? tmpx1 : 0;
	tmpx2 = (tmpx2 < screen_res_x) ? tmpx2 : (screen_res_x-1);

	//set length
	cnt =  abs(tmpx2 - tmpx1);

	//fill memory
	u32mem = pixelmem + points[0].y*screen_res_x + tmpx1;
	for (i = 0; i < cnt; i++) {
		memcpy(u32mem++, &color, sizeof(Uint32));
	}
	
}

int draw_memline(SDL_Point point_a, SDL_Point point_b, SDL_Point *memloc)
{
	int i = 0;
	int a = 0;	//from
	int b = 2;	//to
	float xadd;
	int ydiff = point_b.y - point_a.y;
	int xdiff = point_b.x - point_a.x;
	float xfloat = (float)point_a.x;

	memloc->x = point_a.x;
	memloc->y = point_a.y;

	if (ydiff)
		xadd = (float)xdiff / (float)ydiff;
	else {
		return 1;
	}

	if (0 && dbg_global == SDL_TRUE)
		printf("memline | point a:(%d,%d), point b:(%d,%d) | xdiff=%d, ydiff=%d, xadd=%f\n", point_a.x, point_a.y, point_b.x, point_b.y, xdiff, ydiff, xadd);

	if (0 && dbg_global == SDL_TRUE)
		printf("memline | memloc x=%d,y=%d\n", memloc->x, memloc->y);

	for (i = 1; i <= ydiff; i++) {	//0 esetén (ha a két y megegyezik) nem rajzol - egyelöre
		(memloc + i)->y = (memloc + i - 1)->y + 1;
		xfloat = xfloat + xadd;
		(memloc + i)->x = (int)(roundf(xfloat));
		if (0 && dbg_global == SDL_TRUE)
			printf("memline inner | (memloc + i)x=%d,y=%d\n", (memloc + i)->x, (memloc + i)->y);
	}

	if (0 && dbg_global == SDL_TRUE)
		printf("memline return %d\n", i);
	return i;
}









































int create_lot_objs_v2(void)
{
#define BLOCKSIZE_V2 10		//10^3 cubes
#define CUBESIDE_V2 200
#define CUBEPOLYS_V2 (2 * 6)
#define DISTANCE_V2 CUBESIDE_V2
	int i,k;

	//v2
	t_point3d *p_obj_points_3d;
	t_point2d_tf *p_obj_points_2d;
	t_poly *p_obj_polys;
	//t_poly *cube_polyconns;
	int num_objects = 1;
	int rand_x, rand_y, rand_z;
	int rand_a, rand_b, rand_c;
	SDL_Color tmpcol;
	int points_num = objs[0].num_points;
	int polys_num = objs[0].num_polys;
#define MAXDISTANCE_V2 (BLOCKSIZE_V2 * DISTANCE_V2 * 2 + OBJS_V2)
	//#define MAXDISTANCE_V2 (BLOCKSIZE_V2 + DISTANCE_V2 *  OBJS_V2)

	//randomnumber = rand() % MAXDISTANCE;
	for (k = 1; k < OBJS_V2; k++) {
		objs[k].num_points = objs[0].num_points;
		objs[k].num_polys = objs[0].num_polys;
		objs[k].polys = obj_polys;	//global stuff
		objs[k].points_2d_tf = obj_points_2d;
		p_obj_points_3d = (t_point3d *)malloc(points_num * sizeof(t_point3d));
		p_obj_points_2d = (t_point2d_tf *)malloc(points_num * sizeof(t_point2d_tf));
		p_obj_polys = (t_poly *)malloc(polys_num * sizeof(t_poly));
		if (p_obj_points_3d == NULL || p_obj_points_2d == NULL || p_obj_polys == NULL)
			return -1;
		objs[k].points_2d_tf = p_obj_points_2d;
		objs[k].points_3d = p_obj_points_3d;
		objs[k].polys = p_obj_polys;
		memcpy(objs[k].polys, objs[0].polys, polys_num * sizeof(t_poly) );
		rand_x = rand() % MAXDISTANCE_V2;
		rand_y = rand() % MAXDISTANCE_V2;
		rand_z = rand() % MAXDISTANCE_V2;
		for (i = 0; i < objs[0].num_points; i++) {
			objs[k].points_3d[i].x = objs[0].points_3d[i].x + rand_x;
			objs[k].points_3d[i].y = objs[0].points_3d[i].y + rand_y;
			objs[k].points_3d[i].z = objs[0].points_3d[i].z + rand_z;
		}
		//random color
		rand_a = rand() % 255 - 50;
		rand_b = rand() % 255 - 50;
		rand_c = rand() % 255 - 50;
		tmpcol.r = rand_a + 50;
		tmpcol.g = rand_b + 50;
		tmpcol.b = rand_c + 50;
		tmpcol.a = 255;
		for (i = 0; i < objs[k].num_polys; i++) {
			objs[k].polys[i].color = tmpcol;
		}
	}

	return 0;
}

































t_vector vector_crossprod(t_vector inputv1, t_vector inputv2)
//https://github.com/OneLoneCoder/videos/blob/master/OneLoneCoder_olcEngine3D_Part3.cpp
{
	t_vector retvect;	//returned vector
	retvect.vx = inputv1.vy * inputv2.vz - inputv1.vz * inputv2.vy;
	retvect.vy = inputv1.vz * inputv2.vx - inputv1.vx * inputv2.vz;
	retvect.vz = inputv1.vx * inputv2.vy - inputv1.vy * inputv2.vx;

	if (retvect.vx == 0 && retvect.vy == 0 && retvect.vz == 0) {
		printf("vector_crossprod error!\n");
		printf("inputv1.vy * inputv2.vz - inputv1.vz * inputv2.vy = %f * %f - %f * %f = %f - %f = %f\n", inputv1.vy, inputv2.vz, inputv1.vz, inputv2.vy, inputv1.vy * inputv2.vz, inputv1.vz * inputv2.vy, retvect.vx);
		printf("inputv1.vx * inputv2.vz - inputv1.vz * inputv2.vx = %f * %f - %f * %f = %f - %f = %f\n", inputv1.vx, inputv2.vz, inputv1.vz, inputv2.vx, inputv1.vx * inputv2.vz, inputv1.vz * inputv2.vx, retvect.vy);
		printf("inputv1.vx * inputv2.vx - inputv1.vy * inputv2.vx = %f * %f - %f * %f = %f - %f = %f\n", inputv1.vx, inputv2.vx, inputv1.vy, inputv2.vx, inputv1.vx * inputv2.vx, inputv1.vy * inputv2.vx, retvect.vz);
	}

	return retvect;
}

double vector_dotprod(t_vector inputv1, t_vector inputv2)
//https://github.com/OneLoneCoder/videos/blob/master/OneLoneCoder_olcEngine3D_Part3.cpp
{
	return inputv1.vx * inputv2.vx + inputv1.vy * inputv2.vy + inputv1.vz * inputv2.vz;
}


t_vector vector_unitize(t_vector inputv)
//http://www.fundza.com/vectors/normalize/
{
	t_vector retvect;	//returned vector
	double length;

	//calculate length
	length = sqrt((inputv.vx * inputv.vx) + (inputv.vy * inputv.vy) + (inputv.vz * inputv.vz));

	//unitize
	retvect.vx = inputv.vx / length;
	retvect.vy = inputv.vy / length;
	retvect.vz = inputv.vz / length;

	return retvect;

}


// ----------------------------------------------------------
// ***************************************************************** testing shit starts
void testing(void)
{
	t_vector ivect;
	t_vector ovect;

	ivect.vx = 3;
	ivect.vy = 1;
	ivect.vz = 2;

	ovect = vector_unitize(ivect);
	printf("ivect.vx=%f, ivect.vy=%f, ivect.vz=%f\n", ivect.vx, ivect.vy, ivect.vz);
	printf("ovect.vx=%f, ovect.vy=%f, ovect.vz=%f\n", ovect.vx, ovect.vy, ovect.vz);
	// ***************************************************************** testing shit ends

}

