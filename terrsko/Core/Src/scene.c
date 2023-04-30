#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>

#include "enums.h"
#include "materials.h"
#include "ugui.h"
#include "scene.h"
#include "material_classes.h"
#include "environment_models.h"
#include "utils.h"
// 512kB flash drive, 128kB RAM

#define CAVE_THRESH 12 //40
#define CAVE_ITER 10
#define CAVE_BIRTH_THRESH 3
#define CAVE_DEATH_THRESH 5

// 4 bits define the block at the position
cell_t WORLD[WORLD_HEIGHT_CELLS][WORLD_WIDTH_CELLS];		// 46KB
cell_t SCENE[SCENE_HEIGHT_BLOCKS][SCENE_WIDTH_BLOCKS/2];	// 2.3KB
uint8_t SCENE_MASK[SCENE_HEIGHT_BLOCKS][SCENE_WIDTH_BLOCKS/2];	// Tells which blocks should not be overwritten in the loop

uint8_t LIGHT_MAP[WORLD_HEIGHT_BLOCKS][WORLD_WIDTH_BLOCKS/8];	// 1 cell = 8 blocks, 1 bit for each

// Only holds bytes
uint8_t** CAVE_MAP;//[WORLD_HEIGHT/CAVE_SAMPLES_PER_CELL][WORLD_WIDTH/(2*CAVE_SAMPLES_PER_CELL)];

// Height can be > 256, need uint16_t
int16_t HEIGHT_MAP[WORLD_WIDTH_BLOCKS/HMAP_SAMPLES_PER_CELL+1][WORLD_WIDTH_BLOCKS/HMAP_SAMPLES_PER_CELL+1];	// Requires size 2^n + 1 in each dimension ie. [9][161]
block_c LVL1_HMAP[WORLD_WIDTH_BLOCKS];

uint8_t TREE_MASK[tree_mask_width];	// 2 bits per cell, 0 = no tree, 1 = normal tree, 2 = tree_tall_green 3 = tree_tall_yellow

block_c camera_x_block = 0;
block_c camera_y_block = 0;
uint8_t new_frame = 0;
block_c old_camera_x = 0;
block_c old_camera_y = 0;

float EUCLIDEAN_DISTANCES[LIGHT_RADIUS * 2];	// Inclusive precomputed euclidean distances for all possible distances
float LIGHT_INTENSITIES[LIGHT_RADIUS * 3];

// Initialize world, spawn in height/2, width/2, measured in blocks of 4x4, only call once per level, use enums to mark materials
void init_world() {

	world_zero_fill();
	generate_height_map(-3, 3, 5);
	generate_caves();

	uint8_t dirt = (_dirt << 4) | _dirt;		// low val
	uint8_t cave = (_dirt_bg << 4) | _dirt_bg;	// high val

	shape_caves_with_morphological_operations(dirt, cave);
	place_lava();

	precompute_euclidean();
	precompute_light_intensity();

	// Generate level with destroyables
	init_stage_0();

	init_light_map();
//	place_trees();
//	init_stage_1();

	uint16_t zero_height = LVL1_HMAP[WORLD_WIDTH_BLOCKS/2];
	update_camera_center((uint16_t) floor(WORLD_WIDTH_BLOCKS/3), zero_height - SKY_GROUND_OFFSET);	// zero level height should be at 1/3 of the screen

}

void world_zero_fill() {
	for (cell_c y = 0; y < WORLD_HEIGHT_CELLS; y++) {
		for (cell_c x = 0; x < WORLD_WIDTH_CELLS; x++) {
			WORLD[y][x] = build_cell(_empty, _empty);
		}
	}
}

// DO NOT UNDER ANY CIRCUMSTANCE TOUCH BELOW CODE!
void get_scene() {
	// width / 4 because each cell represents 2 blocks
	block_c block_offset_width = (SCENE_WIDTH_BLOCKS / 2);
	block_c block_offset_height = (SCENE_HEIGHT_BLOCKS / 2);

	cell_c offset_w = block_to_cell_x(block_offset_width);
	cell_c offset_h = block_to_cell_y(block_offset_height);

	// Check if within bounds
	cell_c left = MAX(0, (int16_t) block_to_cell_x(camera_x_block) - offset_w);
	cell_c top = MAX(0, (int16_t) block_to_cell_y(camera_y_block) - offset_h);
	cell_c right = MIN(block_to_cell_x(camera_x_block) + offset_w, block_to_cell_x(WORLD_WIDTH_BLOCKS));
	cell_c bottom = MIN(block_to_cell_y(camera_y_block) + offset_h, block_to_cell_y(WORLD_HEIGHT_BLOCKS));


	cell_c x = 0;
	cell_c y = 0;
	for (cell_c j = top; j <= bottom; j++) {
		for (cell_c i = left; i <= right; i++) {
			SCENE[y][x] = WORLD[j][i];
			x++;
		}
		x = 0;
		y++;
	}
}

void mark_tree(uint16_t y, uint16_t x, uint8_t left_right, uint16_t height, uint16_t width);

