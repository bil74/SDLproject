//#pragma once
#ifndef _SOURCE_H_
#define _SOURCE_H_


//unit vector
typedef struct {
	double vx;
	double vy;
	double vz;
} t_vector;

typedef struct {
	double x;		//position
	double y;
	double z;
	double roty;	//rotation in degree (0-360)
	double rotx;
	double rotz;
	double curr_segment_3d;		//felosztjuk a teret kock�kra �s csak az aku�lis plusz szomsz�dos szegmenseket rajzoljuk meg
	t_vector uvect;	//unit vector
	t_vector uvect_default;	//unit vector default
} t_camera;

typedef struct {
	signed int x;
	signed int y;
	signed int z;
	signed int tz;	//transformed z
}t_point3d;

typedef struct {
	int x;
	int y;
}t_point2d;

typedef struct {
	signed int x;
	signed int y;
	bool valid;
}t_point2d_plus;



//v2 - this is used now
typedef struct {
	int point_id[3];
	SDL_Color color;
	int draw_it;
	t_vector uvect_normal;	//face normal unit vector
	float lightmag;			//light magnitude (0-1)
}t_poly;

//object v2
typedef struct {
	int num_points;
	int num_polys;
	t_point3d *points_3d;
	t_point2d_plus *points_2d;
	t_poly *polys;
}t_obj;

//scene v2
typedef struct {
	//some global shit
	//light sources
	//3d segment
	//drawing type (wireframe, solid, textured)
	t_point3d lightpos;
	int num_objects;
	t_obj *objects;
}t_scene;

//------------------------------------------------------------------------------------------------

//v3 stuff - poly oriented
//scene v2 is OK

typedef struct {	//must be working without knowing the object
	t_point3d points3d[3];	//original - no modify
	t_point2d points2d[3];	//calculated
	SDL_Color color;		//original - no modify
	int draw_it;
	t_vector uvect_normal;	//face normal unit vector - calculated once
	float lightmag;			//light magnitude (0-1)
	int camz;				//z of first point for sorting
}t_poly3;

typedef struct {
	int num_polys;
	t_poly3 *polys;
}t_obj3;

typedef struct {
	//some global shit
	//light sources
	//3d segment
	//drawing type (wireframe, solid, textured)
	t_point3d lightpos;
	int num_objects;
	t_obj3 *objects;
}t_scene3;

typedef struct {
	t_point2d points[4];
	SDL_Color color;
	int zdist;
}t_draw;








//------------------------------------------------------------------------------------------------
//v1 - not used - will be deleted


typedef struct {
	t_point3d points[3];
	int color;
	int draw_it;
	//norm�lis, stb
}t_poly3d;

typedef struct {
	t_point2d points[3];
	int color;
	int draw_it;
	//norm�lis, stb
}t_poly2d;




typedef struct {
	int draw_it;	//visible or not
	int relative;	//0: a koordin�t�k abszolutak, 0: a poly koordin�t�khoz hozz� kell adni az obj koordin�t�kat (+forgat�s)
	//int segment_3d;	//ez x,y,z koordin�ta lesz (pl. 10, 10, 10) es csak 9 db. szegmenst vesz�nk figyelembe renderel�sn�l
	int obj_x;
	int obj_y;
	int obj_z;
	int obj_rot_x;
	int obj_rot_y;
	int obj_rot_z;
	int num_of_polys;
	//polys here
}t_object_3d_header;

typedef struct {
	int num_of_objects;
	//objects here
}scene_segment_3d;

#endif //_SOURCE_H_

