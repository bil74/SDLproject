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
#define CUBES_V3 200

extern int screen_res_x;
extern int screen_res_y;
extern SDL_Window* window;
extern SDL_Renderer* renderer;

//timers
INT64 timer_main = 0;
INT64 timer_refr_txt = 0;
INT64 timer_frame = 0;

//some colors
//SDL_Color col_objects = { 100, 100, 100, 255 };		//grey
SDL_Color col_objects = { 255, 255, 255, 255 };		//white
//SDL_Color col_objects = { 0, 0, 0, 255 };		//black
SDL_Color col_texts = { 255, 159, 114, 255 };			//brown


//v3
t_poly3 cube_polys3[2 * 6] = {
	{ { {-100, -100, -100}, {-100, 100, -100}, { 100, 100, -100} }, { { 0,0 }, { 0,0 }, { 0,0 } }, col_objects, 0, { 0,0,0 }, 0, 0 },
    { { {100,100,-100}, {100,-100,-100}, { -100,-100,-100} }, { { 0,0 }, { 0,0 }, { 0,0 } }, col_objects, 0, { 0,0,0 }, 0, 0 },
	{ { {100,100,100}, {-100,100,100}, { -100,-100,100} }, { { 0,0 }, { 0,0 }, { 0,0 } }, col_objects, 0, { 0,0,0 }, 0, 0 },
	{ { {-100,-100,100}, {100,-100,100}, { 100,100,100} }, { { 0,0 }, { 0,0 }, { 0,0 } }, col_objects, 0, { 0,0,0 }, 0, 0 },
	{ { {100,100,-100}, {100,100,100}, { 100,-100,100} }, { { 0,0 }, { 0,0 }, { 0,0 } }, col_objects, 0, { 0,0,0 }, 0, 0 },
	{ { {100,-100,-100}, {100,100,-100}, { 100,-100,100} }, { { 0,0 }, { 0,0 }, { 0,0 } }, col_objects, 0, { 0,0,0 }, 0, 0 },
	{ { {-100,-100,100}, {-100,100,100}, {-100,100,-100} }, { { 0,0 }, { 0,0 }, { 0,0 } }, col_objects, 0, { 0,0,0 }, 0, 0 },
	{ { {-100,100,-100}, {-100,-100,-100}, { -100,-100,100} }, { { 0,0 }, { 0,0 }, { 0,0 } }, col_objects, 0, { 0,0,0 }, 0, 0 },
	{ { {-100,100,100}, {100,100,100}, { 100,100,-100} }, { { 0,0 }, { 0,0 }, { 0,0 } }, col_objects, 0, { 0,0,0 }, 0, 0 },
	{ { {100,100,-100}, {-100,100,-100}, {-100,100,100} }, { { 0,0 }, { 0,0 }, { 0,0 } }, col_objects, 0, { 0,0,0 }, 0, 0 },
	{ { {100,-100,-100}, {100,-100,100}, { -100,-100,100} }, { { 0,0 }, { 0,0 }, { 0,0 } }, col_objects, 0, { 0,0,0 }, 0, 0 },
	{ { {-100,-100,100}, {-100,-100,-100}, {100,-100,-100} }, { { 0,0 }, { 0,0 }, { 0,0 } }, col_objects, 0, { 0,0,0 }, 0, 0 }
};


t_obj3 objs_v3[CUBES_V3] = {
	{2 * 6, cube_polys3 }
};

t_scene3 scene3 = {
	{10000, 10000, -10000},	//light source position (lehet elég lenne csak vmi irány?)
	CUBES_V3,
	objs_v3
};





t_camera camera = {
	0,0,-1500, 0,0,0,  0, {0,0,1}, {0,0,1}
};

double sindeg[360+1];
double cosdeg[360+1];

void testing(void);

int init_some_stuff(void);
void close_some_stuff(void);


t_vector vector_unitize(t_vector input);
t_vector vector_crossprod(t_vector inputv1, t_vector inputv2);
double vector_dotprod(t_vector inputv1, t_vector inputv2);

//v3 stuff
int create_lot_cubes_v3(void);
void render_scene_v3(t_scene3 *scene);
void do_3d_to_2d_v3(t_scene3 *scene);
int draw2d_v3(t_scene3 *scene);
void draw_actual(void);

void drawpoly_wf_v3(t_draw *draw);
void drawpoly_fill(SDL_Point *points, SDL_Color color);
void drawpoly_fill_v3(t_draw *draw);
int draw_memline(SDL_Point point_a, SDL_Point point_b, SDL_Point *memloc);
void sort_polys(void);


