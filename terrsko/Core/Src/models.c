#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>

#include "ugui.h"
#include "scene.h"
#include "enums.h"
#include "materials.h"
#include "models.h"

// 320x240 px

/* Creates block with given coordinates and width */
block* create_block(uint16_t x, uint16_t y, uint8_t width, int colors[4]) {
	 block* block = malloc(sizeof(block));
	 block->pos = malloc(sizeof(coord));
	 block->pos->x = x;
	 block->pos->y = y;
	 block->width = width;

	 for (uint8_t i = 0; i < 4; i++) {
		 block->colors[i] = colors[i];
	 }

	 return block;
}

// Create wrapper for block - destroyables
destroyable* create_destroyable(char* type, uint16_t x, uint16_t y, uint8_t width, int colors[4], void* data) {
	block* block = create_block(x, y, width, colors);

	destroyable* destroyable = malloc(sizeof(destroyable));
	destroyable->block = block;
	destroyable->type = type;
	destroyable->data = data;
	return destroyable;
}

bg_material* create_bg_material(char* type, uint16_t x, uint16_t y, uint8_t width, int colors[4], void* data) {
	block* block = create_block(x, y, width, colors);

	bg_material* bg_material= malloc(sizeof(bg_material));
	bg_material->block = block;
	bg_material->type = type;
	bg_material->data = data;
	return bg_material;
}


// TODO consider adding another struct for each type of block rather than only having destroyables of different types


/* Draws a block with its colors */
void draw_block(block* block) {

	UG_FillFrame(block->pos->x - 2, block->pos->y - 2, block->pos->x, block->pos->y, block->colors[0]);
	UG_FillFrame(block->pos->x, block->pos->y - 2, block->pos->x+2, block->pos->y, block->colors[1]);
	UG_FillFrame(block->pos->x-2, block->pos->y, block->pos->x, block->pos->y+2, block->colors[2]);
	UG_FillFrame(block->pos->x, block->pos->y, block->pos->x+2, block->pos->y+2, block->colors[3]);
}




void draw_scene() {
	// update and get scene
	get_scene();

	for (int i = 0; i < SCENE_BLOCKS_Y; i++) {
		for (int j = 0; j < SCENE_BLOCKS_X; j++) {

			int value = SCENE[i][j];
			// SCENE[y][x]

			if (value == _dirt) {
				destroyable* dirt = create_destroyable("dirt", 4*j, 4*i, BLOCK_WIDTH, C_DIRT, NULL);
				draw_block(dirt->block);
				free_destroyable(dirt);
			} else if (value == _grass) {
				destroyable* grass = create_destroyable("grass", 4*j, 4*i, BLOCK_WIDTH, C_GRASS, NULL);
				draw_block(grass->block);
				free_destroyable(grass);
			} else if (value == _sky) {
				bg_material* sky = create_bg_material("sky", 4*j, 4*i, BLOCK_WIDTH, C_SKY, NULL);
				draw_block(sky->block);
				free_bg_material(sky);
			} else if (value == _dirt_bg) {
				bg_material* dirt_bg = create_bg_material("dirt_bg", 4*j, 4*i, BLOCK_WIDTH, C_BG_DIRT, NULL);
				draw_block(dirt_bg->block);
				free_bg_material(dirt_bg);
			}

		}
	}
}

void free_destroyable(destroyable* destroyable) {
	free(destroyable->block);
	free(destroyable);
}


void free_bg_material(bg_material* mat) {
	free(mat->block);
	free(mat);
}


/* Frees the memory of a block pointer */
void free_block(block* block) {
	free(block->pos);
	free(block);
}


