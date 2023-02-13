#include <stdio.h>
#include <stdlib.h>

#define MODELS_H


typedef struct _coord {
	uint8_t x;
	uint8_t y;
} coord;

typedef struct _block {
	coord* pos;
	uint8_t width;
} block;

typedef struct _ground {
	coord* center;
    coord* adjacent_pixels[240];
} ground;

block* create_block(uint16_t x, uint16_t y, uint8_t width);

void free_block(block* block);

void draw_block(block* block);

ground* define_floor(int center_x, int center_y);

void free_floor(ground* floor);
