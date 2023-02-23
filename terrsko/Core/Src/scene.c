#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>

#include "enums.h"
#include "materials.h"
#include "ugui.h"
#include "scene.h"

// 512kB flash drive, 128kB RAM

#define CAVE_THRESH 12 //40
#define CAVE_ITER 10
#define CAVE_BIRTH_THRESH 3
#define CAVE_DEATH_THRESH 5

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

// 4 bits define the block at the position
uint8_t WORLD[WORLD_HEIGHT][WORLD_WIDTH/2];		// 10KB
uint8_t SCENE[SCENE_HEIGHT][SCENE_WIDTH/2];		// 2.3KB

// Only holds bytes
uint8_t CAVE_MAP[WORLD_HEIGHT/CAVE_SAMPLES_PER_CELL][WORLD_WIDTH/(2*CAVE_SAMPLES_PER_CELL)];

// Height can be > 256, need uint16_t
int16_t HEIGHT_MAP[WORLD_WIDTH/HMAP_SAMPLES_PER_CELL+1][WORLD_WIDTH/HMAP_SAMPLES_PER_CELL+1];	// Requires size 2^n + 1 in each dimension ie. [9][161]
int16_t LVL1_HMAP[WORLD_WIDTH];

uint16_t camera_x = 0;
uint16_t camera_y = 0;

// Initialize world, spawn in height/2, width/2, measured in blocks of 4x4, only call once per level, use enums to mark materials
void init_world() {

	generate_height_map(-3, 4, 5);
	generate_caves();
	shape_caves_with_morphological_operations();

	// Generate level with destroyables
	init_stage_0();

	uint16_t zero_height = LVL1_HMAP[WORLD_WIDTH/2];

	// Set camera center to the middle of the world
	update_camera_center((uint16_t) floor(WORLD_WIDTH/2), zero_height - SKY_GROUND_OFFSET);	// zero level height should be at 1/3 of the screen
}

void get_scene() {
	uint16_t left = camera_x - (SCENE_WIDTH / 4);
	uint16_t top = camera_y - (SCENE_HEIGHT / 2);
	uint16_t right = camera_x + (SCENE_WIDTH / 4);
	uint16_t bottom = camera_y + (SCENE_HEIGHT / 2);


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

void update_camera_center(uint16_t x, uint16_t y) {
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

			// Cave
			if (WORLD[i][j/2] == ((_dirt_bg << 4) | _dirt_bg) && i > LVL1_HMAP[j]) {
				continue;
			}

			// left block

			// Ground
			if (i > LVL1_HMAP[j]) {
				float random = (float) rand() / RAND_MAX;

				// Add random rocks
				if (random < probability_rock && abs(LVL1_HMAP[j] - i) > 2) {	// Rocks at least 2 dirt deep
					l_block = _rock;
				} else {
					l_block = _dirt;
				}

			} else if (i == LVL1_HMAP[j]) {
				l_block = _grass;
			} else {
				l_block = _empty;
			}

			// left block
			if (i > LVL1_HMAP[j+1]) { // Ground

				float random = (float) rand() / RAND_MAX;

				// Add random rocks
				if (random < probability_rock && abs(LVL1_HMAP[j+1] - i) > 2) {
					r_block = _rock;
				} else {
					r_block = _dirt;
				}

			} else if (i == LVL1_HMAP[j+1]) {
				r_block = _grass;
			} else {
				r_block = _empty;
			}

			WORLD[i][j/2] = (l_block << 4) | r_block;
		}
	}
}

void generate_caves() {

	uint8_t cave_value = (_dirt_bg << 4) | _dirt_bg;
	uint8_t dirt_value = (_dirt << 4) | _dirt;

	uint16_t map_width = WORLD_WIDTH/(2*CAVE_SAMPLES_PER_CELL);
	uint16_t map_height = WORLD_HEIGHT/CAVE_SAMPLES_PER_CELL;

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
}