void get_scene_mask() {
	// width / 4 because each cell represents 2 blocks
	block_c block_offset_width = (SCENE_WIDTH_BLOCKS / 2);
	block_c block_offset_height = (SCENE_HEIGHT_BLOCKS / 2);

	cell_c offset_w = block_to_cell_x(block_offset_width);
	cell_c offset_h = block_to_cell_y(block_offset_height);


	//camera_x = 40; camera_y = 40;

	// Check if within bounds
	cell_c left = MAX(0, (int16_t) block_to_cell_x(camera_x_block) - offset_w);
	cell_c top = MAX(0, (int16_t) block_to_cell_y(camera_y_block) - offset_h);
	cell_c right = MIN(block_to_cell_x(camera_x_block) + offset_w, block_to_cell_x(WORLD_WIDTH_BLOCKS));
	cell_c bottom = MIN(block_to_cell_y(camera_y_block) + offset_h, block_to_cell_y(WORLD_HEIGHT_BLOCKS));

	new_frame = 0;

	uint16_t x = 0;
	uint16_t y = 0;

	// Init to zero
	for (uint16_t i = top; i < bottom; i++) {
		for (uint16_t j = left; j < right; j++) {
			SCENE_MASK[y][x] = 0;
			x++;
		}
		y++;
	}

	x = 0; y = 0;

	for (uint16_t i = top; i < bottom; i++) {
		for (uint16_t j = left; j < right; j++) {
			// Dont overwrite the tree marks
			if (SCENE_MASK[y][x] > 0 && new_frame == 0) continue;
			uint8_t val = WORLD[i][j];
			uint8_t l_val = (val & 0xF0) >> 4;
			uint8_t r_val = val & 0x0F;

			uint8_t mask_val_l = 0, mask_val_r = 0;

			// Check if there is an overlay
			if (l_val == _tree) {
				mask_val_l = 1;
				mark_tree(y, x, 0, TREE_HEIGHT/BLOCK_WIDTH, TREE_WIDTH/BLOCK_WIDTH);
			}
			if (r_val == _tree) {
				mask_val_r = 1;
				mark_tree(y, x, 1, TREE_HEIGHT/BLOCK_WIDTH, TREE_WIDTH/BLOCK_WIDTH);
			}



			SCENE_MASK[y][x] = (mask_val_l << 4) | mask_val_r;
			x++;
		}
		x = 0;
		y++;
	}

	new_frame = 1;
}

void mark_tree(uint16_t y, uint16_t x, uint8_t left_right, uint16_t height, uint16_t width) {
//	x += left_right;
	for (uint16_t j = x; j < x + width; j++) {
		for (uint16_t i = y; i < y + height; i++) {
			SCENE_MASK[i][j] = 1 << 4 | 1;
		}
	}
}

// x,y in blocks
void update_camera_center(uint16_t x, uint16_t y) {

	// Check if we are at least half a screen from a border
	if (x >= WORLD_WIDTH_BLOCKS - (SCENE_WIDTH_BLOCKS / 2 + 1)) {
		x = WORLD_WIDTH_BLOCKS - SCENE_WIDTH_BLOCKS / 2 + 1;
	} else if (x < SCENE_WIDTH_BLOCKS / 2) {
		x = SCENE_WIDTH_BLOCKS / 2 + 1;
	}

	if (y >= WORLD_HEIGHT_BLOCKS - (SCENE_HEIGHT_BLOCKS / 2 + 1)) {
		y = WORLD_HEIGHT_BLOCKS - (SCENE_HEIGHT_BLOCKS / 2 + 1);
	} else if (y < (SCENE_HEIGHT_BLOCKS / 2 + 1)) {
		y = (SCENE_HEIGHT_BLOCKS / 2 + 1);
	}

	camera_x_block = x;
	camera_y_block = y;
}


void init_stage_1() {
	// Read in the tree matrix



}


void place_trees() {
	// TODO - mark every covered pixel as taken

	srand(time(NULL));
	float tree_density = 0.2;
	for (int i = 0; i < tree_mask_width; i++) {
		TREE_MASK[i] = 0;
	}

	for (block_c i = 0; i < WORLD_WIDTH_BLOCKS; i++) {
		block_c coord = i / (TREE_WIDTH / BLOCK_WIDTH);
//		if (TREE_MASK[coord] == 1) {
//			WORLD[y][i/2] = _empty;
//		}

		block_c y = LVL1_HMAP[i]; //- TREE_HEIGHT / BLOCK_WIDTH;


		if (rand() % 100 < tree_density) {

			// Trees only on odd numbered blocks (bottom 4 bits)
			WORLD[y][i/2] = (WORLD[y][i/2] & 0xF0) | _tree;
//			for (int j = 1; j < TREE_WIDTH; j++) {
//				for (int i = 0; i < TREE_HEIGHT; i++) {
//					if (tree_r[i][j] > 0) {
//						WORLD[y+i][i/2+j] = _empty;
//					}
//				}
//			}
			TREE_MASK[coord] = 1;
		}
	}
}


void init_light_map() {

	// Coefficient between world and light width, !HEIGHT IS THE SAME!
	uint8_t light_to_world_coefficient = WORLD_WIDTH_CELLS / LIGHT_MAP_WIDTH;

	for (uint16_t i = 0; i < LIGHT_MAP_WIDTH; i++) {
		for (uint16_t j = 0; j < LIGHT_MAP_HEIGHT; j++) {

			uint8_t lighting_byte = 0;

			uint16_t x_coor = i * light_to_world_coefficient;
			uint16_t y_coor = j;

			// Check 4 world cells, equivalent to 8 blocks
			for (uint16_t k = 0; k < light_to_world_coefficient; k++) {
				uint16_t world_x = x_coor + k;
				uint16_t world_y = y_coor;

				uint8_t cell = WORLD[world_y][world_x];

				uint8_t l_cell = (cell & 0xF0) >> 4;
				uint8_t r_cell = cell & 0x0F;

				if (is_light_source(l_cell)) {
					lighting_byte |= 1 << (8 - (2 * k));
				}

				if (is_light_source(r_cell)) {
					lighting_byte |= 1 << (8 - (2 * k) - 1);
				}
			}
		LIGHT_MAP[j][i] = lighting_byte;
		}
	}
}

