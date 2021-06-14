#ifndef _CONFIG_H_
#define _CONFIG_H_

#define FLAG_APPLY 1
#define FLAG_SAVE 2

#define SUBSYS_ALL 0
#define SUBSYS_SDL 1
#define SUBSYS_RND 2

typedef enum {
	vsync = 0,
	fullscr,
	sres_y,
	rres_y,
	drawmode
} conf_ids;

//
extern int config_load();
extern int config_save();

int get_conf_val(int conf_id);
int set_conf_val(int conf_id, int conf_val);
int is_config_changed(int subsys, int change_level, SDL_bool reset);

#endif //_CONFIG_H_
