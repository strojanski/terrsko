#ifndef _SCENE_H
#define _SCENE_H

#include <stdlib.h>

#include "structures.h"

#define BLOCK_WIDTH 4

#define WORLD_HEIGHT 120
#define WORLD_WIDTH 160

#define SCENE_HEIGHT 60
#define SCENE_WIDTH 80


extern uint8_t WORLD[WORLD_HEIGHT][WORLD_WIDTH/2];
extern uint8_t SCENE[SCENE_HEIGHT][SCENE_WIDTH/2];
extern uint16_t camera_x;
extern uint16_t camera_y;

extern uint16_t GROUND_ZERO;

void get_scene();

void update_camera_center(uint16_t x, uint8_t y);

void init_world();

bool is_night();

void init_bg(uint16_t z_height);

#endif
