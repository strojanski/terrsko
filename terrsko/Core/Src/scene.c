#include <stdlib.h>

#include "enums.h"
#include "materials.h"
#include "ugui.h"

#define BLOCK_WIDTH 4



// Initialize world, spawn in height/2, width/2, measured in blocks of 4x4
int** init_world(int w_height, int w_width) {
	int WORLD[w_height][w_width];

	return NULL;
}



// Initial scene - Only the visible parts
int** init_scene() {
	int SCENE[60][80];

	for (int i = 44; i < 60; i++) {
		for (int j = 0; i < 80; j++) {

			enum destroyables material;
			// Grass
			if (i == 44) {
				material = grass;
				SCENE[i][j] = material;
			} else {
				// Dirt
				//enum destroyables material;
				material = dirt;
				SCENE[i][j] = material;
			}
		}
	}

	return SCENE;
}
