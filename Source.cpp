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
#define CUBES 1
#define CUBES_V2 1000

extern int screen_res_x;
extern int screen_res_y;
extern SDL_Window* window;
extern SDL_Renderer* renderer;

//timers
INT64 timer_main = 0;
INT64 timer_refr_txt = 0;
INT64 timer_frame = 0;

//cube object v2
t_point3d cube_points_3d[8] = { { -100, -100, -100 }, { -100, 100, -100 }, { 100, 100, -100 }, { 100, -100, -100 },  /**/  
								{ -100, -100,  100 }, { -100, 100,  100 }, { 100, 100,  100 }, { 100, -100,  100 } };
t_point2d cube_points_2d[8] = {0};
t_poly cube_polyconns[2 * 6] = { { 0,1,2 ,0,1 }, { 2,3,0 ,0,1},   { 6,5,4 ,0,1}, { 4,7,6 ,0,1},   
								{ 2,6,7 ,0,1}, { 3,2,7 ,0,1},  { 4,5,1 ,0,1}, { 1,0,4 ,0,1},    //OK
								{ 5,6,2 ,0,1}, { 2,1,5 ,0,1},    { 3,7,4 ,0,1}, { 4,0,3 ,0,1} };

t_obj objs_v2[CUBES_V2] = {
{8, 2 * 6, cube_points_3d, cube_points_2d, cube_polyconns}
};

t_scene scene = {
	CUBES_V2,
	objs_v2
};



//cube object v1
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

t_poly2d cube2d[2 * 6 * CUBES] = { 0 };

//not used
int sizeof_segment3d = 16000;	//mondjuk


t_camera camera = {
	0,0,-1500, 0,0,0,  0, {0,0,0}, {0,0,1}
};

double sindeg[360+1];
double cosdeg[360+1];


int init_some_stuff(void);
void close_some_stuff(void);

//v1 render
void create_lot_cubes(void);
void render_scene(void);
void apply_camera(void);
void transform3d_to_2d(void);
void draw2d(void);

//v2 render
int create_lot_cubes_v2(void);
void render_scene_v2(t_scene *scene);
void do_3d_to_2d(t_scene *scene);
void draw2d_v2(t_scene *scene);

void movement(void);
void move_camera(void);
void getkeys(void);



SDL_bool done_global = SDL_FALSE;
SDL_bool dbg_global = SDL_FALSE;



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
				setup_screen();

				//draw to the screen

				render_scene_v2(&scene);

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
	if (keystates[SDL_SCANCODE_ESCAPE]) {
		done_global = SDL_TRUE;
	}
	//debug stuff
	if (keystates[SDL_SCANCODE_RETURN]) {
		dbg_global = SDL_TRUE;
	}
	else
		dbg_global = SDL_FALSE;
	
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
	
	if (create_lot_cubes_v2() == -1)
		return -1;
	

	//init timer
	InitTimer();
	StartCounter(&timer_frame);
	StartCounter(&timer_main);

	
	{
		//fill sin-cos tables
		int i;
		double val = PI / 180;

		for (i = 0; i <= 360; i++) {
			sindeg[i] = sin(i*val);
			cosdeg[i] = cos(i*val);
		}
	}
	
	
	return 0;
}

