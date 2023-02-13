#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>

#include "ugui.h"

// 320x240 px

#define WIDTH 320
#define HEIGHT 240
#define BLOCK_WIDTH 4



typedef struct _coord {
	uint16_t x;
	uint16_t y;
} coord;

// The most basic object - block of 3x3 pixels
typedef struct _block {
	coord* pos;
	uint8_t width;
} block;

typedef struct _destructible {
	char* type;		// dirt, wood ....
	block* block;
	void* data;
} destroyable;


/* Creates block with given coordinates and width */
block* create_block(uint16_t x, uint16_t y, uint8_t width) {
	 block* block = malloc(sizeof(block));
	 block->pos = malloc(sizeof(coord));
	 block->pos->x = x;
	 block->pos->y = y;
	 block->width = width;

	 return block;
}

// Create wrapper for block - destroyables
destroyable* create_destroyable(char* type, uint16_t x, uint16_t y, uint8_t width, void* data) {
	block* block = create_block(x, y, width);

	destroyable* destroyable = malloc(sizeof(destroyable));
	destroyable->block = block;
	destroyable->type = type;
	destroyable->data = data;
	return destroyable;
}


// Draws block of dirt
void draw_dirt(block* block) {
	//char* colors[2] = {"C_BROWN", "C_SADDLE_BROWN"};

	UG_FillFrame(block->pos->x - 2, block->pos->y - 2, block->pos->x, block->pos->y, C_BROWN);
	UG_FillFrame(block->pos->x, block->pos->y - 2, block->pos->x+2, block->pos->y, C_BROWN);
	UG_FillFrame(block->pos->x-2, block->pos->y, block->pos->x, block->pos->y+2, C_SADDLE_BROWN);
	UG_FillFrame(block->pos->x, block->pos->y, block->pos->x+2, block->pos->y+2, C_BROWN);
}

// Draws block of dirt
void draw_grass(block* block) {
	//char* colors[2] = {"C_BROWN", "C_SADDLE_BROWN"};

	UG_FillFrame(block->pos->x - 2, block->pos->y - 2, block->pos->x, block->pos->y, C_GREEN);
	UG_FillFrame(block->pos->x, block->pos->y - 2, block->pos->x+2, block->pos->y, C_GREEN);
	UG_FillFrame(block->pos->x-2, block->pos->y, block->pos->x, block->pos->y+2, C_DARK_GREEN);
	UG_FillFrame(block->pos->x, block->pos->y, block->pos->x+2, block->pos->y+2, C_DARK_GREEN);
}



/* Draws a red block */
void draw_block(block* block) {
	int diff = floor(block->width/2);
	UG_FillFrame(block->pos->x - diff, block->pos->y - diff, block->pos->x + diff, block->pos->y + diff, C_RED);
}

/* Frees the memory of a block pointer */
void free_block(block* block) {
	free(block->pos);
	free(block);
}

void free_destroyable(destroyable* destroyable) {
	free(destroyable->block);
	free(destroyable);
}

