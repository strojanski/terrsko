#include <stdlib.h>

#define BLOCK_WIDTH 4

enum destroyables {
	dirt = 1,
	grass = 2
};


// Initial scene - 1 = dirt
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
