#include <stdio.h>
#include <stdlib.h>

#ifndef MODELS_H
#define MODELS_H

#define SCENE_WIDTH 320
#define SCENE_BLOCKS_X 80

#define SCENE_HEIGHT 240
#define SCENE_BLOCKS_Y 60

#define BLOCK_WIDTH 4

typedef struct {
	uint8_t x;
	uint8_t y;
} coord;


typedef struct {
	coord* pos;
	uint8_t width;
	int colors[4];
} block;


typedef struct {
	char* type;		// dirt, wood ....
	block* block;
	void* data;
} destroyable;


typedef struct {
	char* type;
	block* block;
	void* data;
} bg_material;	// background material, can't be interacted with



block* create_block(uint16_t x, uint16_t y, uint8_t width, int colors[4]);

destroyable* create_destroyable(char* type, uint16_t x, uint16_t y, uint8_t width, int colors[4], void* data);

bg_material* create_bg_material(char* type, uint16_t x, uint16_t y, uint8_t width, int colors[4], void* data);

void draw_scene();

void draw_block(block* block);

void free_destroyable(destroyable* destroyable);

void free_bg_material(bg_material* mat);

void free_block(block* block);

#endif
