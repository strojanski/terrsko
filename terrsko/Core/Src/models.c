#include <stdio.h>
#include <stdlib.h>

typedef struct _ground {
    int center_x;
    int center_y;
    int adjacent_pixels[100];
} ground;


ground* define_floor(int center_x, int center_y) {
	ground* floor = (ground*) malloc(sizeof(ground));
	floor->center_x = center_x;
	floor->center_y = center_y;
	return floor;
}

void free_floor(ground* floor) {
	free(floor);
}
