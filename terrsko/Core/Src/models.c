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

#define RGB565_RED 0xF800
#define RGB565_GREEN 0x7E0
#define RGB565_BLUE 0x1F

// 320x240 pi
uint16_t* apply_shading(uint16_t colors[4], float illumination) {

	for (uint8_t i = 0; i < 4; i++) {
		uint16_t g = (colors[i] & RGB565_GREEN) >> 5;
		uint16_t r = (colors[i] & RGB565_RED) >> 11;
		uint16_t b = colors[i] & RGB565_BLUE;

		// For dirt, remove less red and green to get a better color
		if (colors[i] == C_DIRT[0]) {
			r = (uint8_t) (r * MIN(1, illumination * 1.3));
			g = (uint8_t) (g * MIN(1, illumination * 1.3));

			b = (uint8_t) (b * illumination);
		} else if (colors[i] == C_BG_DIRT[0]) {
			// Leave extra red in caves
			r = (uint8_t) (r * MIN(1, illumination * 1.5));

			g = (uint8_t) (g * illumination);
			b = (uint8_t) (b * illumination);
		}
		else {
			r = (uint8_t) (r * illumination);
			g = (uint8_t) (g * illumination);
			b = (uint8_t) (b * illumination);
		}

		// MAKE SURE TO 0 PAD TO GET RGB OF LENGTH 4
		uint16_t rgb = (r << 11) | (g << 5) | (b);

		colors[i] = rgb;
	}

	return colors;
}

/* Creates block with given coordinates and width */
block* create_block(uint16_t x, uint16_t y, uint16_t colors[4], uint8_t type, float illumination) {
	 block* block = (struct _block*) malloc(sizeof(struct _block));
	 block->pos.x = x;
	 block->pos.y = y;
	 block->type = type;

	 uint16_t copy[4];
	 for (uint8_t i = 0; i < 4; i++) {
		 copy[i] = colors[i];
	 }

	 uint16_t* new_colors = apply_shading(copy, illumination);

	 for (uint8_t i = 0; i < 4; i++) {
		 block->colors[i] = new_colors[i];
	 }

	 return block;
}

// Create wrapper for block - destroyables
destroyable* create_destroyable(uint16_t x, uint16_t y, uint16_t colors[4], uint8_t type, float illumination) {
	block* block = create_block(x, y, colors, type, illumination);

	destroyable* destroyable = (struct _destroyable*) malloc(sizeof(struct _destroyable));
	destroyable->block = block;
	return destroyable;
}

bg_material* create_bg_material(uint16_t x, uint16_t y, uint16_t colors[4], uint8_t type, float illumination) {
	block* block = create_block(x, y, colors, type, illumination);

	bg_material* bg_material = (struct _bg_material*) malloc(sizeof(struct _bg_material));
	bg_material->block = block;
	return bg_material;
}

/* Draws a block with its colors */
void draw_block(block* block) {

	UG_FillFrame(block->pos.x-4, block->pos.y-4, block->pos.x-2, block->pos.y-2, block->colors[0]);
	UG_FillFrame(block->pos.x-2, block->pos.y-4, block->pos.x, block->pos.y-2, block->colors[1]);
	UG_FillFrame(block->pos.x-4, block->pos.y-2, block->pos.x-2, block->pos.y, block->colors[2]);
	UG_FillFrame(block->pos.x-2, block->pos.y-2, block->pos.x, block->pos.y, block->colors[3]);
}

// Black outline
void draw_detailed_block(block* block) {
	draw_block(block);
	UG_DrawLine(block->pos.x-4, block->pos.y-4, block->pos.x, block->pos.y-4, C_BLACK);
	UG_DrawLine(block->pos.x-4, block->pos.y-4, block->pos.x-4, block->pos.y, C_BLACK);
	UG_DrawLine(block->pos.x, block->pos.y-4, block->pos.x, block->pos.y, C_BLACK);
	UG_DrawLine(block->pos.x-4, block->pos.y, block->pos.x, block->pos.y, C_BLACK);
}