void draw_circle(uint16_t x, uint16_t y, uint8_t radius, uint8_t cave_value) {
	uint16_t width = WORLD_WIDTH / (2 * CAVE_SAMPLES_PER_CELL);
	uint16_t height = WORLD_HEIGHT / CAVE_SAMPLES_PER_CELL;

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
		for (uint8_t j = 0; j < HMAP_SAMPLES_PER_CELL; j++) {
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

// Perform morphological opening on the binary mask (cave map) - connect the caves
void shape_caves_with_morphological_operations() {

	uint8_t dirt = (_dirt << 4) | _dirt;		// low val
	uint8_t cave = (_dirt_bg << 4) | _dirt_bg;	// high val

	uint8_t EROSION_SE[SE_SIZE_EROSION][SE_SIZE_EROSION];

	for (uint8_t i = 0; i < SE_SIZE_EROSION; i++) {
		for (uint8_t j = 0; j < SE_SIZE_EROSION; j++) {
			if ((i < SE_SIZE_EROSION/3 || i > 2*SE_SIZE_EROSION/3) && (j < SE_SIZE_EROSION/3 || j > 2*SE_SIZE_EROSION/3)) {
				EROSION_SE[i][j] = dirt;
			} else {
				EROSION_SE[i][j] = cave;
			}
		}
	}

	uint8_t DILATION_SE[SE_SIZE_DILATION][SE_SIZE_DILATION];

	for (uint8_t i = 0; i < SE_SIZE_DILATION; i++) {
		for (uint8_t j = 0; j < SE_SIZE_DILATION; j++) {
			if ((i < SE_SIZE_DILATION/3 || i > 2*SE_SIZE_DILATION/3) && (j < SE_SIZE_DILATION/3 || j > 2*SE_SIZE_DILATION/3)) {
				DILATION_SE[i][j] = dirt;
			} else {
				DILATION_SE[i][j] = cave;
			}
		}
	}

	uint16_t width = WORLD_WIDTH/2;
	uint16_t height = WORLD_HEIGHT;

	dilation(DILATION_SE, width, height);
	erosion(EROSION_SE, width, height);
	dilation(DILATION_SE, width, height);
	erosion(EROSION_SE, width, height);
	dilation(DILATION_SE, width, height);
	erosion(EROSION_SE, width, height);
	dilation(DILATION_SE, width, height);
	dilation(DILATION_SE, width, height);


}

void erosion(uint8_t SE[SE_SIZE_EROSION][SE_SIZE_EROSION], uint16_t map_width, uint16_t map_height) {
	uint8_t se_size = SE_SIZE_EROSION;

	uint8_t temp[map_height][map_width];

//	uint8_t dirt = (_dirt << 4) | _dirt;		// low val
	uint8_t cave = (_dirt_bg << 4) | _dirt_bg;	// high val

	for (uint16_t i = 0; i < map_width; i++) {
		for (uint16_t j = LVL1_HMAP[i]; j < map_height; j++) {
			uint8_t min_value = cave;
			for (uint16_t k = 0; k < se_size; k++) {
				for (uint16_t l = 0; l < se_size; l++) {
					if (SE[k][l] == cave) {
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

void dilation(uint8_t SE[SE_SIZE_DILATION][SE_SIZE_DILATION], uint16_t map_width, uint16_t map_height) {

	uint8_t se_size = SE_SIZE_DILATION;

	uint8_t temp[map_height][map_width];

	uint8_t dirt = (_dirt << 4) | _dirt;		// low val
	uint8_t cave = (_dirt_bg << 4) | _dirt_bg;	// high val

	uint8_t depth = rand() % 5 + 5;

	for (uint16_t i = 0; i < map_width; i++) {
		for (uint16_t j = LVL1_HMAP[i] + depth; j < map_height; j++) {
			uint8_t max_value = dirt;
			for (uint16_t k = 0; k < se_size; k++) {
				for (uint16_t l = 0; l < se_size; l++) {
					if (SE[k][l] == cave) {
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

void filter_level(uint16_t array_size, uint8_t kernel_width, uint8_t sigma) {
	int8_t* result = malloc(array_size);

	float* filter = gauss_kernel(kernel_width, sigma);

	for (uint16_t i = 0; i < array_size; i++) {
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
	for (uint16_t i = 0; i < array_size; i++) {
		LVL1_HMAP[i] = result[i];
	}

	free(filter);
	free(result);
}

uint8_t random_int(uint8_t min, uint8_t max) {
    return (uint8_t) rand() % (max - min + 1) + min;
}
