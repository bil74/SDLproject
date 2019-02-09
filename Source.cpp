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
t_poly cube_polys[2 * 6] = { { 0,1,2 ,0,0, {0,0,0}}, { 2,3,0 ,0,0,{ 0,0,0 } },  { 6,5,4 ,0,0,{ 0,0,0 } }, { 4,7,6 ,0,0,{ 0,0,0 } },
							{ 2,6,7 ,0,0, { 0,0,0 } }, { 3,2,7 ,0,0,{ 0,0,0 } },  { 4,5,1 ,0,0,{ 0,0,0 } }, { 1,0,4 ,0,0,{ 0,0,0 } },
							{ 5,6,2 ,0,0,{ 0,0,0 } }, { 2,1,5 ,0,0,{ 0,0,0 } },  { 3,7,4 ,0,0,{ 0,0,0 } }, { 4,0,3 ,0,0,{ 0,0,0 } } };

t_obj objs_v2[CUBES_V2] = {
{8, 2 * 6, cube_points_3d, cube_points_2d, cube_polys}
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
	0,0,-1500, 0,0,0,  0, {0,0,1}, {0,0,1}
};

double sindeg[360+1];
double cosdeg[360+1];

void testing(void);

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
int draw2d_v2(t_scene *scene);
t_vector vector_unitize(t_vector input);
t_vector vector_crossprod(t_vector inputv1, t_vector inputv2);
double vector_dotprod(t_vector inputv1, t_vector inputv2);

void movement(void);
void move_camera(void);
void getkeys(void);



SDL_bool done_global = SDL_FALSE;
SDL_bool dbg_global = SDL_FALSE;



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
	
	if (create_lot_cubes_v2() == -1)
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
	//don't forget to duplicate them in create_lot_cubes_v2!!!
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
	int polycount;
	int i, j;



	//render cycle start
	do_3d_to_2d(scene_rsv2);

	for (i = 0; i < scene_rsv2->num_objects; i++) {
		for (j = 0; j < scene_rsv2->objects[i].num_polys; j++) {
			if (0 && dbg_global == SDL_TRUE) {
				//printf("object %d | poly %d | vect2poly (%.2f,%.2f,%.2f), normal (%.2f,%.2f,%.2f), dotprod %.2f\n", i, j, vect2poly.vx, vect2poly.vy, vect2poly.vz, normal.vx, normal.vy, normal.vz, dotprod);
				printf("object %d | poly %2d | drawit %s\n", i, j, scene_rsv2->objects[i].polys[j].draw_it ? "true " : "false");
			}
		}
		if (0 && dbg_global == SDL_TRUE)
			printf("\n");
	}

	polycount = draw2d_v2(scene_rsv2);
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
	sprintf_s(text2print, sizeof(text2print), "fps: %.2f, cubes: %d, polys drawn: %d", (float)(1000.00 / frametime), CUBES_V2, polycount);				//float
#else
	sprintf(text2print, "fps: %.2f, cubes: %d, polys drawn: %d", (float)(1000.00 / frametime), CUBES_V2, polycount);				//float
#endif
	display_text(0, 1, text2print);


	//OTHER
	//sprintf_s(text2print, sizeof(text2print), "fps %d", (int)roundf((float)(1000.00 / frametime)));	//int
#ifndef _LINUX_
	sprintf_s(text2print, sizeof(text2print), "camera unit vector x=%.2f, y=%.2f, z=%.2f", camera.uvect.vx, camera.uvect.vy, camera.uvect.vz);
#else
	sprintf(text2print, "camera unit vector x=%.2f, y=%.2f, z=%.2f", camera.uvect.vx, camera.uvect.vy, camera.uvect.vz);
#endif
	display_text(0, 2, text2print);


}




void do_3d_to_2d(t_scene *scene_3to2v2) {
	int i, j;
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
	t_poly tmpoly;
	t_vector vect2poly, normal;
	double dotprod;

	//TODO: use camera unit vectors to determine rotations?
	//get cam rot vals from its unit vector
	//int camrot_x, camrot_y, camrot_z;
	//camrot_y = asin(camera.uvect. ) * val;
	
	for (i = 0; i < scene_3to2v2->num_objects; i++) {					//all objects


		for (j = 0; j < scene_3to2v2->objects[i].num_points; j++) {		//1 object pontjai
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
				scene_3to2v2->objects[i].points_2d[j].x = (int)((x * persp_value) / z);
				scene_3to2v2->objects[i].points_2d[j].y = (int)((y * persp_value) / z);
				//printf("; 2d x:%d, y:%d (z:%d)\n", scene_3to2v2->objects[0].points_2d[j].x, scene_3to2v2->objects[0].points_2d[j].y, z);

			}

		}	//3d to 2d transform OK (for one object)

		
		//visibility (backface culling) for polygons
		for (j = 0; j < scene_3to2v2->objects[i].num_polys; j++) {		//1 object poligonjai
			
			//retrieve poly data
			tmpoly = scene_3to2v2->objects[i].polys[j];

			//face normal vector
			normal.vx = tmpoly.uvect_normal.vx;
			normal.vy = tmpoly.uvect_normal.vy;
			normal.vz = tmpoly.uvect_normal.vz;

			
			//camera to face vector (no need to find center of the polygon, so first point is OK)
			vect2poly.vx = scene_3to2v2->objects[i].points_3d[tmpoly.point_id[0]].x - (int)camera.x;
			vect2poly.vy = scene_3to2v2->objects[i].points_3d[tmpoly.point_id[0]].y - (int)camera.y;
			vect2poly.vz = scene_3to2v2->objects[i].points_3d[tmpoly.point_id[0]].z - (int)camera.z;
			
			//get dot product
			dotprod = vector_dotprod(vect2poly, normal);
			
			//face visible or not?
			tmpoly.draw_it = (dotprod > 0.0f) ? 0 : 1;

			//save poly data
			scene_3to2v2->objects[i].polys[j] = tmpoly;

		}	//bf culling cycle end (for one object)

	}

}