void movement(void);
void move_camera(void);
void getkeys(void);



SDL_bool done_global = SDL_FALSE;
SDL_bool dbg_global = SDL_FALSE;

//t_poly3 *p_glb_polys2sort;
t_draw *p_glb_polys2sort;
int glb_max_polys;
int glb_polycount;



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

				render_scene_v3(&scene3);

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
	if (create_lot_cubes_v3() == -1)
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


	//fill v3 data
		//calculate normal vectors of initial object
	{
		int i, j;
		double ax, ay, az, bx, by, bz, cx, cy, cz;
		t_vector vect1, vect2, crossvect, normvect;
		for (j = 0; j < scene3.num_objects; j++) {
			for (i = 0; i < scene3.objects[j].num_polys; i++) {
				//fill 3 point's coordinates
				ax = scene3.objects[j].polys[i].points3d[0].x;
				ay = scene3.objects[j].polys[i].points3d[0].y;
				az = scene3.objects[j].polys[i].points3d[0].z;

				bx = scene3.objects[j].polys[i].points3d[1].x;
				by = scene3.objects[j].polys[i].points3d[1].y;
				bz = scene3.objects[j].polys[i].points3d[1].z;

				cx = scene3.objects[j].polys[i].points3d[2].x;
				cy = scene3.objects[j].polys[i].points3d[2].y;
				cz = scene3.objects[j].polys[i].points3d[2].z;

				if (0) {	//testing
					printf("poly %2d: (%d,%d,%d), (%d,%d,%d), (%d,%d,%d)", i, (int)ax, (int)ay, (int)az, (int)bx, (int)by, (int)bz, (int)cx, (int)cy, (int)cz);
					//printf("poly %2d: crossvect.vx=%f, crossvect.vy=%f, crossvect.vz=%f\n", i, crossvect.vx, crossvect.vy, crossvect.vz);
					//printf("poly %2d: normvect.vx=%f, normvect.vy=%f, normvect.vz=%f\n", i, normvect.vx, normvect.vy, normvect.vz);
					printf("\n");
				}



				vect1.vx = bx - ax;
				vect1.vy = by - ay;
				vect1.vz = bz - az;

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

				scene3.objects[j].polys[i].uvect_normal.vx = normvect.vx;
				scene3.objects[j].polys[i].uvect_normal.vy = normvect.vy;
				scene3.objects[j].polys[i].uvect_normal.vz = normvect.vz;

				if (0) {	//testing
					printf("poly %2d: (%d,%d,%d), (%d,%d,%d), (%d,%d,%d)", i, (int)ax, (int)ay, (int)az, (int)bx, (int)by, (int)bz, (int)cx, (int)cy, (int)cz);
					//printf("poly %2d: crossvect.vx=%f, crossvect.vy=%f, crossvect.vz=%f\n", i, crossvect.vx, crossvect.vy, crossvect.vz);
					//printf("poly %2d: normvect.vx=%f, normvect.vy=%f, normvect.vz=%f\n", i, normvect.vx, normvect.vy, normvect.vz);
					printf("\n");
				}

			}
		}
	}





	//malloc mem for polys to sort
	//count max polys
	{
		int j;
		for (j = 0, glb_max_polys = 0; j < scene3.num_objects; j++) {
			glb_max_polys += scene3.objects[j].num_polys;
		}

		p_glb_polys2sort = (t_draw *)malloc(glb_max_polys * sizeof(t_draw));
		if (p_glb_polys2sort == NULL) {
			printf("malloc mem for polys to sort failed!\n");
			return -1;
		}

	}
	return 0;
}

void close_some_stuff(void)
{
	int k;

	//free mallocs
	for (k = 1; k < CUBES_V3; k++) {
		free(objs_v3[k].polys);
	}
	
	//free space for sorting polys
	free(p_glb_polys2sort);

}


