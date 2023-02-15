#ifndef _SCENE_H
#define _SCENE_H

#include <stdlib.h>

#define BLOCK_WIDTH 4

#define WORLD_HEIGHT 120
#define WORLD_WIDTH 160



extern int WORLD[WORLD_HEIGHT][WORLD_WIDTH];
extern int SCENE[60][80];
extern int camera_x;
extern int camera_y;
//extern coord camera_center;

void get_scene();

void update_camera_center(int x, int y);

int init_world();

void init_bg(int z_height);

#endif
