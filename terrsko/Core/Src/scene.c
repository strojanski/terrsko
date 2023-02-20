#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>

#include "enums.h"
#include "materials.h"
#include "ugui.h"
#include "scene.h"

// 512kB flash drive, 128kB RAM

#define CAVE_THRESH 40
#define CAVE_ITER 100
#define CAVE_BIRTH_THRESH 2
#define CAVE_DEATH_THRESH 6

/*
 * WORLD array of ints HxW
 * 	- stores center of LCD
 *
 * SCENE visible part of world
 *
 * DRAW SCENE
 * 	- draws visible scene, determines position of player
 *
 *
 *
 * */

uint8_t WORLD[WORLD_HEIGHT][WORLD_WIDTH/2];		// 10KB
uint8_t SCENE[SCENE_HEIGHT][SCENE_WIDTH/2];		// 2.3KB

int8_t HEIGHT_MAP[WORLD_WIDTH/HMAP_SAMPLES_PER_CELL+1][WORLD_WIDTH/HMAP_SAMPLES_PER_CELL+1];	// Requires size 2^n + 1 in each dimension ie. [9][161]
uint8_t CAVE_MAP[WORLD_HEIGHT/CAVE_SAMPLES_PER_CELL][WORLD_WIDTH/(2*CAVE_SAMPLES_PER_CELL)];
int8_t LVL1_HMAP[WORLD_WIDTH];

//coord camera_center = {
//		x: 0,
//		y: 0
//};

uint16_t camera_x = 0;
uint16_t camera_y = 0;

// Initialize world, spawn in height/2, width/2, measured in blocks of 4x4, only call once per level, use enums to mark materials
void init_world() {

	generate_height_map(-3, 5, 4);
	generate_caves();

	// Generate level with destroyables
	init_stage_0();

	uint16_t zero_height = LVL1_HMAP[WORLD_WIDTH/2];

	// Set camera center to the middle of the world
	update_camera_center((uint16_t) floor(WORLD_WIDTH/4), zero_height - SKY_GROUND_OFFSET);	// zero level height should be at 1/3 of the screen
}

void get_scene() {
	uint16_t left = camera_x - (SCENE_WIDTH / 4);
	uint8_t top = camera_y - (SCENE_HEIGHT / 2);
	uint16_t right = camera_x + (SCENE_WIDTH / 4);
	uint8_t bottom = camera_y + (SCENE_HEIGHT / 2);


	uint16_t x = 0;
	uint16_t y = 0;
	for (uint16_t i = top; i < bottom; i++) {
		for (uint16_t j = left; j < right; j++) {
			SCENE[y][x] = WORLD[i][j];
			x++;
		}
		x = 0;
		y++;
	}
}

void update_camera_center(uint16_t x, uint8_t y) {
	if (x >= WORLD_WIDTH - 40) {
		x = WORLD_WIDTH - 40;
	} else if (x < 40) {
		x = 40;
	}

	if (y >= WORLD_HEIGHT - 30) {
		y = WORLD_HEIGHT - 30;
	} else if (y < 30) {
		y = 30;
	}

	camera_x = x;
	camera_y = y;
}

/*
int get_time_of_day() {

}
*/

bool is_night() {
	srand(time(NULL));
	float random = (float)rand() / RAND_MAX;

	if (random < 0.5) {
		return true;
	}
	return false;
}


// Get level with only dirt
void init_stage_0() {

	srand(time(NULL));

	float probability_rock = 0.03;

	for (uint16_t i = 0; i < WORLD_HEIGHT; i++) {
		for (uint16_t j = 0; j < WORLD_WIDTH; j+=2) {
			uint8_t l_block; uint8_t r_block;
			uint8_t prev_val_l = (WORLD[i][j/2] & 0xF0) >> 4;
			uint8_t prev_val_r = WORLD[i][j/2] & 0x0F;
			bool cave = false;


			// Skip caves
			if (prev_val_l == (uint8_t)_dirt_bg) {
				cave = true;
			}

			// left block
			if (i > LVL1_HMAP[j]) { // Ground


				float random = (float) rand() / RAND_MAX;

				// Add random rocks
				if (random < probability_rock && abs(LVL1_HMAP[j] - i) > 2) {	// Rocks at least 2 dirt deep
					l_block = _rock;
				} else {
					l_block = _dirt;		// Set 2 cells at once
				}

			} else if (i == LVL1_HMAP[j]) {
				l_block = _grass;
			} else {
				l_block = _empty;
			}

			// Skip caves
			if (prev_val_r == (uint8_t) _dirt_bg) {
				cave = true;
			}
			// left block
			if (i > LVL1_HMAP[j+1]) { // Ground

				float random = (float) rand() / RAND_MAX;

				// Add random rocks
				if (random < probability_rock && abs(LVL1_HMAP[j+1] - i) > 2) {
					r_block = _rock;
				} else {
					r_block = _dirt;		// Set 2 cells at once
				}

			} else if (i == LVL1_HMAP[j+1]) {
				r_block = _grass;
			} else {
				r_block = _empty;
			}

			if (!cave) {
				WORLD[i][j/2] = (l_block << 4) | r_block;
			} else {
				WORLD[i][j/2] = ((uint8_t) _dirt_bg << 4) | (uint8_t) _dirt_bg;
			}
		}
	}
}

