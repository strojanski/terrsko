#include <stdio.h>
#include <stdlib.h>

// 240x320 px

typedef struct _coord {
	uint8_t x;
	uint8_t y;
} coord;

typedef struct _ground {
	coord* center;
    coord* adjacent_pixels[240];
} ground;


ground* define_floor(int center_x, int center_y) {
	ground* floor = (ground*) malloc(sizeof(ground));
	floor->center = (coord*) malloc(sizeof(coord));
	floor->center->x = center_x;
	floor->center->y = center_y;

	for (uint8_t i = 0; i < 240; i++) {
		// Each element is a pointer to a struct
		floor->adjacent_pixels[i] = (coord*) malloc(sizeof(coord));
		floor->adjacent_pixels[i]->x = i;
		floor->adjacent_pixels[i]->y = center_y;
	}

	return floor;
}

void free_floor(ground* floor) {
	for (int i = 0; i < 240; i++) {
		free(floor->adjacent_pixels[i]);
	}
	free(floor->center);
	free(floor);
}
