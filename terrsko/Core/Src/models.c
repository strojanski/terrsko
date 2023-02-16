#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#include "ugui.h"
#include "scene.h"
#include "enums.h"
#include "materials.h"
#include "models.h"

// 320x240 px

/* Creates block with given coordinates and width */
block* create_block(uint16_t x, uint8_t y, uint16_t colors[4]) {
	 block* block = (struct _block*) malloc(sizeof(struct _block));
	 block->pos.x = x;
	 block->pos.y = y;

	 for (uint8_t i = 0; i < 4; i++) {
		 block->colors[i] = colors[i];
	 }

	 return block;
}

// Create wrapper for block - destroyables
destroyable* create_destroyable(uint16_t x, uint8_t y, uint16_t colors[4]) {
	block* block = create_block(x, y, colors);

	destroyable* destroyable = (struct _destroyable*) malloc(sizeof(struct _destroyable));
	destroyable->block = block;
	return destroyable;
}

bg_material* create_bg_material(uint16_t x, uint8_t y, uint16_t colors[4]) {
	block* block = create_block(x, y, colors);

	bg_material* bg_material = (struct _bg_material*) malloc(sizeof(struct _bg_material));
	bg_material->block = block;
	return bg_material;
}


// TODO consider adding another struct for each type of block rather than only having destroyables of different types


/* Draws a block with its colors */
void draw_block(block* block) {

	UG_FillFrame(block->pos.x - 4, block->pos.y - 4, block->pos.x-2, block->pos.y-2, block->colors[0]);
	UG_FillFrame(block->pos.x-2, block->pos.y-4, block->pos.x, block->pos.y-2, block->colors[1]);
	UG_FillFrame(block->pos.x-4, block->pos.y-2, block->pos.x-2, block->pos.y, block->colors[2]);
	UG_FillFrame(block->pos.x-2, block->pos.y-2, block->pos.x, block->pos.y, block->colors[3]);
}




void draw_scene() {
	// update and get scene
	get_scene();

	srand(time(NULL));

	uint16_t pos_x1 = 4;
	uint16_t pos_x2 = 8;

	bool night = is_night();

	float probability_star = .02;

	for (uint8_t i = 0; i < SCENE_BLOCKS_X; i++) {
		for (uint8_t j = 0; j < SCENE_BLOCKS_Y; j++) {

			uint8_t value = SCENE[j][i]; // SCENE[y][x]
			uint8_t l_cell = (value & 0xF0) >> 4;
			uint8_t r_cell = (value & 0x0F);

			float random = (float) rand() / RAND_MAX;

			// left (first) cell
			if (l_cell == (uint8_t) _dirt) {

				destroyable* dirt = create_destroyable(pos_x1, 4*(j+1), C_DIRT);

				draw_block(dirt->block);
				free_destroyable(dirt);
			} else if (l_cell == (uint8_t) _grass) {

				destroyable* grass = create_destroyable(pos_x1, 4*(j+1), C_GRASS);

				draw_block(grass->block);
				free_destroyable(grass);
			} else if (l_cell == (uint8_t) _wood) {

				destroyable* wood = create_destroyable(pos_x1, 4*(j+1), C_WOOD);

				draw_block(wood->block);
				free_destroyable(wood);
			} else if (l_cell == (uint8_t) _rock) {

				destroyable* rock = create_destroyable(pos_x1, 4*(j+1), C_ROCK);

				draw_block(rock->block);
				free_destroyable(rock);
			} else {
				if (j < LVL1_HMAP[i]) {

					uint16_t* color = C_SKY;

					if (random < probability_star && night) {
						color = C_STAR;
					} else if (night) {
						color = C_NIGHT_SKY;
					}

					// SKY
					bg_material* sky = create_bg_material(pos_x1, 4*(j+1), color);
					draw_block(sky->block);
					free_bg_material(sky);
				} else {
					// DIRT BG
					bg_material* dirt = create_bg_material(pos_x1, 4*(j+1), C_BG_DIRT);
					draw_block(dirt->block);
					free_bg_material(dirt);
				}
			}

			// right (second) cell
			if (r_cell == (uint8_t) _dirt) {

				destroyable* dirt = create_destroyable(pos_x2, 4*(j+1), C_DIRT);

				draw_block(dirt->block);
				free_destroyable(dirt);
			} else if (r_cell == (uint8_t) _grass) {

				destroyable* grass = create_destroyable(pos_x2, 4*(j+1), C_GRASS);

				draw_block(grass->block);
				free_destroyable(grass);
			} else if (r_cell == (uint8_t) _wood) {

				destroyable* wood = create_destroyable(pos_x2, 4*(j+1), C_WOOD);

				draw_block(wood->block);
				free_destroyable(wood);
			} else if (r_cell == (uint8_t) _rock) {

				destroyable* rock = create_destroyable(pos_x2, 4*(j+1), C_ROCK);

				draw_block(rock->block);
				free_destroyable(rock);
			}
			else {
				if (j < LVL1_HMAP[i]) {

					// SKY
					bg_material* sky = create_bg_material(pos_x2, 4*(j+1), night ? C_NIGHT_SKY : C_SKY);
					draw_block(sky->block);
					free_bg_material(sky);
				} else {
					// DIRT BG
					bg_material* dirt = create_bg_material(pos_x2, 4*(j+1), C_BG_DIRT);
					draw_block(dirt->block);
					free_bg_material(dirt);
				}
			}
		}
		// Each cell in x-direction in SCENE = move 2 blocks on screen
		pos_x1 += 8;
		pos_x2 += 8;
	}
}

void free_destroyable(destroyable* destroyable) {
	free_block(destroyable->block);
	free(destroyable);
}


void free_bg_material(bg_material* mat) {
	free_block(mat->block);
	free(mat);
}


/* Frees the memory of a block pointer */
void free_block(block* block) {
	free(block);
}