//v3 stuff
void render_scene_v3(t_scene3 *scene_rsv3)
{
#define DSPTIME 1000						//refresh interval for frametime (millisec)
	static double frametime, sleeptime;
	static int num_of_frames;
	int polycount;



	//render cycle start
	do_3d_to_2d_v3(scene_rsv3);

	polycount = draw2d_v3(scene_rsv3);
	sort_polys();
	draw_actual();

	//init poly counter to sort
	glb_polycount = 0;

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
	{
		char text2print1[100];
		char text2print2[100];
		char text2print3[100];
#ifndef _LINUX_
		sprintf_s(text2print1, sizeof(text2print1), "cam x=%.0f, y=%.0f, z=%.0f, rot-x:%.0f, rot-y:%.0f, rot-z:%.0f", camera.x, camera.y, camera.z, camera.rotx, camera.roty, camera.rotz);
		sprintf_s(text2print2, sizeof(text2print2), "fps: %.2f, cubes: %d, polys drawn: %d", (float)(1000.00 / frametime), CUBES_V3, polycount);				//float
		sprintf_s(text2print3, sizeof(text2print3), "camera unit vector x=%.2f, y=%.2f, z=%.2f", camera.uvect.vx, camera.uvect.vy, camera.uvect.vz);
#else
		sprintf(text2print1, "cam x=%.0f, y=%.0f, z=%.0f, rot-x:%.0f, rot-y:%.0f, rot-z:%.0f", camera.x, camera.y, camera.z, camera.rotx, camera.roty, camera.rotz);
		sprintf(text2print2, "fps: %.2f, cubes: %d, polys drawn: %d", (float)(1000.00 / frametime), CUBES_V3, polycount);				//float
		sprintf(text2print3, "camera unit vector x=%.2f, y=%.2f, z=%.2f", camera.uvect.vx, camera.uvect.vy, camera.uvect.vz);
#endif
		display_text(0, 0, text2print1, col_texts);	//camera data
		display_text(0, 1, text2print2, col_texts);	//fps & co
		display_text(0, 2, text2print3, col_texts);	//OTHER
	}

}