bool is_night() {
	srand(time(NULL));
	float random = (float)rand() / RAND_MAX;

	if (random < 0.5) {
		return true;
	}
	return false;
}

// x,y are scene coordinates
float compute_illumination(uint16_t x, uint16_t y) {
	// TODO check if the neighbors are lit as well and interpolate brightness
	uint16_t global_x, global_y;

	global_x = block_to_cell_x(camera_x_block) - (SCENE_WIDTH_BLOCKS / 4) + x;
	global_y = camera_y_block - (SCENE_HEIGHT_BLOCKS / 2) + y;

	// Light source is lit
	if (is_light_source(WORLD[global_y][global_x])) return 1.0;
	else if (is_light_source((WORLD[global_y][global_x] & 0xF0) >> 4) || is_light_source(WORLD[global_y][global_x] & 0x0F))
		return 1.0;

//	Should look like this
//	uint8_t dist_to_camera = floor(sqrt(pow((camera_x - global_x) * 2, 2) + pow((camera_y - global_y), 2)));
//	if (dist_to_camera < LIGHT_RADIUS) {
//		return light_intensity(dist_to_camera);
//	}

	// For each block, calculate the distance to light source
	uint8_t search_radius = LIGHT_RADIUS;	// In blocks
	float max_illumination = 0;
	uint8_t current_bit_position = global_x % 4;
	uint8_t iter = 0;
	for (int8_t j = -search_radius; j <= search_radius; j++) {		// Each column
		for (int8_t i = -ceil(search_radius/8); i <= ceil(search_radius/8); i++) {	// Each row
			uint16_t x_coor = global_x / 4 + i;
			uint16_t y_coor = global_y + j;
			float illumination = 0;
			iter++;

			// Decode horizontal cells
			uint8_t light_cell_value = LIGHT_MAP[y_coor][x_coor];

			uint8_t c8 = (light_cell_value & 0b10000000) >> 7;	// 8 = most important bit (left)
			uint8_t c7 = (light_cell_value & 0b01000000) >> 6;
			uint8_t c6 = (light_cell_value & 0b00100000) >> 5;
			uint8_t c5 = (light_cell_value & 0b00010000) >> 4;

			uint8_t c4 = (light_cell_value & 0b00001000) >> 3;
			uint8_t c3 = (light_cell_value & 0b00000100) >> 2;
			uint8_t c2 = (light_cell_value & 0b00000010) >> 1;
			uint8_t c1 = (light_cell_value & 0b00000001);

			int8_t horizontal_dist = 0;
			uint8_t dist = 0;
			uint8_t max_manhattan = 2 * search_radius;
			// Try getting average/min/max illumination
			// TODO currently returns 1 for every block less than 8 blocks away from light source horizontally
			// 		The offset distance is of (the hardcoded +/-)


			if (i > 0) {
				horizontal_dist = (i - 1) * 8;					// full blocks
				horizontal_dist += (8 - current_bit_position);	// original inner offset
				dist = manhattan_dist(horizontal_dist, abs(j));
				if (c8 && dist < max_manhattan) {
					illumination = get_light_intensity(manhattan_dist(j, horizontal_dist));
				} else if (c7 && dist < max_manhattan - 1) {
					illumination = get_light_intensity(manhattan_dist(j, horizontal_dist+1));
				} else if (c6 && dist < max_manhattan - 2) {
					illumination = get_light_intensity(manhattan_dist(j, horizontal_dist+2));
				} else if (c5 && dist < max_manhattan - 3) {
					illumination = get_light_intensity(manhattan_dist(j, horizontal_dist+3));
				} else if (c4 && dist < max_manhattan - 4) {
					illumination = get_light_intensity(manhattan_dist(j, horizontal_dist+4));
				} else if (c3 && dist < max_manhattan - 5) {
					illumination = get_light_intensity(manhattan_dist(j, horizontal_dist+5));
				} else if (c2 && dist < max_manhattan - 6) {
					illumination = get_light_intensity(manhattan_dist(j, horizontal_dist+6));
				} else if (c1 && dist < max_manhattan - 7) {
					illumination = get_light_intensity(manhattan_dist(j, horizontal_dist+7));
				}
			} else if (i < 0) {
				horizontal_dist = (abs(i) - 1) * 8;
				horizontal_dist += current_bit_position;
				dist = manhattan_dist(horizontal_dist, abs(j));

				if (c1 && dist < max_manhattan) {
					illumination = get_light_intensity(manhattan_dist(j, horizontal_dist));
				} else if (c2 && dist < max_manhattan - 1) {
					illumination = get_light_intensity(manhattan_dist(j, horizontal_dist+1));
				} else if (c3 && dist < max_manhattan - 2) {
					illumination = get_light_intensity(manhattan_dist(j, horizontal_dist+2));
				} else if (c4 && dist < max_manhattan - 3) {
					illumination = get_light_intensity(manhattan_dist(j, horizontal_dist+3));
				} else if (c5 && dist < max_manhattan - 4) {
					illumination = get_light_intensity(manhattan_dist(j, horizontal_dist+4));
				} else if (c6 && dist < max_manhattan - 5) {
					illumination = get_light_intensity(manhattan_dist(j, horizontal_dist+5));
				} else if (c7 && dist < max_manhattan - 6) {
					illumination = get_light_intensity(manhattan_dist(j, horizontal_dist+6));
				} else if (c8 && dist < max_manhattan - 7) {
					illumination = get_light_intensity(manhattan_dist(j, horizontal_dist+7));
				}
			} else {
				if (c1) {
//					horizontal_dist += abs(0 - current_bit_position);
					illumination = get_light_intensity(manhattan_dist(j, horizontal_dist));
				} else if (c2) {
//					horizontal_dist += abs(1 - current_bit_position);
					illumination = get_light_intensity(manhattan_dist(j, horizontal_dist));
				} else if (c3) {
//					horizontal_dist += abs(2 - current_bit_position);
					illumination = get_light_intensity(manhattan_dist(j, horizontal_dist));
				} else if (c4) {
//					horizontal_dist += abs(3 - current_bit_position);
					illumination = get_light_intensity(manhattan_dist(j, horizontal_dist));
				} else if (c5) {
//					horizontal_dist += abs(4 - current_bit_position);
					illumination = get_light_intensity(manhattan_dist(j, horizontal_dist));
				} else if (c6) {
//					horizontal_dist += abs(5 - current_bit_position);
					illumination = get_light_intensity(manhattan_dist(j, horizontal_dist));
				} else if (c7) {
//					horizontal_dist += abs(6 - current_bit_position);
					illumination = get_light_intensity(manhattan_dist(j, horizontal_dist));
				} else if (c8) {
//					horizontal_dist += abs(7 - current_bit_position);
					illumination = get_light_intensity(manhattan_dist(j, horizontal_dist));
				}
			}

			if (illumination > max_illumination) {
				max_illumination = illumination;
			}
		}
	}

	return max_illumination;
}

