#ifndef _SCENE_H
#define _SCENE_H

#include <stdlib.h>
#include <stdbool.h>

#include "structures.h"
#include "materials.h"
#include "enums.h"
#include "ugui.h"
#include "environment_models.h"
#include "position.h"

#ifndef MAX
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#endif

#ifndef MIN
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#endif

#define BLOCK_WIDTH 4
#define CELL_WIDTH BLOCK_WIDTH * 2

// Number of visible blocks on screen
#define SCENE_HEIGHT_BLOCKS 60
#define SCENE_WIDTH_BLOCKS 80

#define SCENE_HEIGHT_CELLS SCENE_HEIGHT_BLOCKS
#define SCENE_WIDTH_CELLS SCENE_WIDTH_BLOCKS / 2

#define SCENE_HEIGHT_PIXELS SCENE_HEIGHT_BLOCKS * BLOCK_WIDTH
#define SCENE_WIDTH_PIXELS SCENE_WIDTH_BLOCKS * BLOCK_WIDTH

// Number of blocks in the world
#define WORLD_HEIGHT_BLOCKS ((uint16_t) SCENE_HEIGHT_BLOCKS * 4) //120
#define WORLD_WIDTH_BLOCKS ((uint16_t) SCENE_WIDTH_BLOCKS * 5) //160

#define WORLD_WIDTH_CELLS WORLD_WIDTH_BLOCKS / 2
#define WORLD_HEIGHT_CELLS WORLD_HEIGHT_BLOCKS

#define WORLD_HEIGHT_PIXELS WORLD_HEIGHT_BLOCKS * BLOCK_WIDTH
#define WORLD_WIDTH_PIXELS WORLD_WIDTH_BLOCKS * BLOCK_WIDTH

#define N_LEVELS 9
#define SKY_GROUND_OFFSET 15
#define GROUND_SKY_RATIO WORLD_HEIGHT_BLOCKS / 3	// how much of the world is sky
#define LEVEL_SMOOTHING_FACTOR 12
#define KERNEL_WIDTH 32
#define TERRAIN_STD_THRESH 3

#define CAMERA_SPEED 5
#define SE_SIZE_DILATION 3
#define SE_SIZE_EROSION 5

#define LIGHT_MAP_HEIGHT WORLD_HEIGHT_BLOCKS
#define LIGHT_MAP_WIDTH (WORLD_WIDTH_BLOCKS / 8)
#define LIGHT_RADIUS 16		// radius of light in blocks
#define LIGHT_DEGRADATION_RATE 0.95

#define HMAP_SAMPLES_PER_CELL 16	// One hmap cell determines height 16 blocks
#define CAVE_SAMPLES_PER_CELL 2	// One cave sample determines nxn blocks of cave

#define tree_mask_width WORLD_WIDTH_BLOCKS/(TREE_WIDTH/BLOCK_WIDTH)

extern cell_t WORLD[WORLD_HEIGHT_BLOCKS][WORLD_WIDTH_BLOCKS/2];
extern cell_t SCENE[SCENE_HEIGHT_BLOCKS][SCENE_WIDTH_BLOCKS/2];
extern uint8_t SCENE_MASK[SCENE_HEIGHT_BLOCKS][SCENE_WIDTH_BLOCKS/2];	// Tells which blocks should not be overwritten in the loop
extern uint8_t** CAVE_MAP; 	//[WORLD_HEIGHT/CAVE_SAMPLES_PER_CELL][WORLD_WIDTH/(2*CAVE_SAMPLES_PER_CELL)];
extern int16_t HEIGHT_MAP[WORLD_WIDTH_BLOCKS/HMAP_SAMPLES_PER_CELL+1][WORLD_WIDTH_BLOCKS/HMAP_SAMPLES_PER_CELL+1];
extern block_c LVL1_HMAP[WORLD_WIDTH_BLOCKS];
extern uint8_t LIGHT_MAP[WORLD_HEIGHT_BLOCKS][WORLD_WIDTH_BLOCKS/8];	// 1 cell = 8 blocks, 1 bit for each
extern uint8_t TREE_MASK[tree_mask_width];	// 2 bits per cell, 0 = no tree, 1 = normal tree, 2 = tree_tall_green 3 = tree_tall_yellow
extern block_c camera_x_block;
extern block_c camera_y_block;

extern block_c old_camera_x;
extern block_c old_camera_y;

extern bool night;

void get_scene();

void get_scene_mask();

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

cell_t build_cell(block_t mat1, block_t mat2);

void world_zero_fill();

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

uint8_t get_block_with_pixels_from_WORLD(uint16_t x, uint16_t y);

bool isSolid (uint8_t block);

posx_pixel world_pixel_to_scene_pixel_x_no_band (posx_pixel pos_x);
posy_pixel world_pixel_to_scene_pixel_y_no_band (posy_pixel pos_y);

posx_pixel world_pixel_to_scene_pixel_x_band (posx_pixel pos_x);
posy_pixel world_pixel_to_scene_pixel_y_band (posy_pixel pos_y);

posx_pixel world_pixel_to_scene_pixel_x_no_band_param (posx_pixel pos_x, posx_pixel x_diff);
posy_pixel world_pixel_to_scene_pixel_y_no_band_param (posy_pixel pos_y, posy_pixel y_diff);

posx_pixel world_pixel_to_world_pixel_x_no_band_param (posx_pixel pos_x, posx_pixel x_diff);
posy_pixel world_pixel_to_world_pixel_y_no_band_param (posy_pixel pos_y, posy_pixel y_diff);

#endif