//v3
void do_3d_to_2d_v3(t_scene3 *scene_3to2v3) {
	int i, j, k;
	double val;
	val = PI / 180;
	const int persp_value = 500;
	double x, y, z, tx, ty, tz;	//temporary values
	double cx = cos(camera.rotx*val);
	double sx = sin(camera.rotx*val);
	double cy = cos(camera.roty*val);
	double sy = sin(camera.roty*val);
	double cz = cos(camera.rotz*val);
	double sz = sin(camera.rotz*val);


	//backface culling
	t_poly3 tmpoly;
	t_vector vect2poly, normal;
	double dotprod;

	//TODO: use camera unit vectors to determine rotations?
	//get cam rot vals from its unit vector
	//int camrot_x, camrot_y, camrot_z;
	//camrot_y = asin(camera.uvect. ) * val;



	for (i = 0; i < scene_3to2v3->num_objects; i++) {					//objects


		for (j = 0; j < scene_3to2v3->objects[i].num_polys; j++) {		//polys

			//backface culling first

			//retrieve poly data
			tmpoly = scene_3to2v3->objects[i].polys[j];

			//face normal vector
			normal.vx = tmpoly.uvect_normal.vx;
			normal.vy = tmpoly.uvect_normal.vy;
			normal.vz = tmpoly.uvect_normal.vz;

			//camera to face vector (no need to find center of the polygon, so first point is OK)
			vect2poly.vx = tmpoly.points3d[0].x - (int)camera.x;
			vect2poly.vy = tmpoly.points3d[0].y - (int)camera.y;
			vect2poly.vz = tmpoly.points3d[0].z - (int)camera.z;

			//get dot product
			dotprod = vector_dotprod(vect2poly, normal);

			//face visible or not?
			tmpoly.draw_it = (dotprod > 0.0f) ? 0 : 1;

			//light source
			vect2poly.vx = tmpoly.points3d[0].x - scene_3to2v3->lightpos.x;
			vect2poly.vx = tmpoly.points3d[0].y - scene_3to2v3->lightpos.y;
			vect2poly.vx = tmpoly.points3d[0].z - scene_3to2v3->lightpos.z;
			//unitize
			vect2poly = vector_unitize(vect2poly);

			//get dot product for light
			dotprod = vector_dotprod(vect2poly, normal);
			tmpoly.lightmag = (float)(dotprod + 2.00);	//value is from -1 to 1 originally
			if (0 && dbg_global == SDL_TRUE) {
				printf("lightmag %f\n", tmpoly.lightmag);
			}












			//visible, continue transforming...
			if (tmpoly.draw_it == 1) {
				//transform 1 poly
				//for (k = 0; k < 3, tmpoly.draw_it == 1; k++) {		//1 poly
				for (k = 0; k < 3; k++) {		//1 poly

				//apply camera position

					x = tmpoly.points3d[k].x - (int)camera.x;
					y = tmpoly.points3d[k].y - (int)camera.y;
					z = tmpoly.points3d[k].z - (int)camera.z;

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

					//save transformed z for sorting
					if (k == 0)
						tmpoly.camz = (int)z;
					else
						 tmpoly.camz = (tmpoly.camz + (int)z) / 2;

					//transform 3d to 2d
					if (z <= 0) {
						tmpoly.draw_it = 0;
					}
					else {
						//transform 3d to 2d
						//tmpoly.draw_it = 1;
						tmpoly.points2d[k].x = (int)((x * persp_value) / z);
						tmpoly.points2d[k].y = (int)((y * persp_value) / z);
						if (0 && dbg_global == SDL_TRUE) {
							printf("2d x:%d, y:%d (z:%d)\n", tmpoly.points2d[k].x, tmpoly.points2d[k].x, (int)z);
						}

					}
				}


			}

			//save poly data
			scene_3to2v3->objects[i].polys[j] = tmpoly;
			/*
			//save for sorting
			if ((tmpoly.draw_it == 1) && (glb_polycount < glb_max_polys)) {
				*(p_glb_polys2sort + glb_polycount) = tmpoly;
				glb_polycount++;
			}
			*/
			















		}	//3d to 2d transform OK (for one poly)

	}

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

void draw_actual(void)
{
	int i;
	for (i = 0; i < glb_polycount; i++) {
		drawpoly_fill_v3(p_glb_polys2sort + i);
		//drawpoly_wf_v3(p_glb_polys2sort + i);
	}
}
//init poly counter to sort

//v3
int draw2d_v3(t_scene3 *scene_d2v3)
{
	SDL_Point points_tmp[3 + 1] = { 0 };
	SDL_Point points_norm[2] = { 0 };
	int i, j, k;
	int out_of_screen_points;
	int polys_drawn = 0;
	SDL_Color drawcolor;
	t_draw tmpdraw;


	//printf("draw2d_v2 scene_d2v2.objects[0].num_polys:%d\n", scene_d2v2.objects[0].num_polys);
	for (k = 0; k < scene_d2v3->num_objects; k++) {
		for (i = 0; i < scene_d2v3->objects[k].num_polys; i++) {
			out_of_screen_points = 0;
			if (scene_d2v3->objects[k].polys[i].draw_it == 1) {
				for (j = 0; j < 3; j++) {
					//p = scene_d2v3->objects[k].polys[i].point_id[j];
					//printf("draw2d_v2 p:%d, ",p);
					points_tmp[j].x = tmpdraw.points[j].x = scene_d2v3->objects[k].polys[i].points2d[j].x + (screen_res_x / 2);
					points_tmp[j].y = tmpdraw.points[j].y = -(scene_d2v3->objects[k].polys[i].points2d[j].y) + (screen_res_y / 2);	//-: sdl y has positive downwards;
					//printf("draw2d_v2 p%dx:%d, p%dy:%d; ", p, points_tmp[j].x, p, points_tmp[j].y);
					if (points_tmp[j].x < 0 || points_tmp[j].x > screen_res_x)
						out_of_screen_points++;
					else if (points_tmp[j].y < 0 || points_tmp[j].y > screen_res_y)
						out_of_screen_points++;

				}

				if (0 && dbg_global == SDL_TRUE) {
					//printf("object %d | poly %d | vect2poly (%.2f,%.2f,%.2f), normal (%.2f,%.2f,%.2f), dotprod %.2f\n", i, j, vect2poly.vx, vect2poly.vy, vect2poly.vz, normal.vx, normal.vy, normal.vz, dotprod);
					//printf("object %d | poly %2d | drawit %s\n", k, i, scene_d2v3->objects[k].polys[i].draw_it ? "true " : "false");
				}

				//calc color
				{
					float lm = scene_d2v3->objects[k].polys[i].lightmag;
					drawcolor = scene_d2v3->objects[k].polys[i].color;

					drawcolor.r = (int)((float)drawcolor.r / lm);
					drawcolor.g = (int)((float)drawcolor.g / lm);
					drawcolor.b = (int)((float)drawcolor.b / lm);

				}

				/*
				//save for sorting
				if ((tmpoly.draw_it == 1) && (glb_polycount < glb_max_polys)) {
					*(p_glb_polys2sort + glb_polycount) = tmpoly;
					glb_polycount++;
				}
				*/

				//dont draw triangle if all points are outside of the window
				if (out_of_screen_points < 3) {
					points_tmp[3].x = tmpdraw.points[3].x = points_tmp[0].x;
					points_tmp[3].y = tmpdraw.points[3].y = points_tmp[0].y;
					tmpdraw.color = drawcolor;
					tmpdraw.zdist = scene_d2v3->objects[k].polys[i].camz;
					*(p_glb_polys2sort + glb_polycount) = tmpdraw;
					glb_polycount++;
					//drawpoly_fill(points_tmp, drawcolor);
					//drawpoly_wf(points_tmp, col_objects);
					polys_drawn++;
					if (0 && dbg_global == SDL_TRUE) {
						printf("draw2d_v3 p1(%d,%d), p2(%d,%d), p3(%d,%d)\n", points_tmp[0].x, points_tmp[0].y, points_tmp[1].x, points_tmp[1].y, points_tmp[2].x, points_tmp[2].y);
					}
				}
			}	//printf("\n");
		}

		if (0 && dbg_global == SDL_TRUE) {
			//printf("object %d | poly %d | vect2poly (%.2f,%.2f,%.2f), normal (%.2f,%.2f,%.2f), dotprod %.2f\n", i, j, vect2poly.vx, vect2poly.vy, vect2poly.vz, normal.vx, normal.vy, normal.vz, dotprod);
			printf("\n");
		}
	}

	return polys_drawn;

}




void drawpoly_wf_v3 (t_draw *draw)
//wireframe draw
{
	SDL_Color tmpcolor;
	//save color
	SDL_GetRenderDrawColor(renderer, &tmpcolor.r, &tmpcolor.g, &tmpcolor.b, &tmpcolor.a);
	//set color
	SDL_SetRenderDrawColor(renderer, draw->color.r, draw->color.g, draw->color.b, draw->color.a);

	if (0 && dbg_global == SDL_TRUE) {
		printf("point 1:(%d,%d), point 2:(%d,%d), point 3:(%d,%d), point 4:(%d,%d)\n", draw->points[0].x, draw->points[0].y, draw->points[1].x, draw->points[1].y, draw->points[2].x, draw->points[2].y, draw->points[3].x, draw->points[3].y);
	}

	//draw
	SDL_RenderDrawLines(renderer, (SDL_Point*)draw->points, 3 + 1);
	

	//restore color
	SDL_SetRenderDrawColor(renderer, tmpcolor.r, tmpcolor.g, tmpcolor.b, tmpcolor.a);
}




void drawpoly_fill_v3(t_draw *draw)
//flat color draw
{
	SDL_Color tmpcolor;
	SDL_Point p_ord[3] = { 0 };
	SDL_Point p_tmp;
	int i, j = 0;
	int ymin = 0;
	SDL_Point *line1;
	SDL_Point *line2;
	int ymax;


	//save color
	SDL_GetRenderDrawColor(renderer, &tmpcolor.r, &tmpcolor.g, &tmpcolor.b, &tmpcolor.a);
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
	line1 = (SDL_Point *)malloc((ymax + 2) * sizeof(SDL_Point));
	line2 = (SDL_Point *)malloc((ymax + 2) * sizeof(SDL_Point));
	if (line1 == NULL || line2 == NULL) {
		printf("drawpoly_fill malloc failed!\n");
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
			SDL_RenderDrawLines(renderer, tmpoints, 2);
		}
		if (0 && dbg_global == SDL_TRUE)
			printf("\n");
	}



	//restore color
	SDL_SetRenderDrawColor(renderer, tmpcolor.r, tmpcolor.g, tmpcolor.b, tmpcolor.a);

	//free mallocs
	free(line1);
	free(line2);

	return;
}