float manhattan_dist(int8_t x, int8_t y) {
	return abs(x) + abs(y);
}

float euclidean_dist(int8_t x, int8_t y) {
	return sqrt(pow(x,2) + pow(y,2));
}

void precompute_euclidean() {
	for (uint8_t i = 0; i <= LIGHT_RADIUS; i++) {
		for (uint8_t j = i; j <= LIGHT_RADIUS; j++) {
			float dist = euclidean_dist(i, j);
			EUCLIDEAN_DISTANCES[i+j] = dist;
		}
	}
}

float get_euclidean(uint8_t manhattan_dist) {
	return EUCLIDEAN_DISTANCES[manhattan_dist];
}

void precompute_light_intensity() {
	uint8_t n_values = LIGHT_RADIUS * 2;
	for (uint8_t i = 0; i < n_values; i++) {
		float euclidean = get_euclidean(i);
		euclidean = MAX(1, euclidean);
		if (euclidean > 0.01) {
			LIGHT_INTENSITIES[i] = light_intensity(euclidean);
		}
	}
}

float get_light_intensity(uint8_t manhattan_dist) {
	float value = LIGHT_INTENSITIES[manhattan_dist];
	return value;
}

float light_intensity(float dist) {
	if (dist < 4) {
		return MIN(1, pow(LIGHT_DEGRADATION_RATE, dist));
	} else if (dist < 6) {
		return MIN(1, pow(LIGHT_DEGRADATION_RATE-.1, dist));
	} else {
		return MIN(1, pow(LIGHT_DEGRADATION_RATE-.2, dist));
	}
}


block_t assign_block_material(block_c x, block_c y) {
	float probability_rock = 0.01;
	block_t block;

	// right block
	if (y > LVL1_HMAP[x]) { // Ground

		float random = (float) rand() / (float) (RAND_MAX);

		// Add random rocks
		if (random < probability_rock && abs(LVL1_HMAP[x]) < y) {
			block = _rock;
		} else {
			block = _dirt;
		}

	} else if (y == LVL1_HMAP[x]) {
		block = _grass;
	} else {
		block = _sky;
	}

	return block;
}


// Get basic landscape - dirt, sky + caves, lava
void init_stage_0() {

	srand(time(NULL));

	// Values identifying cave and lava materials
	block_t cave = ((_dirt_bg << 4) | _dirt_bg);
	block_t lava = ((_lava << 4) | _lava);

	for (block_c i = 0; i < WORLD_HEIGHT_BLOCKS; i++) {
		for (block_c j = 0; j < WORLD_WIDTH_BLOCKS; j+=2) {
			block_t l_block = _dirt; block_t r_block = _dirt;

			// Check for predetermined special values and don't overwrite, because they are already put in
			if ((WORLD[i][j/2] == cave || WORLD[i][j/2] == lava) && i > LVL1_HMAP[j]) {
				continue;
			}

			// Assign materials
			l_block = assign_block_material(j, i);
			r_block = assign_block_material(j+1, i);

			float random = (float) rand() / (float) (RAND_MAX);
			// 10% of rocks should be bigger blobs
			if (random < 0.1) {
				if (l_block == _rock || r_block == _rock) {
					int random_radius = random_int(1, 4);
					draw_blob(j/2, i, random_radius, build_cell(_rock, _rock));
				}
			}

			// Store into WORLD
			WORLD[i][j/2] = build_cell(l_block, r_block);

		}
	}
}

// first parameter is left block, second right
cell_t build_cell(block_t mat1, block_t mat2) {
	return (mat1 << 4) | mat2;
}

