#ifndef _SCENE_H
#define _SCENE_H

#include <stdlib.h>

#include "structures.h"
#include "materials.h"
#include "enums.h"
#include "ugui.h"

#define BLOCK_WIDTH 4

#define WORLD_HEIGHT (SCENE_HEIGHT * 3) //120
#define WORLD_WIDTH (SCENE_WIDTH * 3) //160

#define SCENE_HEIGHT 60
#define SCENE_WIDTH 80

#define N_LEVELS 9
#define SKY_GROUND_OFFSET 15
#define GROUND_SKY_RATIO WORLD_HEIGHT / 3	// how much of the world is ground
#define LEVEL_SMOOTHING_FACTOR 12
#define KERNEL_WIDTH 32

#define HMAP_SAMPLES_PER_CELL 16	// One hmap cell determines height 16 blocks
#define CAVE_SAMPLES_PER_CELL 2	// One cave sample determines 8x8 blocks of cave

#define CAMERA_SPEED 3
#define SE_SIZE 17

extern uint8_t WORLD[WORLD_HEIGHT][WORLD_WIDTH/2];
extern uint8_t SCENE[SCENE_HEIGHT][SCENE_WIDTH/2];
extern uint8_t CAVE_MAP[WORLD_HEIGHT/CAVE_SAMPLES_PER_CELL][WORLD_WIDTH/(2*CAVE_SAMPLES_PER_CELL)];
extern int8_t HEIGHT_MAP[WORLD_WIDTH/HMAP_SAMPLES_PER_CELL+1][WORLD_WIDTH/HMAP_SAMPLES_PER_CELL+1];
extern int8_t LVL1_HMAP[WORLD_WIDTH];
extern uint16_t camera_x;
extern uint16_t camera_y;

void get_scene();

void update_camera_center(uint16_t x, uint8_t y);

void init_world();

bool is_night();

void init_stage_0();

void generate_caves();

void draw_circle(uint16_t x, uint16_t y, uint8_t radius, uint8_t cave_value);

void generate_height_map(uint8_t random_lower, uint8_t random_upper, float roughness);

float* gauss_kernel(uint8_t width, uint8_t sigma);

void morphological_opening();

uint8_t** erosion(uint8_t** ground, uint8_t SE[SE_SIZE][SE_SIZE], uint16_t map_width, uint16_t map_height);

uint8_t** dilation(uint8_t** ground, uint8_t SE[SE_SIZE][SE_SIZE], uint16_t map_width, uint16_t map_height);

void filter_level(uint16_t array_size, uint8_t kernel_width, uint8_t sigma);

uint8_t random_int(uint8_t min, uint8_t max);

#endif