int draw2d_v2(t_scene *scene_d2v2)
{
	SDL_Point points_tmp[3 + 1] = { 0 };
	SDL_Point points_norm[2] = { 0 };
	int i, j, k;
	int out_of_screen_points;
	int polys_drawn = 0;


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

			if (0 && dbg_global == SDL_TRUE) {
				//printf("object %d | poly %d | vect2poly (%.2f,%.2f,%.2f), normal (%.2f,%.2f,%.2f), dotprod %.2f\n", i, j, vect2poly.vx, vect2poly.vy, vect2poly.vz, normal.vx, normal.vy, normal.vz, dotprod);
				printf("object %d | poly %2d | drawit %s\n", k, i, scene_d2v2->objects[k].polys[i].draw_it ? "true " : "false");
			}

			//dont draw triangle if all points are outside of the window
			if ((out_of_screen_points < 3) && (scene_d2v2->objects[k].polys[i].draw_it == 1)) {
				points_tmp[3].x = points_tmp[0].x;
				points_tmp[3].y = points_tmp[0].y;
				SDL_RenderDrawLines(renderer, points_tmp, 3 + 1);
				polys_drawn++;
				//printf("draw2d_v2 p3x:%d, p3y:%d; ", points_tmp[3].x, points_tmp[3].y);
			}
			//printf("\n");
		}

		if (0 && dbg_global == SDL_TRUE) {
			//printf("object %d | poly %d | vect2poly (%.2f,%.2f,%.2f), normal (%.2f,%.2f,%.2f), dotprod %.2f\n", i, j, vect2poly.vx, vect2poly.vy, vect2poly.vz, normal.vx, normal.vy, normal.vz, dotprod);
			printf("\n");
		}
	}

	return polys_drawn;

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
	t_poly *p_cube_polys;
	//t_poly *cube_polyconns;
	int num_objects = 1;
	int rand_x, rand_y, rand_z;
	#define MAXDISTANCE_V2 (BLOCKSIZE_V2 * DISTANCE_V2 * 2 + CUBES_V2)
	//#define MAXDISTANCE_V2 (BLOCKSIZE_V2 + DISTANCE_V2 *  CUBES_V2)

	//randomnumber = rand() % MAXDISTANCE;
	for (k = 1; k < CUBES_V2; k++) {
		objs_v2[k].num_points = objs_v2[0].num_points;
		objs_v2[k].num_polys = objs_v2[0].num_polys;
		objs_v2[k].polys = cube_polys;	//global stuff
		objs_v2[k].points_2d = cube_points_2d;
		p_cube_points_3d = (t_point3d *)malloc(8 * sizeof(t_point3d));
		p_cube_points_2d = (t_point2d *)malloc(8 * sizeof(t_point2d));
		p_cube_polys = (t_poly *)malloc(12 * sizeof(t_poly));
		if (p_cube_points_3d == NULL || p_cube_points_2d == NULL || p_cube_polys == NULL)
			return -1;
		objs_v2[k].points_2d = p_cube_points_2d;
		objs_v2[k].points_3d = p_cube_points_3d;
		objs_v2[k].polys = p_cube_polys;
		memcpy(objs_v2[k].polys, objs_v2[0].polys, 12 * sizeof(t_poly) );
		rand_x = rand() % MAXDISTANCE_V2;
		rand_y = rand() % MAXDISTANCE_V2;
		rand_z = rand() % MAXDISTANCE_V2;
		for (i = 0; i < objs_v2[0].num_points; i++) {
			objs_v2[k].points_3d[i].x = objs_v2[0].points_3d[i].x + rand_x;
			objs_v2[k].points_3d[i].y = objs_v2[0].points_3d[i].y + rand_y;
			objs_v2[k].points_3d[i].z = objs_v2[0].points_3d[i].z + rand_z;
		}
	}

	return 0;
}














































/*
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

*/

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