void generate_caves() {

	uint8_t cave_value = _dirt_bg << 4 | _dirt_bg;
	uint8_t dirt_value = _dirt << 4 | _dirt;

	uint16_t map_width = WORLD_WIDTH/(2*CAVE_SAMPLES_PER_CELL);
	uint16_t map_height = WORLD_HEIGHT/CAVE_SAMPLES_PER_CELL;

	// Randomly set some cells to zero
	for (uint16_t y = 0; y < map_height; y++) {	// Caves start 5 blocks under ground
		for (uint16_t x = 0; x < map_width; x++) {
			if (rand() % 100 < CAVE_THRESH) {
				CAVE_MAP[y][x] = cave_value;
			}
		}
	}

	// Cellular automata rules
	for (int iter = 0; iter < CAVE_ITER; iter++) {
		for (int j = 0; j < map_width; j++) {
			for (int i = 0; i < map_height; i++) {

				if (i * CAVE_SAMPLES_PER_CELL < LVL1_HMAP[j*CAVE_SAMPLES_PER_CELL]) continue;

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
						CAVE_MAP[i-1][j] = cave_value;
						CAVE_MAP[i+1][j] = cave_value;
					}
				} else {
					if (neighbor_cave_count < CAVE_BIRTH_THRESH || neighbor_cave_count > CAVE_DEATH_THRESH) {
						CAVE_MAP[i][j] = dirt_value;
					}
				}
			}
		}
	}

	for (uint16_t x = 0; x < WORLD_WIDTH/2; x += CAVE_SAMPLES_PER_CELL) {
		int depth = random_int(4, 8);

		for (uint16_t y = LVL1_HMAP[2*x] + depth; y < WORLD_HEIGHT; y+= CAVE_SAMPLES_PER_CELL) {


			for (uint16_t cx = 0; cx < CAVE_SAMPLES_PER_CELL; cx++) {
				for (int16_t cy = 0; cy < CAVE_SAMPLES_PER_CELL; cy++) {

					// Add smoothing
					if (y + cy > LVL1_HMAP[2*x+cx]) {
						WORLD[y+cy][x+cx] = CAVE_MAP[y][x];
					}
				}
			}

		}
	}
}

// Generate 2D height map, a row can represent a level, can also be used for different tones of background underground (Worms style), Diamond-Square alg (https://en.wikipedia.org/wiki/Diamond-square_algorithm)
void generate_height_map(uint8_t random_lower, uint8_t random_upper, float roughness) {

	srand(time(NULL));

	uint16_t map_size = WORLD_WIDTH + 1;

	// Initialize heights - height 0 equals WORLD_HEIGHT / 2
	HEIGHT_MAP[0][0] = 16;			// Elevated on edge
	HEIGHT_MAP[0][map_size-1] = 4;
	HEIGHT_MAP[map_size-1][0] = 5;	// Elevated on edge
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
	for (uint16_t i = 0; i < WORLD_WIDTH; i+=HMAP_SAMPLES_PER_CELL) {
		uint8_t val = HEIGHT_MAP[0][i/HMAP_SAMPLES_PER_CELL] / HMAP_SAMPLES_PER_CELL + GROUND_SKY_RATIO;
		for (uint16_t j = 0; j < HMAP_SAMPLES_PER_CELL; j++) {
			LVL1_HMAP[i+j] = val;
		}
	}

	// LVL1_HMAP is as wide as the world, smooth the bumps
	filter_level(WORLD_WIDTH, KERNEL_WIDTH, LEVEL_SMOOTHING_FACTOR);

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

// Smoothes level
void filter_level(uint16_t array_size, uint8_t kernel_width, uint8_t sigma) {
	int8_t* result = malloc(array_size);

	float* filter = gauss_kernel(kernel_width, sigma);

	for (uint8_t i = 0; i < array_size; i++) {
		float sum = 0.0;
		for (int j = 0; j < kernel_width; j++) {
			int k = i + j - (kernel_width - 1) / 2;
			if (k >= 0 && k < array_size) {
				sum += LVL1_HMAP[k] * filter[j];
			}
		}
		result[i] = (int8_t) round(sum);
	}

	// Write back
	for (uint8_t i = 0; i < array_size; i++) {
		LVL1_HMAP[i] = result[i];
	}

	free(filter);
	free(result);
}

uint8_t random_int(uint8_t min, uint8_t max) {
    return (uint8_t) rand() % (max - min + 1) + min;
}