void place_lava() {
	// 1% chance of random spawn + last 2 row
	srand(time(NULL));
	float chance_of_lava = 0.1;
	uint8_t lava_blob_radius = rand() % 3;
	block_t lava_block = (_lava << 4) | _lava;
	block_t dirt_block = (_dirt << 4) | _dirt;

	for (cell_c i = 0; i < WORLD_WIDTH_CELLS; i++) {
		cell_c starting_depth = LVL1_HMAP[2*i] + 10;
		for (uint16_t j = starting_depth; j < WORLD_HEIGHT_CELLS; j++) {
			if ((float) rand() / (float) (RAND_MAX / 100) < chance_of_lava) { // && WORLD[j][i] & 0xF0 != _dirt_bg) {
				draw_blob(i, j, lava_blob_radius, lava_block);
//				WORLD[j][i] = lava_block;
			}

			if (j >= WORLD_HEIGHT_CELLS - 2) {
				WORLD[j][i] = lava_block;
			}
		}
	}

	shape_caves_with_morphological_operations(dirt_block, lava_block);

}

void generate_caves() {

	CAVE_MAP = (uint8_t**) malloc((WORLD_HEIGHT_BLOCKS/CAVE_SAMPLES_PER_CELL) * sizeof(uint8_t*));

	for (uint16_t j = 0; j < WORLD_HEIGHT_BLOCKS/CAVE_SAMPLES_PER_CELL; j++) {
		CAVE_MAP[j] = (uint8_t*) malloc((WORLD_WIDTH_BLOCKS/(2*CAVE_SAMPLES_PER_CELL)) * sizeof(uint8_t));
	}


	block_t cave_value = (_dirt_bg << 4) | _dirt_bg;
	block_t dirt_value = (_dirt << 4) | _dirt;

	uint16_t map_width = WORLD_WIDTH_BLOCKS/(2*CAVE_SAMPLES_PER_CELL);
	uint16_t map_height = WORLD_HEIGHT_BLOCKS/CAVE_SAMPLES_PER_CELL;

	uint8_t radius = 3;
	uint8_t blob_probability = 0.1;


	// Randomly set some cells to zero
	for (uint16_t x = 0; x < map_width; x++) {
		for (uint16_t y = 0; y < map_height; y++) {	// Caves start 5 blocks under ground
			CAVE_MAP[y][x] = dirt_value;
			if (rand() % 100 < CAVE_THRESH) {
				CAVE_MAP[y][x] = cave_value;
			}
		}
	}

	// Cellular automata rules
	for (uint8_t iter = 0; iter < CAVE_ITER; iter++) {
		for (uint16_t j = 0; j < map_width; j++) {
			for (uint16_t i = 0; i < map_height; i++) {

				uint8_t neighbor_cave_count = 0;


				if (i > 0 && j > 0 && CAVE_MAP[i-1][j-1] == cave_value) {
					neighbor_cave_count++;
				}
				if (i > 0 && CAVE_MAP[i-1][j] == cave_value) {
					neighbor_cave_count++;
				}
				if (i > 0 && j < map_height-1 && CAVE_MAP[i-1][j+1] == cave_value) {
					neighbor_cave_count++;
				}
				if (j > 0 && CAVE_MAP[i][j-1] == cave_value) {
					neighbor_cave_count++;
				}
				if (j < map_height-1 && CAVE_MAP[i][j+1] == cave_value) {
					neighbor_cave_count++;
				}
				if (i < map_width-1 && j > 0 && CAVE_MAP[i+1][j-1] == cave_value) {
					neighbor_cave_count++;
				}
				if (i < map_width-1 && CAVE_MAP[i+1][j] == cave_value) {
					neighbor_cave_count++;
				}
				if (i < map_width-1 && j < map_height-1 && CAVE_MAP[i+1][j+1] == cave_value) {
					neighbor_cave_count++;
				}

				// Rules for cave spreading
				if (CAVE_MAP[i][j] == dirt_value) {
					if (neighbor_cave_count > CAVE_BIRTH_THRESH) {
						CAVE_MAP[i][j] = cave_value;
						if (rand() % 100 < blob_probability) {
							draw_circle(i, j, radius, cave_value);
						}
					}
				} else {
					if (neighbor_cave_count < CAVE_DEATH_THRESH || (neighbor_cave_count > 6 && rand() % 100 < 40)) {
						CAVE_MAP[i][j] = dirt_value;
						if (rand() % 100 < blob_probability) {
							draw_circle(i, j, radius, dirt_value);
						}
					}
					CAVE_MAP[i][j] = cave_value;
					if (rand() % 100 < blob_probability) {
						draw_circle(i, j, radius, cave_value);
					}
				}
			}
		}
	}

	for (uint16_t x = 0; x < map_width; x += 1) {
		for (uint16_t y = 0; y < map_height; y += 1) {

			uint8_t value = CAVE_MAP[map_height - y][x];

			for (uint8_t cx = 0; cx < CAVE_SAMPLES_PER_CELL; cx++) {
				for (int8_t cy = 0; cy < CAVE_SAMPLES_PER_CELL; cy++) {

					uint8_t depth = rand() % 6 + 7;
					uint16_t y_coor = y * CAVE_SAMPLES_PER_CELL + cy;
					uint16_t x_coor = x * CAVE_SAMPLES_PER_CELL + cx;

					// Add smoothing
					if (y_coor > LVL1_HMAP[x_coor] + depth) {
						WORLD[y_coor][x_coor] = value;
					}
				}
			}

		}
	}

	for (uint16_t j = 0; j < WORLD_HEIGHT_BLOCKS/CAVE_SAMPLES_PER_CELL; j++) {
		free(CAVE_MAP[j]);
	}
	free(CAVE_MAP);

}

// Draws blob, make sure value includes value for both blocks
void draw_blob(cell_c x, cell_c y, uint16_t radius, cell_t value) {
	for (int8_t i = -radius; i <= radius; i++) {
		for (int8_t j = -radius; j <= radius; j++) {
			if (y+i > LVL1_HMAP[x+j] + 5) {
				WORLD[y+i][x+j] = value;
			}
		}
	}
}

