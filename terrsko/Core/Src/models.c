#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "ugui.h"

// 320x240 px

#define WIDTH 320
#define HEIGHT 240


typedef struct _coord {
	uint16_t x;
	uint16_t y;
} coord;

// The most basic object - block of 3x3 pixels
typedef struct _block {
	coord* pos;
	uint8_t width;
} block;

typedef struct _dirt {
	block* block;
	char* color;
};


typedef struct _ground {
	coord* center;
    coord* adjacent_pixels[WIDTH];
} ground;

block* create_block(uint16_t x, uint16_t y, uint8_t width) {
	 block* block = malloc(sizeof(block));
	 block->pos = malloc(sizeof(coord));
	 block->pos->x = x;
	 block->pos->y = y;
	 block->width = width;

	 return block;
}

void free_block(block* block) {
	free(block->pos);
	free(block);
}

void draw_block(block* block) {
	int diff = floor(block->width/2);
	UG_FillFrame(block->pos->x - diff, block->pos->y - diff, block->pos->x + diff, block->pos->y + diff, C_RED);
}

ground* define_floor(int center_x, int center_y) {
	center_y = HEIGHT - center_y;
	ground* floor = (ground*) malloc(sizeof(ground));
	floor->center = (coord*) malloc(sizeof(coord));
	floor->center->x = center_x;
	floor->center->y = center_y;

	for (uint16_t i = 0; i < WIDTH; i++) {
		// Each element is a pointer to a struct
		floor->adjacent_pixels[i] = (coord*) malloc(sizeof(coord));
		floor->adjacent_pixels[i]->x = i;
		floor->adjacent_pixels[i]->y = center_y;
	}

	return floor;
}

void free_floor(ground* floor) {
	for (uint16_t i = 0; i < WIDTH; i++) {
		free(floor->adjacent_pixels[i]);
	}
	free(floor->center);
	free(floor);
}