void draw_scene() {
	// update and get scene
	get_scene();

	srand(time(NULL));

	uint16_t pos_x1 = 4;
	uint16_t pos_x2 = 8;

	bool night = is_night();

	float probability_star = .02;
	float illumination = 1;

	for (uint16_t i = 0; i < SCENE_BLOCKS_X; i++) {
		for (uint16_t j = 0; j < SCENE_BLOCKS_Y; j++) {

			uint8_t value = SCENE[j][i]; // SCENE[y][x]
			uint8_t l_cell = (value & 0xF0) >> 4;
			uint8_t r_cell = (value & 0x0F);

			illumination = compute_illumination(i, j);

			float random = (float) rand() / RAND_MAX;
			// left (first) cell
			if (l_cell == (uint8_t) _dirt) {

				destroyable* dirt = create_destroyable(pos_x1, 4*(j+1), C_DIRT, _dirt, illumination);

				draw_block(dirt->block);
				free_destroyable(dirt);
			} else if (l_cell == (uint8_t) _grass) {

				destroyable* grass = create_destroyable(pos_x1, 4*(j+1), C_GRASS, _grass, illumination);

				draw_block(grass->block);
				free_destroyable(grass);
			} else if (l_cell == (uint8_t) _lava) {

				destroyable* lava = create_destroyable(pos_x1, 4*(j+1), C_LAVA, _lava, illumination);

				draw_block(lava->block);
				free_destroyable(lava);
			} else if (l_cell == (uint8_t) _wood) {

				destroyable* wood = create_destroyable(pos_x1, 4*(j+1), C_WOOD, _wood, illumination);

				draw_block(wood->block);
				free_destroyable(wood);
			} else if (l_cell == (uint8_t) _rock) {

				destroyable* rock = create_destroyable(pos_x1, 4*(j+1), C_ROCK, _rock, illumination);

				draw_block(rock->block);
				free_destroyable(rock);
			} else if (l_cell == (uint8_t) _dirt_bg) {

				bg_material* dirt = create_bg_material(pos_x1, 4*(j+1), C_BG_DIRT, _dirt_bg, illumination);

				draw_block(dirt->block);
				free_bg_material(dirt);
			} else {
				if (j < LVL1_HMAP[camera_x - SCENE_WIDTH/2 + i]) {

					uint16_t* color = C_SKY;

					if (random < probability_star && night) {
						color = C_STAR;
					} else if (night) {
						color = C_NIGHT_SKY;
					}

					// SKY
					bg_material* sky = create_bg_material(pos_x1, 4*(j+1), color, _empty, illumination);
					draw_block(sky->block);
					free_bg_material(sky);
				} else {
					// DIRT BG
					bg_material* dirt = create_bg_material(pos_x1, 4*(j+1), C_BG_DIRT, _dirt_bg, illumination);
					draw_block(dirt->block);
					free_bg_material(dirt);
				}
			}

			// right (second) cell
			if (r_cell == (uint8_t) _dirt) {

				destroyable* dirt = create_destroyable(pos_x2, 4*(j+1), C_DIRT, _dirt, illumination);

				draw_block(dirt->block);
				free_destroyable(dirt);
			} else if (r_cell == (uint8_t) _grass) {

				destroyable* grass = create_destroyable(pos_x2, 4*(j+1), C_GRASS, _grass, illumination);

				draw_block(grass->block);
				free_destroyable(grass);
			} else if (r_cell == (uint8_t) _lava) {

				destroyable* lava = create_destroyable(pos_x2, 4*(j+1), C_LAVA, _lava, illumination);

				draw_block(lava->block);
				free_destroyable(lava);
			}  else if (r_cell == (uint8_t) _wood) {

				destroyable* wood = create_destroyable(pos_x2, 4*(j+1), C_WOOD, _wood, illumination);

				draw_block(wood->block);
				free_destroyable(wood);
			} else if (r_cell == (uint8_t) _rock) {

				destroyable* rock = create_destroyable(pos_x2, 4*(j+1), C_ROCK, _rock, illumination);

				draw_block(rock->block);
				free_destroyable(rock);
			} else if (r_cell == (uint8_t) _dirt_bg) {

				bg_material* dirt = create_bg_material(pos_x2, 4*(j+1), C_BG_DIRT, _dirt_bg, illumination);

				draw_block(dirt->block);
				free_bg_material(dirt);
			} else {
				if (j < LVL1_HMAP[camera_x - SCENE_WIDTH/2 + i]) {

					// SKY
					bg_material* sky = create_bg_material(pos_x2, 4*(j+1), night ? C_NIGHT_SKY : C_SKY, _empty, illumination);
					draw_block(sky->block);
					free_bg_material(sky);
				} else {
					// DIRT BG
					bg_material* dirt = create_bg_material(pos_x2, 4*(j+1), C_BG_DIRT, _dirt_bg, illumination);
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