void draw_circle(uint16_t x, uint16_t y, uint8_t radius, uint8_t cave_value) {
	uint16_t width = WORLD_WIDTH_BLOCKS / (2 * CAVE_SAMPLES_PER_CELL);
	uint16_t height = WORLD_HEIGHT_BLOCKS / CAVE_SAMPLES_PER_CELL;

	for (int8_t xx = -radius; xx < radius; xx++) {
		for (int8_t yy = -radius; yy < radius; yy++) {

			int16_t pos_x = (int16_t) x + xx;
			int16_t pos_y = (int16_t) y + yy;

			if (pos_x > 0 && pos_x < width && pos_y > 0 && pos_y < height) {
				CAVE_MAP[pos_y][pos_x] = cave_value;
			}

		}
	}
}

// Generate 2D height map, a row can represent a level, can also be used for different tones of background underground (Worms style), Diamond-Square alg (https://en.wikipedia.org/wiki/Diamond-square_algorithm)
void generate_height_map(uint8_t random_lower, uint8_t random_upper, float roughness) {

	srand(time(NULL));

	uint16_t map_size = WORLD_WIDTH_BLOCKS + 1;

	// Initialize heights - height 0 equals WORLD_HEIGHT / 2
	HEIGHT_MAP[0][0] = 0;			// Elevated on edge
	HEIGHT_MAP[0][map_size-1] = 0;
	HEIGHT_MAP[map_size-1][0] = 0;	// Elevated on edge
	HEIGHT_MAP[map_size-1][map_size-1] = 0;

	uint8_t step = map_size - 1;


	// Repeat until convergence occurs (chunk size = 1)
	while (step > 1) {

		// Diamond step
		for (uint16_t y = step / 2; y < map_size - 1; y += step) {
			for (uint16_t x = step / 2; x < map_size- 1; x += step) {

				uint16_t half_step = step / 2;

				uint8_t sum = 	HEIGHT_MAP[y-half_step][x-half_step] +
								HEIGHT_MAP[y-half_step][x+half_step] +
								HEIGHT_MAP[y+half_step][x-half_step] +
								HEIGHT_MAP[y+half_step][x+half_step];

				uint8_t random_n = random_lower + (uint8_t) ( (int) rand() % (random_upper - random_lower + 1));

				if (step == map_size - 1) {
					random_n /= 2;
				}

				uint8_t average = (uint8_t) round(sum / 4);

				// Sum average and random number, make sure it is in range
				HEIGHT_MAP[y][x] = average + random_n * roughness;

			}
		}

		// Square step						9/2 = 4 -> gets middle element
		for (uint16_t y = 0; y < map_size; y += step/2) {


			uint16_t x0 = (y + step/2) % step;		// CHECK IF OK
			for (uint16_t x = x0; x < map_size; x += step) {
				uint8_t sum = 0;
				uint8_t count = 0;

				uint8_t half_step = step / 2;

				if (x >= half_step ) {
					sum += HEIGHT_MAP[y][x-half_step];
					count++;
				}
				if (x < map_size - half_step ) {
					sum += HEIGHT_MAP[y][x+half_step];
					count++;
				}
				if (y >= half_step ) {
					sum += HEIGHT_MAP[y-half_step][x];
					count++;
				}
				if (y < map_size - half_step ) {
					sum += HEIGHT_MAP[y+half_step][x];
					count++;
				}

				uint8_t average = sum / count;
				uint8_t random_n = random_lower + (uint8_t) ((int) rand() % (random_upper - random_lower + 1)) * roughness;

				HEIGHT_MAP[y][x] = average + random_n;
			}
		}

		step /= 2;

		// Halve the randomness every iteration
		if (abs(random_lower) > 1 && abs(random_upper) > 1) {
			random_lower /= 2;
			random_upper /= 2;
		}
	}

	// EL PARTE MAS IMPORTANTE - fill in the values
	for (uint16_t i = 0; i < WORLD_WIDTH_BLOCKS; i+=HMAP_SAMPLES_PER_CELL) {
		uint8_t val = HEIGHT_MAP[WORLD_WIDTH_BLOCKS/HMAP_SAMPLES_PER_CELL][i/HMAP_SAMPLES_PER_CELL] / HMAP_SAMPLES_PER_CELL + GROUND_SKY_RATIO;
		for (uint8_t j = 0; j < HMAP_SAMPLES_PER_CELL; j++) {
			LVL1_HMAP[i+j] = val;
		}
	}

	LVL1_HMAP[0] += 5;
	LVL1_HMAP[WORLD_WIDTH_BLOCKS] += 5;

	// LVL1_HMAP is as wide as the world, smooth the bumps
	filter_level(WORLD_WIDTH_BLOCKS, KERNEL_WIDTH, LEVEL_SMOOTHING_FACTOR, false);
//	filter_level(WORLD_WIDTH_BLOCKS, KERNEL_WIDTH, LEVEL_SMOOTHING_FACTOR, true);
//	filter_level(WORLD_WIDTH_BLOCKS, KERNEL_WIDTH, LEVEL_SMOOTHING_FACTOR, true);

}

// Returns gauss kernel of width width and given sigma
float* gauss_kernel(uint8_t width, uint8_t sigma) {
	float* filter = (float*) malloc(width * sizeof(float));

    float sum = 0.0;

    // Compute the filter values
    for (int i = 0; i < width; i++) {
        int x = i - (width - 1) / 2;
        filter[i] = exp(-x * x / (2 * sigma * sigma)) / (sqrt(2 * M_PI) * sigma);
        sum += filter[i];
    }

    // Normalize the filter values
    for (int i = 0; i < width; i++) {
        filter[i] /= sum;
    }

    return filter;
}

