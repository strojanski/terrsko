#include <stdlib.h>
#include <math.h>

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

half_byte WORLD[WORLD_HEIGHT][WORLD_WIDTH];	// 10KB
half_byte SCENE[SCENE_HEIGHT][SCENE_WIDTH];	// 5KB

//coord camera_center = {
//		x: 0,
//		y: 0
//};

uint16_t camera_x = 0;
uint16_t camera_y = 0;


// Initialize world, spawn in height/2, width/2, measured in blocks of 4x4, only call once per level, use enums to mark materials
void init_world() {

	uint16_t zero_height = (uint16_t) floor(WORLD_HEIGHT/2);

	// Set camera center to the middle of the world
	update_camera_center((uint16_t) floor(WORLD_WIDTH/2), zero_height);	// zero level height should be at 1/3 of the screen

	// Everything under zero_height is either dirt or rock
	// background is either sky or bg_dirt
	init_bg(zero_height);

	// Generate level with destroyables


}

void get_scene() {
	uint16_t tl_x = camera_x - 20;
	uint8_t tl_y = camera_y - 15;
	uint16_t bl_x = camera_x + 20;
	uint8_t bl_y = camera_y + 15;


	uint16_t x = 0;
	uint8_t y = 0;
	for (uint8_t i = tl_y; i < bl_y; i++) {
		for (uint16_t j = tl_x; j < bl_x; j++) {
			SCENE[y][x].val = WORLD[i][j].val;
			//int val = SCENE[y][x];
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

// Initialize background materials
void init_bg(uint16_t z_height) {
	int material;
	for (uint16_t i = 0; i < WORLD_HEIGHT; i++) {
		for (uint16_t j = 0; j < WORLD_WIDTH; j++) {
			if (i < z_height) {
				material = _sky;
			} else {
				material = _dirt_bg;
			}
			WORLD[i][j].val = material;
		}
	}
}

