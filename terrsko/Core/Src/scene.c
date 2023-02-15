#include <stdlib.h>
#include <math.h>

#include "enums.h"
#include "materials.h"
#include "ugui.h"
#include "scene.h"


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

int WORLD[WORLD_HEIGHT][WORLD_WIDTH];
int SCENE[60][80];

//coord camera_center = {
//		x: 0,
//		y: 0
//};

int camera_x = 0;
int camera_y = 0;


// Initialize world, spawn in height/2, width/2, measured in blocks of 4x4, only call once per level, use enums to mark materials
int init_world() {

	int zero_height = (int) floor(WORLD_HEIGHT/2);

	// Set camera center to the middle of the world
	update_camera_center((int) floor(WORLD_WIDTH/2), zero_height);

	// Everything under zero_height is either dirt or rock
	// background is either sky or bg_dirt
	init_bg(zero_height);

	// Generate level with destroyables


	return 0;
}

void get_scene() {
	int tl_x = camera_x - 40;
	int tl_y = camera_y - 30;
	int bl_x = camera_x + 40;
	int bl_y = camera_y + 30;


	int x = 0;
	int y = 0;
	for (int i = tl_y; i < bl_y; i++) {
		for (int j = tl_x; j < bl_x; j++) {
			SCENE[y][x] = WORLD[i][j];
			x++;
		}
		y++;
	}
}

void update_camera_center(int x, int y) {
	camera_x = x;
	camera_y = y;
}

// Initialize background materials
void init_bg(int z_height) {
	Material material = _sky;
	for (int i = 0; i < WORLD_HEIGHT; i++) {
		for (int j = 0; j < WORLD_WIDTH; j++) {
			if (i < z_height) {
				material = _sky;
			} else {
				material = _dirt_bg;
			}
			WORLD[i][j] = material;
		}
	}
}

