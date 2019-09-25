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


//definitions of external variables of this module (declared in module header)
//----------------------------------------------------------------------------

extern t_camera camera;

void move_camera(void);

void movement(void) {
	move_camera();
}

void move_camera(void) {
#define MILLIS_PER_SEC 1000.00
	static INT64 timer_move;
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

	rot_step = frametime * speed_cam_rot;
	SDL_PumpEvents();	//actualize events

						//use camera unit vectors


	if (pcntvals[CNT_CAM_ROT_RIGHT] || pcntvals[CNT_CAM_ROT_LEFT]) {
		if (pcntvals[CNT_CAM_ROT_RIGHT]) {
			camera.roty += rot_step;	//% 360
			if (camera.roty > 360)
				camera.roty -= 360;

		}

		if (pcntvals[CNT_CAM_ROT_LEFT]) {
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
	if (pcntvals[CNT_CAM_GO_FORWARD]) {
		//working with degrees of camera
		/*
		camera.z += (cos(camera.roty*val) * (frametime*speed_cam_move));
		camera.x += (sin(camera.roty*val) * (frametime*speed_cam_move));
		*/
		//working with unit vector of camera
		camera.z += (camera.uvect.vz * (frametime*speed_cam_move));
		camera.x += (camera.uvect.vx * (frametime*speed_cam_move));
	}
	if (pcntvals[CNT_CAM_GO_BACKWARD]) {
		/*
		camera.z -= (cos(camera.roty*val) * (frametime*speed_cam_move));
		camera.x -= (sin(camera.roty*val) * (frametime*speed_cam_move));
		*/

		camera.z -= (camera.uvect.vz * (frametime*speed_cam_move));
		camera.x -= (camera.uvect.vx * (frametime*speed_cam_move));
	}

	if (pcntvals[CNT_CAM_GO_UP]) {
		camera.y += (frametime*speed_cam_move);
	}
	if (pcntvals[CNT_CAM_GO_DOWN]) {
		camera.y -= (frametime*speed_cam_move);
	}

	/*
	//ESC
	if (pcntvals[CNT_ESCAPE]) {
		done_global = true;
	}
	//testkey
	if (pcntvals[CNT_BACKSPACE]) {
		menu_global = true;
	}
	*/
	//debug stuff
	if (keystates[SDL_SCANCODE_RETURN]) {
		dbg_global = true;
	}
	else
		dbg_global = false;




	//lightpos = camera
	/*
	scene3.lightpos.x = camera.x;
	scene3.lightpos.y = camera.y;
	scene3.lightpos.z = camera.z;
	*/

	StartCounter(&timer_move);


}