void close_some_stuff(void)
{
	int k;

	//free mallocs
	for (k = 1; k < CUBES_V2; k++) {
		free (objs_v2[k].points_2d);
		free (objs_v2[k].points_3d);
	}

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

	//printf("render scene v2 math done!\n");
	//return;

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

#ifndef _LINUX_
	sprintf_s(text2print, sizeof(text2print), "cam x=%.0f, y=%.0f, z=%.0f, rot-x:%.0f, rot-y:%.0f, rot-z:%.0f", camera.x, camera.y, camera.z, camera.rotx, camera.roty, camera.rotz);
#else
	sprintf(text2print, "cam x=%.0f, y=%.0f, z=%.0f, rot-x:%.0f, rot-y:%.0f, rot-z:%.0f", camera.x, camera.y, camera.z, camera.rotx, camera.roty, camera.rotz);
#endif
	display_text(0, 0, text2print);

	//fps
	//sprintf_s(text2print, sizeof(text2print), "fps %d", (int)roundf((float)(1000.00 / frametime)));	//int
#ifndef _LINUX_
	sprintf_s(text2print, sizeof(text2print), "fps %.2f", (float)(1000.00 / frametime));				//float
#else
	sprintf(text2print, "fps %.2f", (float)(1000.00 / frametime));				//float
#endif
	display_text(0, 1, text2print);


	//OTHER
	//sprintf_s(text2print, sizeof(text2print), "fps %d", (int)roundf((float)(1000.00 / frametime)));	//int
#ifndef _LINUX_
	sprintf_s(text2print, sizeof(text2print), "camera unit vector x=%.2f, y=%.2f, z=%.2f", camera.uvect.vx, camera.uvect.vy, camera.uvect.vz);
#else
	sprintf(text2print, "fps %.2f", (float)(1000.00 / frametime));				//float
#endif
	display_text(0, 2, text2print);


}




void do_3d_to_2d(t_scene *scene_3to2v2) {

	int i, j;
	double val;
	val = PI / 180;
	const int persp_value = 500;
	int x, y, z, tx, ty, tz;	//temporary values
	double cx = cos(camera.rotx*val);
	double sx = sin(camera.rotx*val);
	double cy = cos(camera.roty*val);
	double sy = sin(camera.roty*val);
	double cz = cos(camera.rotz*val);
	double sz = sin(camera.rotz*val);

	//TODO: use camera unit vectors to determine rotations?
	//get cam rot vals from its unit vector
	//int camrot_x, camrot_y, camrot_z;
	//camrot_y = asin(camera.uvect. ) * val;
	
	for (i = 0; i < scene_3to2v2->num_objects; i++) {					//all objects
		for (j = 0; j < scene_3to2v2->objects[i].num_points; j++) {		//1 object

			//apply camera position
			x = scene_3to2v2->objects[i].points_3d[j].x - (int)camera.x;
			y = scene_3to2v2->objects[i].points_3d[j].y - (int)camera.y;
			z = scene_3to2v2->objects[i].points_3d[j].z - (int)camera.z;

			//apply camera rotation
			// ******************* y axis
			tx = x; tz = z;
			x = (int)((cy * tx) - (sy * tz));
			z = (int)((cy * tz) + (sy * tx));

			// ******************* x axis
			ty = y; tz = z;
			z = (int)((cx * tz) - (sx * ty));
			y = (int)((cx * ty) + (sx * tz));

			// ******************* z axis
			ty = y; tx = x;
			x = (int)((cz * tx) - (sz * ty));
			y = (int)((cz * ty) + (sz * tx));

			//transform 3d to 2d
			if (z <= 0)
				scene_3to2v2->objects[i].points_2d[j].valid = false;
			else {
				//transform 3d to 2d
				scene_3to2v2->objects[i].points_2d[j].valid = true;
				scene_3to2v2->objects[i].points_2d[j].x = (x * persp_value) / z;
				scene_3to2v2->objects[i].points_2d[j].y = (y * persp_value) / z;
				//printf("; 2d x:%d, y:%d (z:%d)\n", scene_3to2v2->objects[0].points_2d[j].x, scene_3to2v2->objects[0].points_2d[j].y, z);

			}

		}	
		//break;
	}

}





