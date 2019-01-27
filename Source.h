//#pragma once
#ifndef _SOURCE_H_
#define _SOURCE_H_

typedef struct {
	double x;
	double y;
	double z;
	double roty;//-180 -> +180 ?
	double rotx;
	double rotz;
	double curr_segment_3d;		//felosztjuk a teret kock�kra �s csak az aku�lis plusz szomsz�dos szegmenseket rajzoljuk meg
}t_camera;

typedef struct {
	signed int x;
	signed int y;
	signed int z;
}t_point3d;

typedef struct {
	signed int x;
	signed int y;
	bool valid;
}t_point2d;



//v2 - this is used now
typedef struct {
	int point_id[3];
	int color;
	int draw_it;
	//norm�lis, stb
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