// Perform morphological opening on the binary mask (cave map) - connect the caves
void shape_caves_with_morphological_operations(uint8_t dirt, uint8_t foreground) {

	uint8_t EROSION_SE[SE_SIZE_EROSION][SE_SIZE_EROSION];

	for (uint8_t i = 0; i < SE_SIZE_EROSION; i++) {
		for (uint8_t j = 0; j < SE_SIZE_EROSION; j++) {
			if ((i < SE_SIZE_EROSION/3 || i > 2*SE_SIZE_EROSION/3) && (j < SE_SIZE_EROSION/3 || j > 2*SE_SIZE_EROSION/3)) {
				EROSION_SE[i][j] = dirt;
			} else {
				EROSION_SE[i][j] = foreground;
			}
		}
	}

	uint8_t DILATION_SE[SE_SIZE_DILATION][SE_SIZE_DILATION];

	for (uint8_t i = 0; i < SE_SIZE_DILATION; i++) {
		for (uint8_t j = 0; j < SE_SIZE_DILATION; j++) {
			if ((i < SE_SIZE_DILATION/3 || i > 2*SE_SIZE_DILATION/3) && (j < SE_SIZE_DILATION/3 || j > 2*SE_SIZE_DILATION/3)) {
				DILATION_SE[i][j] = dirt;
			} else {
				DILATION_SE[i][j] = foreground;
			}
		}
	}

	uint16_t width = WORLD_WIDTH_BLOCKS/2;
	uint16_t height = WORLD_HEIGHT_BLOCKS;
	uint8_t cave = (_dirt_bg << 4) | _dirt_bg;

	if (foreground == cave) {
		dilation(DILATION_SE, width, height, dirt, foreground);
		erosion(EROSION_SE, width, height, foreground);
		dilation(DILATION_SE, width, height, dirt, foreground);
		erosion(EROSION_SE, width, height, foreground);
		dilation(DILATION_SE, width, height, dirt, foreground);
		erosion(EROSION_SE, width, height, foreground);
		dilation(DILATION_SE, width, height, dirt, foreground);
		dilation(DILATION_SE, width, height, dirt, foreground);
	} else {
		// Dilate lava
		dilation(DILATION_SE, width, height, dirt, foreground);
		dilation(DILATION_SE, width, height, dirt, foreground);
		dilation(DILATION_SE, width, height, dirt, foreground);
		dilation(DILATION_SE, width, height, dirt, foreground);
	}


}

void erosion(uint8_t SE[SE_SIZE_EROSION][SE_SIZE_EROSION], uint16_t map_width, uint16_t map_height, uint8_t foreground) {
	uint8_t se_size = SE_SIZE_EROSION;

	uint8_t temp[map_height][map_width];

	for (uint16_t i = 0; i < map_width; i++) {
		for (uint16_t j = LVL1_HMAP[i]; j < map_height; j++) {
			uint8_t min_value = foreground;
			for (uint16_t k = 0; k < se_size; k++) {
				for (uint16_t l = 0; l < se_size; l++) {
					if (SE[k][l] == foreground) {
						uint16_t x = i - k + se_size / 2;
						uint16_t y = j - l + se_size / 2;
						if (x >= 0 && x < map_width && y >= 0 && y < map_height) {
							min_value = min_value < WORLD[y][x] ? min_value : WORLD[y][x];
						}
					}
				}
			}

			temp[j][i] = min_value;
		}
	}

	for (uint16_t i = 0; i < map_width; i++) {
		for (uint16_t j = LVL1_HMAP[i]; j < map_height; j++) {
			WORLD[j][i] = temp[j][i];
		}
	}
}

void dilation(uint8_t SE[SE_SIZE_DILATION][SE_SIZE_DILATION], uint16_t map_width, uint16_t map_height, uint8_t dirt, uint8_t foreground) {

	uint8_t se_size = SE_SIZE_DILATION;

	uint8_t temp[map_height][map_width];

	uint8_t depth = rand() % 5 + 5;

	for (uint16_t i = 0; i < map_width; i++) {
		for (uint16_t j = LVL1_HMAP[i] + depth; j < map_height; j++) {
			uint8_t max_value = dirt;
			for (uint16_t k = 0; k < se_size; k++) {
				for (uint16_t l = 0; l < se_size; l++) {
					if (SE[k][l] == foreground) {
						uint16_t x = i - k + se_size / 2;
						uint16_t y = j - l + se_size / 2;
						if (x >= 0 && x < map_width && y > GROUND_SKY_RATIO + depth && y < map_height) {
							max_value = max_value > WORLD[y][x] ? max_value : WORLD[y][x];
						}
					}
				}
			}

			temp[j][i] = max_value;
		}
	}

	for (uint16_t i = 0; i < map_width; i++) {
		for (uint16_t j = LVL1_HMAP[i]; j < map_height; j++) {
			WORLD[j][i] = temp[j][i];
		}
	}
}

