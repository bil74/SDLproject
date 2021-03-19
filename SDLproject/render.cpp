#ifndef _LINUX_
using namespace std;
#include <iostream>
#include <windows.h>
#include <SDL.h>
#else
#include <unistd.h>
//#include <SDL/SDL.h>
#include </usr/include/SDL2/SDL.h>
#include </usr/include/SDL2/SDL_ttf.h>
#include <inttypes.h>
#endif
#include <math.h>
#include <time.h>
#include "main.h"
#include "timer.h"
#include "display.h"
#include "render.h"
#include "utils.h"
#include "control.h"
#include "config.h"


#define OBJS_V2 100

//definitions of external variables of this module (declared in module header)
//----------------------------------------------------------------------------

//timers
INT64 timer_refr_txt = 0;
INT64 timer_frame = 0;



// ----------------------------------------------------- videoship object v2
t_point3d obj_points_3d[] = {
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

t_point2d_tf obj_points_2d[ARRAY_SIZE(obj_points_3d)] = { 0 };

t_poly obj_polys[] = {
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
	{ ARRAY_SIZE(obj_points_3d), ARRAY_SIZE(obj_polys), obj_points_3d, obj_points_2d, obj_polys }
};
t_scene scene = {
	{ -1000,-1000,-1000 },
	OBJS_V2,
	objs
};











/*
// ----------------------------------------------------- cube object v2
t_point3d obj_points_3d[] = { { -100, -100, -100 }, { -100, 100, -100 }, { 100, 100, -100 }, { 100, -100, -100 },
{ -100, -100,  100 }, { -100, 100,  100 }, { 100, 100,  100 }, { 100, -100,  100 } };
t_point2d_tf obj_points_2d[ARRAY_SIZE(obj_points_3d)] = {0};
t_poly obj_polys[] = { { 0,1,2 ,{255,255,255,0},0, {0,0,0}}, { 2,3,0 ,{ 255,255,255,0 },0,{ 0,0,0 } },  { 6,5,4 ,{ 255,255,255,0 },0,{ 0,0,0 } }, { 4,7,6 ,{ 255,255,255,0 },0,{ 0,0,0 } },
{ 2,6,7 ,{ 255,255,255,0 },0, { 0,0,0 } }, { 3,2,7 ,{ 255,255,255,0 },0,{ 0,0,0 } },  { 4,5,1 ,{ 255,255,255,0 },0,{ 0,0,0 } }, { 1,0,4 ,{ 255,255,255,0 },0,{ 0,0,0 } },
{ 5,6,2 ,{ 255,255,255,0 },0,{ 0,0,0 } }, { 2,1,5 ,{ 255,255,255,0 },0,{ 0,0,0 } },  { 3,7,4 ,{ 255,255,255,0 },0,{ 0,0,0 } }, { 4,0,3 ,{ 255,255,255,0 },0,{ 0,0,0 } } };

t_obj objs[OBJS_V2] = {
{ARRAY_SIZE(obj_points_3d), ARRAY_SIZE(obj_polys), obj_points_3d, obj_points_2d, obj_polys}
};

t_scene scene = {
{0,0,0 },
OBJS_V2,
objs
};
*/



t_camera camera = {
	0,0,-1500,   0,0,0,   0,0,0,0,0,0,   0,{ 0,0,1 },{ 0,0,1 }
};

double sindeg[360 + 1];
double cosdeg[360 + 1];

void testing(void);





//local render functions
void transform_object(t_obj *object, t_point3d *lightpos, t_drawpoly **dpoly);
int prepare_poly(t_poly *p_poly, t_obj *object, t_point3d *p_lightpos, t_drawpoly *draw_data);
void sort_dpolys(t_drawpoly *p_draw_array, int count);
void draw_dpolys(t_drawpoly *p_draw_array, int count);
void drawpoly_fill(t_drawpoly *draw);
void drawpoly_wire(t_drawpoly *draw);	//wire
void drawline_pixelmem_OLD(Uint32 *pixelmem, SDL_Point point_a, SDL_Point point_b, SDL_Color color);
void drawline_pixelmem(Uint32 *pixelmem, SDL_Point point_a, SDL_Point point_b, SDL_Color color);	//wire
int draw_memline(SDL_Point point_a, SDL_Point point_b, SDL_Point *memloc);
int backface_culling(t_poly *p_poly, t_point3d *p_points3d);
void transform_points(t_obj *object);
int set_screen_coords(t_poly *p_poly, t_point2d_tf *p_points2d_tf, t_drawpoly *p_draw_data);
void set_color(t_poly *p_poly, t_point3d *p_points3d, t_point3d *p_lightpos, t_drawpoly *p_draw_data);
void draw_horizontal_line(Uint32 *pixelmem, SDL_Color color, SDL_Point *points);

//local vector functions
t_vector vector_unitize(t_vector input);
t_vector vector_crossprod(t_vector inputv1, t_vector inputv2);
double vector_dotprod(t_vector inputv1, t_vector inputv2);

//local other functions
int create_lot_objs_v2(void);
int sign(int x);


t_drawpoly *p_dpolys;	//must include here as well


SDL_Color col_texts = { 255, 159, 114, 255 };			//brown-ish
int rend_drawmode;	//DM_FLATS, DM_WIREF
int persp_value = 150;

//not used (yet)
int sizeof_segment3d = 16000;


int get_persp_value()
{
	return persp_value;
}
void set_persp_value(int pval)
{
	persp_value = pval;
}



int start_render(void)
{
	rend_drawmode = get_conf_val(drawmode);
	return 0;
}

void stop_render(void)
{

}


int init_some_stuff(void)
{
	/*
	//init sdl
	if (sdl_open() != 0)
		return -1;
	*/

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

	//calculate normal vectors for all objects
	{
		int i, j;
		double ax, ay, az, bx, by, bz, cx, cy, cz;
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


				//calculate cross product
				crossvect = vector_crossprod(vect1, vect2);
				//cut to unit length
				normvect = vector_unitize(crossvect);

				if (normvect.vx == -0)
					normvect.vx = 0;
				if (normvect.vy == -0)
					normvect.vy = 0;
				if (normvect.vz == -0)
					normvect.vz = 0;

				//save normal to object
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
	{
		int max_polys;
		int j;

		//count max polys
		for (j = 0, max_polys = 0; j < scene.num_objects; j++) {
			max_polys += scene.objects[j].num_polys;
		}

		//allocate memory
		p_dpolys = (t_drawpoly *)malloc(max_polys * sizeof(t_drawpoly));
		if (p_dpolys == NULL) {
			printf("malloc mem for polys to sort failed!\n");
			return -1;
		}

	}

	//calc cos and sin for initial camera rotation
	{
		double val = PI / 180;

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

	//free object mallocs
	for (k = 1; k < OBJS_V2; k++) {
		free(objs[k].points_2d_tf);
		free(objs[k].points_3d);
		free(objs[k].polys);
	}

	//free space for sorting polys
	free(p_dpolys);

}





//v2 stuff
void render_scene(t_scene *scene, t_drawpoly *drawpolys)
{
#define DSPTIME 1000						//refresh interval for frametime (millisec)
	static double frametime, sleeptime;
	static int num_of_frames;

	t_obj *p_obj;
	int polycount = 0;
	t_drawpoly *dpoly = drawpolys;

	//go through objects
	for (p_obj = scene->objects; p_obj < (scene->objects + scene->num_objects); p_obj++) {
		transform_object(p_obj, &scene->lightpos, &dpoly);
	}
	polycount = dpoly - drawpolys;
	sort_dpolys(drawpolys, polycount);
	draw_dpolys(drawpolys, polycount);

	//update texture
	SDL_UpdateTexture(texture, NULL, pixels, screen_res_x * sizeof(Uint32));
	//copy texture to renderer
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
		//char text2print1[100];
		char text2print2[100];
		//char text2print3[100];
		//char text2print4[100];
		//snprintf(text2print1, sizeof(text2print1), "cam x=%.0f, y=%.0f, z=%.0f, rot-x:%.0f, rot-y:%.0f, rot-z:%.0f", camera.x, camera.y, camera.z, camera.rotx, camera.roty, camera.rotz);
		snprintf(text2print2, sizeof(text2print2), "fps: %.2f, objects: %d, polys drawn: %d, alt: %d", (float)(1000.00 / frametime), OBJS_V2, polycount, (int)camera.y);				//float
																																								//snprintf(text2print3, sizeof(text2print3), "camera unit vector x=%.2f, y=%.2f, z=%.2f", camera.uvect.vx, camera.uvect.vy, camera.uvect.vz);
																																								//snprintf(text2print4, sizeof(text2print4), "lightpos = %d, %d, %d", scene->lightpos.x, scene->lightpos.y, scene->lightpos.z);

																																								//display_text(0, ypos++, text2print1, col_texts);	//camera data
		display_text(0, ypos++, text2print2, col_texts);	//fps & co
															//display_text(0, ypos++, text2print3, col_texts);	//OTHER
															//display_text(0, ypos++, text2print4, col_texts);	//light source
	}


}




void transform_object(t_obj *object, t_point3d *lightpos, t_drawpoly **dpoly)
{
	t_poly *p_poly;

	//points belong to objects, so transform them here
	transform_points(object);

	for (p_poly = object->polys; p_poly < (object->polys + object->num_polys); p_poly++) {
		if (prepare_poly(p_poly, object, lightpos, *dpoly) == 0) {
			//update drawpoly pointer if poly is visible
			(*dpoly)++;
		}

	}

}


void transform_points(t_obj *object)
{
	int i, x, y, z, tx, ty, tz;
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



int prepare_poly(t_poly *p_poly, t_obj *object, t_point3d *p_lightpos, t_drawpoly *draw_data)
//return value
//0: must be drawn
{

	if (backface_culling(p_poly, object->points_3d) != 0)
		return -1;		//not visible


	if (set_screen_coords(p_poly, object->points_2d_tf, draw_data) != 0)
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

	//return visibility
	return (dotprod > 0.0f) ? 1 : 0;
}




int set_screen_coords(t_poly *p_poly, t_point2d_tf *p_points2d_tf, t_drawpoly *p_draw_data)
//translate transformed 2d coordinates to screen cordinates (0 <= x < screen_res_x and 0 <= y < screen_res_y)
//also check if poly is within field of vision
{
	int i;
	int out_of_screen_points = 0;
	int p, z, x, y;
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
			out_of_screen_points += 10;
	}

	//poly is not valid (known before this function)
	if (out_of_screen_points > 3)
		return 1;

	//if all points are outside of screen, we have to check if it is visible or not
	//all points are outside of the same side of the screen: not visible
	//points are on different sides of the screen: visible
	if (out_of_screen_points == 3) {
		if (abs(xsign) == 3 || abs(ysign) == 3)
			return 1;
	}

	return 0;
}



void set_color(t_poly *p_poly, t_point3d *p_points3d, t_point3d *p_lightpos, t_drawpoly *p_draw_data)
{

	t_vector vect2poly;
	double dotprod;
	float lightmag;

	//test - don't change original color
	/*
	p_draw_data->color.r = p_poly->color.r;
	p_draw_data->color.g = p_poly->color.g;
	p_draw_data->color.b = p_poly->color.b;
	return;
	*/
	if (rend_drawmode == DM_FLATS) {
		//camera to face vector (no need to find center of the polygon, so first point is OK)
		vect2poly.vx = p_points3d[p_poly->point_id[0]].x - p_lightpos->x;
		vect2poly.vy = p_points3d[p_poly->point_id[0]].y - p_lightpos->y;
		vect2poly.vz = p_points3d[p_poly->point_id[0]].z - p_lightpos->z;
		vect2poly = vector_unitize(vect2poly);

		//get dot product
		dotprod = vector_dotprod(vect2poly, p_poly->uvect_normal);
		lightmag = (float)(dotprod + 2.00);
	}
	else {
		lightmag = 1;		//set this to disable shading
	}

	//calc color
	p_draw_data->color.r = (int)((float)p_poly->color.r / lightmag);
	p_draw_data->color.g = (int)((float)p_poly->color.g / lightmag);
	p_draw_data->color.b = (int)((float)p_poly->color.b / lightmag);

}






int cmpfunc(const void * a, const void * b) {
	//this function belongs to qsort
	t_drawpoly *polyA = (t_drawpoly *)a;
	t_drawpoly *polyB = (t_drawpoly *)b;
	return (polyB->zdist - polyA->zdist);
}

void sort_dpolys(t_drawpoly *p_draw_array, int count)
{
	qsort(p_draw_array, count, sizeof(t_drawpoly), cmpfunc);
	if (0 && (dbg_global == true)) {
		printf("sort_dpolys | glb_polycount=%d\n", count);
	}
}

void draw_dpolys(t_drawpoly *p_draw_array, int count)
{
	int i;

	for (i = 0; i < count; i++) {
		if (rend_drawmode == DM_FLATS)
			drawpoly_fill(p_draw_array);
		else
			drawpoly_wire(p_draw_array);
		if (0 && dbg_global == true) {
			printf("draw_dpolys | p_draw %d (%d,%d), (%d,%d), (%d,%d)\n", i, p_draw_array->points[0].x, p_draw_array->points[0].y, p_draw_array->points[1].x, p_draw_array->points[1].y, p_draw_array->points[2].x, p_draw_array->points[2].y);
		}
		p_draw_array++;
	}
}


void drawpoly_wire(t_drawpoly *draw)
{
	SDL_Point p_ord[3] = { 0 };	//*not ordered :)

	//set color
	SDL_Color col_draw = { draw->color.r, draw->color.g, draw->color.b, draw->color.a };
	memcpy(p_ord, draw->points, sizeof(SDL_Point) * 3);
	//draw lines on texture mem
	drawline_pixelmem(pixels, p_ord[0], p_ord[2], col_draw);
	drawline_pixelmem(pixels, p_ord[0], p_ord[1], col_draw);
	drawline_pixelmem(pixels, p_ord[1], p_ord[2], col_draw);
}

//testing
void drawline_pixelmem_BAD_DONT_USE_IT(Uint32 *pixelmem, SDL_Point point_a, SDL_Point point_b, SDL_Color color)
{
	int i;
	int xdiff = point_b.x - point_a.x;
	int ydiff = point_b.y - point_a.y;
	Uint32 *pm1;// = pixelmem + (point_a.y * screen_res_x) + point_a.x;
	bool oos = false;	//true if line is partly out of screen

	//testing stuff
	int diff_count;
	int cycle_add_constant, cycle_add_conditional;
	int checkval;
	int checkval_base = 0;
	int xdiff_abs = abs(xdiff);
	int ydiff_abs = abs(ydiff);
	int puthere = (point_a.y * screen_res_x) + point_a.x;
	/*
	int going_on;
	int puthere_x = point_a.x;
	int puthere_y = point_a.y;
	int cycle_add_x, cycle_add_y;
	*/

	if (!(xdiff_abs + ydiff_abs))
		return;

	if (xdiff_abs >= ydiff_abs) {
	//we go on "x" axis in the cycle
		diff_count = xdiff_abs;
		cycle_add_constant /*= cycle_add_x*/ = sign(xdiff);
		cycle_add_conditional = sign(ydiff) * screen_res_x;
		//cycle_add_y = sign(ydiff);
		checkval = (ydiff_abs * 256) / xdiff_abs;
		//going_on = 1;
	}
	else {
	//we go on "y" axis in the cycle
		diff_count = ydiff_abs;
		cycle_add_constant = sign(ydiff) * screen_res_x;
		//cycle_add_y = sign(ydiff);
		cycle_add_conditional /*= cycle_add_x*/ = sign(xdiff);
		checkval = (xdiff_abs * 256) / ydiff_abs;
		//going_on = 0;
	}


	if (point_a.x < 0 || point_a.y < 0 || point_a.x >= screen_res_x || point_a.y >= screen_res_y)
		oos = true;
	if (point_b.x < 0 || point_b.y < 0 || point_b.x >= screen_res_x || point_b.y >= screen_res_y)
		oos = true;



	if (oos == true) {
		/*
		for (i = 0; i < diff_count; i++) {
			pm1 = pixelmem + puthere;
			if (puthere_x >= 0 && puthere_x < screen_res_x && puthere_y >= 0 && puthere_y < screen_res_y)
				memcpy(pm1, &color, sizeof(Uint32));
			puthere += cycle_add_constant;
			checkval_base += checkval;

			if (checkval_base & ~255) {
				puthere += cycle_add_conditional;
				puthere_x += cycle_add_x;
				puthere_y += cycle_add_y;
			}
			else {
				puthere_x += going_on ? cycle_add_x : 0;
				puthere_y += !going_on ? cycle_add_y : 0;
			}
			checkval_base &= 255;

		}
		*/
	}
	else {
		for (i = 0; i < diff_count; i++) {
			pm1 = pixelmem + puthere;
			memcpy(pm1, &color, sizeof(Uint32));
			puthere += cycle_add_constant;
			checkval_base += checkval;
			puthere += (checkval_base & ~255) ? cycle_add_conditional : 0;		//if (checkval_base > 255)
			checkval_base &= 255;
		}
	}

}


void drawline_pixelmem(Uint32 *pixelmem, SDL_Point point_a, SDL_Point point_b, SDL_Color color)
{
	int i;
	int xdiff = point_b.x - point_a.x;
	int ydiff = point_b.y - point_a.y;
	float xfloat = (float)point_a.x;
	float yfloat = (float)point_a.y;
	int diff_count = ((abs(xdiff) - abs(ydiff)) >= 0) ? abs(xdiff) : abs(ydiff);	//cycle counter
	float xadd = (float)xdiff / (float)diff_count;
	float yadd = (float)ydiff / (float)diff_count;
	Uint32 *pm1;// = pixelmem + (point_a.y * screen_res_x) + point_a.x;
	
	bool oos = false;	//true if line is partly out of screen
	
	if (point_a.x < 0 || point_a.y < 0 || point_a.x >= screen_res_x || point_a.y >= screen_res_y)
		oos = true;
	if (point_b.x < 0 || point_b.y < 0 || point_b.x >= screen_res_x || point_b.y >= screen_res_y)
		oos = true;
	
	if (oos == true) {
		
		for (i = 0; i < diff_count; i++) {
			pm1 = pixelmem + ((int)yfloat * screen_res_x) + (int)xfloat;
			if (sign((int)yfloat) >= 0 && sign((int)xfloat) >= 0 && xfloat < (float)screen_res_x && yfloat < (float)screen_res_y)
				memcpy(pm1, &color, sizeof(Uint32));
			xfloat += xadd;
			yfloat += yadd;
		}
		
	}
	else {
		for (i = 0; i < diff_count; i++) {
			pm1 = pixelmem + ((int)yfloat * screen_res_x) + (int)xfloat;
			memcpy(pm1, &color, sizeof(Uint32));
			xfloat += xadd;
			yfloat += yadd;
		}
	}
	
}



void drawpoly_fill(t_drawpoly *draw)
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

	if (0 && dbg_global == true) {
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
	if (0 && dbg_global == true) {
		printf("point SORT:(%d,%d), point b:(%d,%d), point c:(%d,%d), ymax %d\n", p_ord[0].x, p_ord[0].y, p_ord[1].x, p_ord[1].y, p_ord[2].x, p_ord[2].y, ymax);
	}


	//allocate memory for two virtual lines
	if (ymax > (screen_res_y * 10))
		return;

	line1 = (SDL_Point *)malloc((ymax + 2) * sizeof(SDL_Point));
	line2 = (SDL_Point *)malloc((ymax + 2) * sizeof(SDL_Point));
	if (line1 == NULL || line2 == NULL) {
		printf("drawpoly_fill malloc failed! ymax=%d\n", ymax);
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
			if (0 && dbg_global == true) {
				printf("line drawing | point a:(%d,%d), point b:(%d,%d)\n", tmpoints[0].x, tmpoints[0].y, tmpoints[1].x, tmpoints[1].y);
			}
			draw_horizontal_line(pixels, draw->color, tmpoints);
		}
		if (0 && dbg_global == true)
			printf("\n");
	}


	//free mallocs
	free(line1);
	free(line2);

	return;
}

void draw_horizontal_line(Uint32 *pixelmem, SDL_Color color, SDL_Point *points)
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
	tmpx2 = (tmpx2 < screen_res_x) ? tmpx2 : (screen_res_x - 1);

	//set length
	cnt = abs(tmpx2 - tmpx1);

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

	if (0 && dbg_global == true)
		printf("memline | point a:(%d,%d), point b:(%d,%d) | xdiff=%d, ydiff=%d, xadd=%f\n", point_a.x, point_a.y, point_b.x, point_b.y, xdiff, ydiff, xadd);

	if (0 && dbg_global == true)
		printf("memline | memloc x=%d,y=%d\n", memloc->x, memloc->y);

	for (i = 1; i <= ydiff; i++) {	//0 esetén (ha a két y megegyezik) nem rajzol - egyelöre
		(memloc + i)->y = (memloc + i - 1)->y + 1;
		xfloat = xfloat + xadd;
		(memloc + i)->x = (int)(roundf(xfloat));
		if (0 && dbg_global == true)
			printf("memline inner | (memloc + i)x=%d,y=%d\n", (memloc + i)->x, (memloc + i)->y);
	}

	if (0 && dbg_global == true)
		printf("memline return %d\n", i);
	return i;
}









































int create_lot_objs_v2(void)
{
#define BLOCKSIZE_V2 10		//10^3 cubes
#define CUBESIDE_V2 200
#define CUBEPOLYS_V2 (2 * 6)
#define DISTANCE_V2 CUBESIDE_V2
	int i, k;

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
		memcpy(objs[k].polys, objs[0].polys, polys_num * sizeof(t_poly));
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
