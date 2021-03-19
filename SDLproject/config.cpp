#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "utils.h"
#include "config.h"

struct config_item {
	int id;
	int subsys;		//
	const char *name;
	int value_int;
	int changed;		//bit1:need to apply, bit2:need to save
};


struct config_item config_items[] {
	{ vsync, SUBSYS_SDL, "video_vsync", 1, FLAG_SAVE },
	{ fullscr, SUBSYS_SDL, "video_fullscreen", 0, FLAG_SAVE },
	{ sres_y, SUBSYS_SDL, "video_screen_res_y", 640, FLAG_SAVE },
	{ rres_y, SUBSYS_SDL, "video_render_res_y", 480, FLAG_SAVE },
	{ drawmode, SUBSYS_RND, "render_drawmode", 0, FLAG_SAVE }
};

const char *config_fname = "config.txt";


int get_conf_val(int conf_id)
{
	if (conf_id < 0 || conf_id >= ARRAY_SIZE(config_items))
		return -1;
	return config_items[conf_id].value_int;
}

int set_conf_val(int conf_id, int conf_val)
{
	if (conf_id < 0 || conf_id >= ARRAY_SIZE(config_items))
		return -1;
	//printf("%s; conf_id:%d, conf_val:%d\n", __func__, conf_id, conf_val);
	if (config_items[conf_id].value_int != conf_val) {
		config_items[conf_id].value_int = conf_val;
		config_items[conf_id].changed = FLAG_APPLY | FLAG_SAVE;
	}
	return 0;
}

int config_save(void)
{
	//simple version - does not retain comments
	FILE *fp_conf;
	char tmpstr[100];
	int i;
	char tmpfile[100] = { 0 };
	int retval = 0;

	// check if need to save
	if (is_config_changed(SUBSYS_ALL, FLAG_SAVE, false) == 0) {
		printf("nothing to save\n");
		return 0;
	}

	//open conf file for writing
	fopen_s(&fp_conf, config_fname, "w");
	if (fp_conf == nullptr) {
		printf("opening conf file \"%s\" failed!\n", config_fname);
		retval = -1;
		goto lab_savend;
	}

	//actual writing
	for (i = 0; i < ARRAY_SIZE(config_items); i++) {
		snprintf(tmpstr, sizeof(tmpstr), "%s:%d\n", config_items[i].name, config_items[i].value_int);
		if (fprintf(fp_conf, tmpstr) < 0) {
			retval = -1;
			printf("saving conf line \"%s\" failed!\n", tmpstr);
			break;
		}
	}
	fclose(fp_conf);

lab_savend:
	if (retval) {
		printf("saving config has been failed!\n");
	}
	else {
		is_config_changed(SUBSYS_ALL, FLAG_SAVE, 1);
		printf("config saved\n");
	}

	return retval;
}



int config_load()
{
	FILE *fp;
	char tmpstr[100];
	int i;
	int tmpval;

	//fp = fopen(config_fname, "r");
	fopen_s(&fp, config_fname, "r");
	if (fp == nullptr) {
		printf("opening config file \"%s\" failed!\n", config_fname);
		return -1;
	}
	while (fgets (tmpstr, sizeof(tmpstr), fp)) {
		char *datapos = strrchr(tmpstr, ':');
		int comment = tmpstr[0] == '#' ? 1 : 0;
		for (i = 0; comment == 0 && datapos != nullptr && i < ARRAY_SIZE(config_items); i++) {
			if (memcmp(tmpstr, config_items[i].name, strlen(config_items[i].name)) == 0) {
				tmpval = atoi(datapos + 1);
				config_items[i].value_int = tmpval;
				config_items[i].changed = 0;
				//printf("%s; text:%s, value:%d\n",__func__, config_items[i].name, config_items[i].value_int);
			}
		}
	}
	fclose(fp);

	return 0;
}

int is_config_changed(int subsys, int change_level, bool reset)
{
	int i;
	int retval = 0;
	subsys = (subsys == SUBSYS_ALL) ? ~subsys : subsys;
	for (i = 0; i < ARRAY_SIZE(config_items); i++) {
		if ((config_items[i].subsys & subsys) && (config_items[i].changed & change_level)) {
			retval = 1;
			if (reset == true)
				config_items[i].changed &= ~change_level;
			break;
		}
	}
	return retval;
}