void draw2d_v2(t_scene *scene_d2v2)
{
	SDL_Point points_tmp[3 + 1] = { 0 };
	int i, j, k;
	int out_of_screen_points;


	//printf("draw2d_v2 scene_d2v2.objects[0].num_polys:%d\n", scene_d2v2.objects[0].num_polys);
	for (k = 0; k < scene_d2v2->num_objects; k++) {
		for (i = 0; i < scene_d2v2->objects[k].num_polys; i++) {
			out_of_screen_points = 0;
			int p;
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
#define BLOCKSIZE_V2 10		//10^3 cubes
#define CUBESIDE_V2 200
#define CUBEPOLYS_V2 (2 * 6)
#define DISTANCE_V2 CUBESIDE_V2
	int i,k;

	//v2
	t_point3d *p_cube_points_3d;
	t_point2d *p_cube_points_2d;
	//t_poly *cube_polyconns;
	int num_objects = 1;
	int rand_x, rand_y, rand_z;
	#define MAXDISTANCE_V2 (BLOCKSIZE_V2 * DISTANCE_V2 * 2 + CUBES_V2)

	//randomnumber = rand() % MAXDISTANCE;
	for (k = 1; k < CUBES_V2; k++) {
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
		rand_x = rand() % MAXDISTANCE_V2;
		rand_y = rand() % MAXDISTANCE_V2;
		rand_z = rand() % MAXDISTANCE_V2;
		for (i = 0; i < 8; i++) {
			objs_v2[k].points_3d[i].x = objs_v2[0].points_3d[i].x + rand_x;
			objs_v2[k].points_3d[i].y = objs_v2[0].points_3d[i].y + rand_y;
			objs_v2[k].points_3d[i].z = objs_v2[0].points_3d[i].z + rand_z;
		}
	}

	return 0;
}















































//old stuff here - will be deleted later
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
#ifndef _LINUX_
	sprintf_s(text2print, sizeof(text2print), "cam x=%.0f, y=%.0f, z=%.0f, rot-x:%.0f, rot-y:%.0f, rot-z:%.0f", camera.x, camera.y, camera.z, camera.rotx, camera.roty, camera.rotz);
#else
	sprintf(text2print, "cam x=%.0f, y=%.0f, z=%.0f, rot-x:%.0f, rot-y:%.0f, rot-z:%.0f", camera.x, camera.y, camera.z, camera.rotx, camera.roty, camera.rotz);
#endif
	display_text(0, 0, text2print);
	//fps
	//sprintf_s(text2print, sizeof(text2print), "fps %d", (int)roundf((float)(1000.00 / frametime)));
#ifndef _LINUX_
	sprintf_s(text2print, sizeof(text2print), "fps %.2f", (float)(1000.00 / frametime));
#else
	sprintf(text2print, "fps %.2f", (float)(1000.00 / frametime));
#endif
	display_text(0, 1, text2print);
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
	int i, j;

	for (i = 0; i < sizeof(cube3d_1) / sizeof(t_poly3d); i++) {

		cube2d[i].draw_it = 1;
		if (cube3d_1[i].draw_it == 0) {
			cube2d[i].draw_it = 0;
			continue;
		}

		for (j = 0; j < sizeof(cube3d_1[j].points) / sizeof(t_point3d); j++) {
			cube2d[i].points[j].x = ((cube3d_1[i].points[j].x) * persp_value) / cube3d_1[i].points[j].z;
			cube2d[i].points[j].y = ((cube3d_1[i].points[j].y) * persp_value) / cube3d_1[i].points[j].z;
		}
	}

}

void draw2d(void)
{
	SDL_Point points_tmp[3 + 1] = { 0 };
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

void create_lot_cubes(void)
{
#define BLOCKSIZE 1		//10^3 cubes
#define CUBESIDE 200
#define CUBEPOLYS (2 * 6)
#define DISTANCE CUBESIDE
	t_poly3d *ppoly;
	int i, j, k;


	for (k = 1, ppoly = cube3d; k < BLOCKSIZE; k++) {	//1 row of cubes
		for (i = 0; i < CUBEPOLYS; i++) {	//one cube copy x direction
			for (j = 0; j < 3; j++) {
				ppoly[i + CUBEPOLYS * k].points[j].x = ppoly[i].points[j].x + (CUBESIDE + DISTANCE)*k;
				ppoly[i + CUBEPOLYS * k].points[j].y = ppoly[i].points[j].y;
				ppoly[i + CUBEPOLYS * k].points[j].z = ppoly[i].points[j].z;
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