void drawpoly_fill(SDL_Point *points, SDL_Color color)
//flat color draw
{
	SDL_Color tmpcolor;
	SDL_Point p_ord[3] = {0};
	SDL_Point p_tmp;
	int i,j = 0;
	int ymin = 0;
	SDL_Point *line1;
	SDL_Point *line2;
	int ymax;


	//save color
	SDL_GetRenderDrawColor(renderer, &tmpcolor.r, &tmpcolor.g, &tmpcolor.b, &tmpcolor.a);
	//set color
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	
	memcpy(p_ord, points, sizeof(SDL_Point) * 3);

	if (0 && dbg_global == SDL_TRUE) {
		printf("point orig 1:(%d,%d), point 2:(%d,%d), point 3:(%d,%d)\n", points[0].x, points[0].y, points[1].x, points[1].y, points[2].x, points[2].y);
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
		printf("point sort a:(%d,%d), point b:(%d,%d), point c:(%d,%d), ymax %d\n", p_ord[0].x, p_ord[0].y, p_ord[1].x, p_ord[1].y, p_ord[2].x, p_ord[2].y, ymax);
	}


	//allocate memory for two virtual lines
	line1 = (SDL_Point *)malloc((ymax+2) * sizeof(SDL_Point));
	line2 = (SDL_Point *)malloc((ymax+2) * sizeof(SDL_Point));
	if (line1 == NULL || line2 == NULL) {
		printf("drawpoly_fill malloc failed!\n");
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
			SDL_RenderDrawLines(renderer, tmpoints, 2);
		}
		if (0 && dbg_global == SDL_TRUE)
			printf("\n");
	}



	//restore color
	SDL_SetRenderDrawColor(renderer, tmpcolor.r, tmpcolor.g, tmpcolor.b, tmpcolor.a);

	//free mallocs
	free(line1);
	free(line2);

	return;
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
		(memloc + i)->y = (memloc + i-1)->y+1;
		xfloat = xfloat + xadd;
		(memloc + i)->x = (int)(roundf(xfloat));
		if (0 && dbg_global == SDL_TRUE)
			printf("memline inner | (memloc + i)x=%d,y=%d\n", (memloc + i)->x, (memloc + i)->y);
	}

	if (0 && dbg_global == SDL_TRUE)
		printf("memline return %d\n", i);
	return i;
}