void filter_level(uint16_t array_size, uint8_t kernel_width, uint8_t sigma, bool only_sharp_edges) {
	int8_t* result = malloc(array_size);

	float* filter = gauss_kernel(kernel_width, sigma);


	for (uint16_t i = 0; i < array_size; i++) {
		float sum = 0.0;

		float std = 0.0;
		float mean = 0.0;

		for (int j = 0; j < kernel_width; j++) {
			int k = i + j - (kernel_width - 1) / 2;
			if (k >= 0 && k < array_size) {
				sum += LVL1_HMAP[k] * filter[j];
				mean += LVL1_HMAP[k];
			}
		}

		// Calculate standard deviation
		if (only_sharp_edges) {
			mean = mean / (kernel_width-1);
			for (int j = 0; j < kernel_width; j++) {
				int k = i + j - (kernel_width - 1) / 2;
				if (k >= 0 && k < array_size) {
					std += pow(LVL1_HMAP[k] - mean, 2);
				}
			}
			std /= (kernel_width - 1);
			std = sqrt(std);
		}

		if ((only_sharp_edges && std > TERRAIN_STD_THRESH) || !only_sharp_edges) {
			result[i] = (int8_t) round(sum);
		} else {
			result[i] = LVL1_HMAP[i];
		}
	}

	// Write back
	for (uint16_t i = 0; i < array_size; i++) {
		LVL1_HMAP[i] = result[i];
	}

	free(filter);
	free(result);
}

uint8_t random_int(uint8_t min, uint8_t max) {
    return (uint8_t) rand() % (max - min + 1) + min;
}

// x and y are postion of pixels on world
// function used for movables to get what is around them
block_t get_block_with_pixels_from_WORLD(pixel_c x, pixel_c y) {
	// Same as x % 2 but for pixel coordinates
	if (x % 8 >= 4) {
		return lower(WORLD[pixel_to_cell_y(y)][pixel_to_cell_x(x)]);
	}

	return upper(WORLD[pixel_to_cell_y(y)][pixel_to_cell_x(x)]);
}


/*
 * returns the pixel that is represented in WORLD position to
 * position that is represented in SCENE position
 * for x axis and down under y axis
 *
 */
posx_pixel world_pixel_to_scene_pixel_x_no_band (posx_pixel pos_x) {
	posx_pixel mapped_pos = pos_x - ((camera_x_block * BLOCK_WIDTH) - (SCENE_WIDTH_PIXELS / 2));

	if (mapped_pos < 0) mapped_pos = 0;
	if (mapped_pos >= SCENE_WIDTH_PIXELS) mapped_pos = SCENE_WIDTH_PIXELS - 1;

	return mapped_pos;
}
posy_pixel world_pixel_to_scene_pixel_y_no_band (posy_pixel pos_y) {
	posx_pixel mapped_pos = pos_y - ((camera_y_block * BLOCK_WIDTH) - (SCENE_HEIGHT_PIXELS / 2));

	if (mapped_pos < 0) mapped_pos = 0;
	if (mapped_pos >= SCENE_HEIGHT_PIXELS) mapped_pos = SCENE_HEIGHT_PIXELS - 1;
	return mapped_pos;
}

posx_pixel world_pixel_to_scene_pixel_x_band (posx_pixel pos_x) {
	posx_pixel mapped_pos = pos_x - ((camera_x_block * BLOCK_WIDTH) - (SCENE_WIDTH_BLOCKS * BLOCK_WIDTH / 2));

	return mapped_pos;
}
posy_pixel world_pixel_to_scene_pixel_y_band (posy_pixel pos_y) {
	posx_pixel mapped_pos = pos_y - ((camera_y_block * BLOCK_WIDTH) - (SCENE_HEIGHT_BLOCKS * BLOCK_WIDTH / 2));

	return mapped_pos;
}

posx_pixel world_pixel_to_scene_pixel_x_no_band_param (posx_pixel pos_x, posx_pixel x_diff) {
	posx_pixel mapped_pos = pos_x - x_diff - ((camera_y_block * BLOCK_WIDTH) - (SCENE_WIDTH_BLOCKS * BLOCK_WIDTH / 2));

	if (mapped_pos < 0) mapped_pos = 0;
	if (mapped_pos >= SCENE_WIDTH_BLOCKS * BLOCK_WIDTH) mapped_pos = SCENE_WIDTH_BLOCKS * BLOCK_WIDTH - 1;
	return mapped_pos;
}

posy_pixel world_pixel_to_scene_pixel_y_no_band_param (posy_pixel pos_y, posy_pixel y_diff) {
	posx_pixel mapped_pos = pos_y - y_diff - ((camera_y_block * BLOCK_WIDTH) - (SCENE_HEIGHT_BLOCKS * BLOCK_WIDTH / 2));

	if (mapped_pos < 0) mapped_pos = 0;
	if (mapped_pos >= SCENE_HEIGHT_BLOCKS * BLOCK_WIDTH) mapped_pos = SCENE_HEIGHT_BLOCKS * BLOCK_WIDTH - 1;
	return mapped_pos;
}

posx_pixel world_pixel_to_world_pixel_x_no_band_param (posx_pixel pos_x, posx_pixel x_diff) {
	posx_pixel mapped_pos = pos_x + x_diff;

	if (mapped_pos < 0) mapped_pos = 0;
	if (mapped_pos >= WORLD_WIDTH_PIXELS) mapped_pos = WORLD_WIDTH_PIXELS - 1;
	return mapped_pos;

}

posy_pixel world_pixel_to_world_pixel_y_no_band_param (posy_pixel pos_y, posy_pixel y_diff) {
	posx_pixel mapped_pos = pos_y + y_diff;

	if (mapped_pos < 0) mapped_pos = 0;
	if (mapped_pos >= WORLD_HEIGHT_PIXELS) mapped_pos = WORLD_HEIGHT_PIXELS - 1;
	return mapped_pos;
}




