#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>

#include "enums.h"
#include "materials.h"
#include "ugui.h"
#include "scene.h"

// 512kB flash drive, 128kB RAM


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

uint8_t WORLD[WORLD_HEIGHT][WORLD_WIDTH/2];	// 20KB
uint8_t SCENE[SCENE_HEIGHT][SCENE_WIDTH/2];	// 5KB

//coord camera_center = {
//		x: 0,
//		y: 0
//};

uint16_t camera_x = 0;
uint16_t camera_y = 0;
uint16_t GROUND_ZERO = WORLD_HEIGHT / 2 - 15;


// Initialize world, spawn in height/2, width/2, measured in blocks of 4x4, only call once per level, use enums to mark materials
void init_world() {

	uint16_t zero_height = (uint16_t) floor(WORLD_HEIGHT/2);

	// Set camera center to the middle of the world
	update_camera_center((uint16_t) floor(WORLD_WIDTH/4), zero_height-15);	// zero level height should be at 1/3 of the screen
	GROUND_ZERO = zero_height + 15;

	// Everything under zero_height is either dirt or rock
	// background is either sky or bg_dirt
	init_bg(zero_height);

	// Generate level with destroyables


}

void get_scene() {
	uint16_t left = camera_x - (SCENE_WIDTH / 4);
	uint8_t top = camera_y - (SCENE_HEIGHT / 2);
	uint16_t right = camera_x + (SCENE_WIDTH / 4);
	uint8_t bottom = camera_y + (SCENE_HEIGHT / 2);


	uint16_t x = 0;
	uint8_t y = 0;
	for (uint8_t i = top; i < bottom; i++) {
		for (uint16_t j = left; j < right; j++) {
			SCENE[y][x] = WORLD[i][j];
			x++;
		}
		x = 0;
		y++;
	}
}

void update_camera_center(uint16_t x, uint8_t y) {
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


// Initialize background materials
void init_bg(uint16_t z_height) {
	srand(time(NULL));

	float probability_rock = 0.03;

	for (uint16_t i = 0; i < WORLD_HEIGHT; i++) {
		for (uint16_t j = 0; j < WORLD_WIDTH/2; j++) {
			if (i > z_height) {
				float random = (float)rand() / RAND_MAX;

				if (random < probability_rock && abs(z_height - i) > 2) {
					WORLD[i][j] = (_rock << 4) | _dirt;
				} else {
					WORLD[i][j] = (_dirt << 4) | _dirt;		// Set 2 cells at once
				}

			} else if (i == z_height) {
				WORLD[i][j] = (_grass << 4) | _grass;
			} else {
				WORLD[i][j] = 0x00;
			}
		}
	}
}

