#include <stdio.h>
#include <stdlib.h>

#define MODELS_H

typedef struct _coord {
	uint8_t x;
	uint8_t y;
} coord;

typedef struct _ground {
	coord* center;
    coord* adjacent_pixels[240];
} ground;



ground* define_floor(int center_x, int center_y);

void free_floor(ground* floor);
