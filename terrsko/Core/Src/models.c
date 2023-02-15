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
block* create_block(uint16_t x, uint8_t y, uint16_t colors[4]) {
	 block* block = malloc(sizeof(block));
	 block->pos = malloc(sizeof(coord));
	 block->pos->x = x;
	 block->pos->y = y;

	 for (uint8_t i = 0; i < 4; i++) {
		 block->colors[i] = colors[i];
	 }

	 return block;
}

// Create wrapper for block - destroyables
destroyable* create_destroyable(half_byte type, uint16_t x, uint8_t y, uint16_t colors[4]) {
	block* block = create_block(x, y, colors);

	destroyable* destroyable = malloc(sizeof(destroyable));
	destroyable->block = block;
	destroyable->type = type;
	return destroyable;
}

bg_material* create_bg_material(half_byte type, uint16_t x, uint8_t y, uint16_t colors[4]) {
	block* block = create_block(x, y, colors);

	bg_material* bg_material= malloc(sizeof(bg_material));
	bg_material->block = block;
	bg_material->type = type;
	return bg_material;
}


// TODO consider adding another struct for each type of block rather than only having destroyables of different types


/* Draws a block with its colors */
void draw_block(block* block) {

	UG_FillFrame(block->pos->x - 4, block->pos->y - 4, block->pos->x-2, block->pos->y-2, block->colors[0]);
	UG_FillFrame(block->pos->x-2, block->pos->y-4, block->pos->x, block->pos->y-2, block->colors[1]);
	UG_FillFrame(block->pos->x-4, block->pos->y-2, block->pos->x-2, block->pos->y, block->colors[2]);
	UG_FillFrame(block->pos->x-2, block->pos->y-2, block->pos->x, block->pos->y, block->colors[3]);
}




void draw_scene() {
	// update and get scene
	get_scene();

	for (uint8_t i = 0; i < SCENE_BLOCKS_X; i++) {
		for (uint8_t j = 0; j < SCENE_BLOCKS_Y; j++) {

			half_byte value = SCENE[j][i]; // SCENE[y][x]

			if (value.val == _dirt) {

				half_byte d = { val: _dirt};
				destroyable* dirt = create_destroyable(d, 4*(i+1), 4*(j+1), C_DIRT);

				draw_block(dirt->block);
				free_destroyable(dirt);
			} else if (value.val == _grass) {

				half_byte g = { val: _grass };
				destroyable* grass = create_destroyable(g, 4*(i+1), 4*(j+1), C_GRASS);

				draw_block(grass->block);
				free_destroyable(grass);
			} else if (value.val == _sky) {

				half_byte s = { val: _sky};
				bg_material* sky = create_bg_material(s, 4*(i+1), 4*(j+1), C_SKY);

				draw_block(sky->block);
				free_bg_material(sky);
			} else if (value.val == _dirt_bg) {
				half_byte d = { val: _dirt_bg};
				bg_material* dirt_bg = create_bg_material(d, 4*(i+1), 4*(j+1), C_BG_DIRT);


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


