#ifndef _SCENE_H
#define _SCENE_H

#include <stdlib.h>

#include "structures.h"
#include "materials.h"
#include "enums.h"
#include "ugui.h"

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

#define BLOCK_WIDTH 4

#define SCENE_HEIGHT 60
#define SCENE_WIDTH 80

#define WORLD_HEIGHT ((uint16_t) SCENE_HEIGHT * 4) //120
#define WORLD_WIDTH ((uint16_t) SCENE_WIDTH * 5) //160

#define WORLD_MAP_HEIGHT WORLD_HEIGHT
#define WORLD_MAP_WIDTH WORLD_WIDTH / 2

#define N_LEVELS 9
#define SKY_GROUND_OFFSET 15
#define GROUND_SKY_RATIO WORLD_HEIGHT / 3	// how much of the world is sky
#define LEVEL_SMOOTHING_FACTOR 12
#define KERNEL_WIDTH 32
#define TERRAIN_STD_THRESH 3

#define CAMERA_SPEED 5
#define SE_SIZE_DILATION 3
#define SE_SIZE_EROSION 5

#define LIGHT_MAP_HEIGHT WORLD_HEIGHT
#define LIGHT_MAP_WIDTH (WORLD_WIDTH / 8)
#define LIGHT_RADIUS 16		// radius of light in blocks
#define LIGHT_DEGRADATION_RATE 0.95

#define HMAP_SAMPLES_PER_CELL 16	// One hmap cell determines height 16 blocks
#define CAVE_SAMPLES_PER_CELL 2	// One cave sample determines nxn blocks of cave

extern uint8_t WORLD[WORLD_HEIGHT][WORLD_WIDTH/2];
extern uint8_t SCENE[SCENE_HEIGHT][SCENE_WIDTH/2];
extern uint8_t** CAVE_MAP; 	//[WORLD_HEIGHT/CAVE_SAMPLES_PER_CELL][WORLD_WIDTH/(2*CAVE_SAMPLES_PER_CELL)];
extern int16_t HEIGHT_MAP[WORLD_WIDTH/HMAP_SAMPLES_PER_CELL+1][WORLD_WIDTH/HMAP_SAMPLES_PER_CELL+1];
extern int16_t LVL1_HMAP[WORLD_WIDTH];
extern uint8_t LIGHT_MAP[WORLD_HEIGHT][WORLD_WIDTH/8];	// 1 cell = 8 blocks, 1 bit for each

extern uint16_t camera_x;
extern uint16_t camera_y;

void get_scene();

void update_camera_center(uint16_t x, uint16_t y);

void init_world();

bool is_night();

void init_light_map();

uint8_t count_lit_neighbors(uint16_t x, uint16_t y, uint8_t range);

float manhattan_dist(int8_t x, int8_t y);

float euclidean_dist(int8_t x, int8_t y);

coord convert_world_to_light(uint16_t x, uint16_t y);

float compute_illumination(uint16_t x, uint16_t y);

float light_intensity(float dist);

void precompute_euclidean();

float get_euclidean(uint8_t manhattan_dist);

void precompute_light_intensity();

float get_light_intensity(uint8_t manhattan_dist);

void init_stage_0();

void init_stage_1();

void place_trees();

void generate_caves();

void draw_blob(uint16_t x, uint16_t y, uint16_t radius, uint8_t value);

void draw_circle(uint16_t x, uint16_t y, uint8_t radius, uint8_t cave_value);

void generate_height_map(uint8_t random_lower, uint8_t random_upper, float roughness);

float* gauss_kernel(uint8_t width, uint8_t sigma);

void place_lava();

void shape_caves_with_morphological_operations(uint8_t dirt, uint8_t foreground);

void erosion(uint8_t SE[SE_SIZE_EROSION][SE_SIZE_EROSION], uint16_t map_width, uint16_t map_height, uint8_t foreground);

void dilation(uint8_t SE[SE_SIZE_DILATION][SE_SIZE_DILATION], uint16_t map_width, uint16_t map_height, uint8_t dirt, uint8_t foreground);

void filter_level(uint16_t array_size, uint8_t kernel_width, uint8_t sigma, bool only_sharp_edges);

uint8_t random_int(uint8_t min, uint8_t max);

#endif
