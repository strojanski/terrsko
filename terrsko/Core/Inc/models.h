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


typedef struct _destructible {
	char* type;		// dirt, wood ....
	block* block;
	void* data;
} destroyable;




block* create_block(uint16_t x, uint16_t y, uint8_t width);

destroyable* create_destroyable(char* type, uint16_t x, uint16_t y, uint8_t width, void* data);

void draw_dirt(block* block);

void draw_block(block* block);

void draw_grass(block* block);

void free_block(block* block);

void free_destroyable(destroyable* destroyable);