int create_lot_cubes_v3(void)
{
#define BLOCKSIZE_V3 10		//10^3 cubes
#define CUBESIDE_V3 200
#define CUBEPOLYS_V3 (2 * 6)
#define DISTANCE_V3 CUBESIDE_V3
	int i, j, k;

	//v2
	//t_point3d *p_cube_points_3d;
	//t_point2d *p_cube_points_2d;
	t_poly3 *p_cube_polys;
	SDL_Color tmpcol;
	//t_poly *cube_polyconns;
	int num_objects = 1;
	int rand_a, rand_b, rand_c;
#define MAXDISTANCE_V3 (BLOCKSIZE_V3 * DISTANCE_V3 * 2 + CUBES_V3)
	//#define MAXDISTANCE_V2 (BLOCKSIZE_V2 + DISTANCE_V2 *  CUBES_V2)

	//randomnumber = rand() % MAXDISTANCE;
	for (k = 1; k < CUBES_V3; k++) {
		//copy
		p_cube_polys = (t_poly3 *)malloc(12 * sizeof(t_poly3));
		if (p_cube_polys == NULL)
			return -1;
		objs_v3[k].num_polys = objs_v3[0].num_polys;
		objs_v3[k].polys = p_cube_polys;
		memcpy(objs_v3[k].polys, objs_v3[0].polys, 12 * sizeof(t_poly3));

		//random position
		rand_a = rand() % MAXDISTANCE_V3;
		rand_b = rand() % MAXDISTANCE_V3;
		rand_c = rand() % MAXDISTANCE_V3;
		for (i = 0; i < objs_v3[0].num_polys; i++) {
			for (j = 0; j < 3; j++) {
				objs_v3[k].polys[i].points3d[j].x = objs_v3[0].polys[i].points3d[j].x + rand_a;
				objs_v3[k].polys[i].points3d[j].y = objs_v3[0].polys[i].points3d[j].y + rand_b;
				objs_v3[k].polys[i].points3d[j].z = objs_v3[0].polys[i].points3d[j].z + rand_c;
			}
		}
		//random color
		rand_a = rand() % 255 - 50;
		rand_b = rand() % 255 - 50;
		rand_c = rand() % 255 - 50;
		tmpcol.r = rand_a + 50;
		tmpcol.g = rand_b + 50;
		tmpcol.b = rand_c + 50;
		tmpcol.a = 255;
		for (i = 0; i < objs_v3[k].num_polys; i++) {
			objs_v3[k].polys[i].color = tmpcol;
		}
	}

	return 0;
}





































t_vector vector_crossprod(t_vector inputv1, t_vector inputv2)
//https://github.com/OneLoneCoder/videos/blob/master/OneLoneCoder_olcEngine3D_Part3.cpp
{
	t_vector retvect;	//returned vector
	//retvect.vx = inputv1.vy * inputv2.vz - inputv1.vz * inputv2.vy;
	//retvect.vy = inputv1.vx * inputv2.vz - inputv1.vz * inputv2.vx;
	//retvect.vz = inputv1.vx * inputv2.vx - inputv1.vy * inputv2.vx;

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

