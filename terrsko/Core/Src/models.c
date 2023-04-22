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
#include "environment_models.h"
#include "utils.h"
#include "structures.h"

#define RGB565_RED 0xF800
#define RGB565_GREEN 0x7E0
#define RGB565_BLUE 0x1F

// Computes colors for some illumination factor
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
		} else {
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
	block *block = (struct _block*) malloc(sizeof(struct _block));
	block->pos.x = x;
	block->pos.y = y;
	block->type = type;

	uint16_t copy[4];
	for (uint8_t i = 0; i < 4; i++) {
		copy[i] = colors[i];
	}

	// Get appropriate colors
//	if (illumination != 1.0) {
//	}
		uint16_t *new_colors = apply_shading(copy, illumination);
		for (uint8_t i = 0; i < 4; i++) {
			block->colors[i] = new_colors[i];
		}


	return block;
}

// Create wrapper for block - destroyables
destroyable* create_destroyable(uint16_t x, uint16_t y, uint16_t colors[4], uint8_t type, float illumination) {
	block *block = create_block(x, y, colors, type, illumination);

	destroyable *destroyable = (struct _destroyable*) malloc(
			sizeof(struct _destroyable));
	destroyable->block = block;
	return destroyable;
}

bg_material* create_bg_material(uint16_t x, uint16_t y, uint16_t colors[4], uint8_t type, float illumination) {
	block *block = create_block(x, y, colors, type, illumination);

	bg_material *bg_material = (struct _bg_material*) malloc(
			sizeof(struct _bg_material));
	bg_material->block = block;
	return bg_material;
}

/* Draws a block with its colors, attempt to draw chunks as big as possible */
void draw_block(block *block) {
//	_HW_FillFrame_(block->pos.x - 4, block->pos.y - 4, block->pos.x, block->pos.y, block->colors[0]);
	if (block->colors[0] == block->colors[1] && block->colors[1] == block->colors[2] && block->colors[2] == block->colors[3]) {
		_HW_FillFrame_(block->pos.x - 4, block->pos.y - 4, block->pos.x, block->pos.y, block->colors[0]);
	} else if (block->colors[0] == block->colors[1]) {	// Top part
		_HW_FillFrame_(block->pos.x - 4, block->pos.y - 4, block->pos.x,
				block->pos.y - 2, block->colors[0]);
		if (block->colors[2] == block->colors[3]) {		// Bottom part
			_HW_FillFrame_(block->pos.x - 4, block->pos.y - 2, block->pos.x,
					block->pos.y, block->colors[2]);
		} else {
			_HW_FillFrame_(block->pos.x - 4, block->pos.y - 2, block->pos.x - 2,
					block->pos.y, block->colors[2]);
			_HW_FillFrame_(block->pos.x - 2, block->pos.y - 2, block->pos.x,
					block->pos.y, block->colors[3]);
		}
	} else if (block->colors[0] == block->colors[2]) {	// Left part
		_HW_FillFrame_(block->pos.x - 4, block->pos.y - 4, block->pos.x - 2,
				block->pos.y, block->colors[0]);
		if (block->colors[1] == block->colors[3]) {		// Right part
			_HW_FillFrame_(block->pos.x - 2, block->pos.y - 4, block->pos.x,
					block->pos.y - 2, block->colors[1]);
		}
		_HW_FillFrame_(block->pos.x - 2, block->pos.y - 4, block->pos.x,
				block->pos.y - 2, block->colors[1]);
		_HW_FillFrame_(block->pos.x - 2, block->pos.y - 2, block->pos.x,
				block->pos.y, block->colors[3]);
	} else {
		_HW_FillFrame_(block->pos.x - 4, block->pos.y - 4, block->pos.x - 2,
				block->pos.y - 2, block->colors[0]);
		_HW_FillFrame_(block->pos.x - 2, block->pos.y - 4, block->pos.x,
				block->pos.y - 2, block->colors[1]);
		_HW_FillFrame_(block->pos.x - 4, block->pos.y - 2, block->pos.x - 2,
				block->pos.y, block->colors[2]);
		_HW_FillFrame_(block->pos.x - 2, block->pos.y - 2, block->pos.x,
				block->pos.y, block->colors[3]);
	}

}

void draw_tree_normal(coord *pos) {
	// 0,0 in top left corner
	for (int j = 0; j < TREE_WIDTH; j++) {
		for (int i = 0; i < TREE_HEIGHT; i++) {
			if (tree[i][j] == 0)
				continue;
			UG_DrawPixel(j + pos->x, i + pos->y, tree[i][j]);
		}
	}
}

//void draw_tree_tall_green(coord* pos) {
//
//	for (int i = 0; i < TREE_TALL_GREEN_HEIGHT; i++) {
//		for (int j = 0; j < TREE_TALL_GREEN_WIDTH; j++) {
//			if (tree_tall_green[i][j] == 0) continue;
//			UG_DrawPixel(j + pos->x, i + pos->y, tree_tall_green[i][j]);
//		}
//	}
//}

//void draw_tree_tall_yellow(coord* pos) {
//
//	for (int i = 0; i < TREE_TALL_YELLOW_HEIGHT; i++) {
//		for (int j = 0; j < TREE_TALL_YELLOW_WIDTH; j++) {
//			if (tree_tall_yellow[i][j] == 0) continue;
//			UG_DrawPixel(j + pos->x, i + pos->y, tree_tall_yellow[i][j]);
//		}
//	}
//}

// Black outline
void draw_detailed_block(block *block) {
	draw_block(block);
	UG_DrawLine(block->pos.x - 4, block->pos.y - 4, block->pos.x,
			block->pos.y - 4, C_BLACK);
	UG_DrawLine(block->pos.x - 4, block->pos.y - 4, block->pos.x - 4,
			block->pos.y, C_BLACK);
	UG_DrawLine(block->pos.x, block->pos.y - 4, block->pos.x, block->pos.y,
			C_BLACK);
	UG_DrawLine(block->pos.x - 4, block->pos.y, block->pos.x, block->pos.y,
			C_BLACK);
}


/*
 * Draws a block accordingly to the enum stored in the SCENE (aka WORLD)
 * **/

void render_block(block_t material, pixel_c pixel_pos_x, pixel_c pixel_pos_y, float illumination, block_c ground_height, block_c current_height) {
	if (material == _empty) {
		return;
	}
	// Determines time and probabilities
	bool night = is_night();
	float probability_star = .0;
	float random = (float) rand() / RAND_MAX;

	if (material == (block_t) _dirt) {

		destroyable *dirt = create_destroyable(pixel_pos_x, pixel_pos_y, C_DIRT, _dirt, illumination);

		draw_block(dirt->block);
		free_destroyable(dirt);
	} else if (material == (block_t) _grass) {

		destroyable *grass = create_destroyable(pixel_pos_x, pixel_pos_y, C_GRASS, _grass, illumination);

		draw_block(grass->block);
		free_destroyable(grass);
	} else if (material == (block_t) _lava) {

		destroyable *lava = create_destroyable(pixel_pos_x, pixel_pos_y, C_LAVA, _lava, illumination);

		draw_block(lava->block);
		free_destroyable(lava);
	} else if (material == (block_t) _wood) {

		destroyable *wood = create_destroyable(pixel_pos_x, pixel_pos_y, C_WOOD, _wood, illumination);

		draw_block(wood->block);
		free_destroyable(wood);
	} else if (material == (block_t) _rock) {

		destroyable *rock = create_destroyable(pixel_pos_x, pixel_pos_y, C_ROCK, _rock, illumination);

		draw_block(rock->block);
		free_destroyable(rock);
	} else if (material == (block_t) _dirt_bg) {

		bg_material *dirt = create_bg_material(pixel_pos_x, pixel_pos_y, C_BG_DIRT, _dirt_bg, illumination);

		draw_block(dirt->block);
		free_bg_material(dirt);
	} else if (material == (block_t) _sky) {

		bg_material *sky = create_bg_material(pixel_pos_x, pixel_pos_y, C_SKY, _sky, illumination);

		draw_block(sky->block);
		free_bg_material(sky);
	} else {
		// Above ground = sky, below ground = dirt_bg
		if (current_height < ground_height) {

			uint16_t *color = C_SKY;

			if (random < probability_star && night) {
				color = C_STAR;
			} else if (night) {
				color = C_NIGHT_SKY;
			}

			// SKY
			bg_material *sky = create_bg_material(pixel_pos_x, pixel_pos_y, color, _sky, illumination);
			draw_block(sky->block);
			free_bg_material(sky);
		} else {
			// DIRT BG
			bg_material *dirt = create_bg_material(pixel_pos_x, pixel_pos_y, C_BG_DIRT, _dirt_bg, illumination);
			draw_block(dirt->block);
			free_bg_material(dirt);
		}
	}
}

void draw_scene(bool init) {
	// update and get scene
	get_scene();
	get_scene_mask();	// 1 - do not overwrite, 0 do overwrite

	srand(time(NULL));

	float illumination = 1;

	// Rendering optimization
	int8_t move_horizontal = old_camera_x - camera_x_block; // + -> left, - -> right
	int8_t move_vertical = old_camera_y - camera_y_block; // + -> up, - -> down

	// If we didn't move don't render the scene at all as it is already rendered
	if (!init && move_horizontal == 0 && move_vertical == 0) {
		return;
	}
	
	// Scene[0][0] in world coordinates in current frame
	block_c world_block_x0 = camera_x_block - SCENE_BLOCKS_X / 2;
	block_c world_block_y0 = camera_y_block - SCENE_BLOCKS_Y / 2;

	// Scene[0][0] in world coordinates in previous frame
	block_c old_world_block_x0 = old_camera_x - SCENE_BLOCKS_X / 2;
	block_c old_world_block_y0 = old_camera_y - SCENE_BLOCKS_Y / 2;

	// Pixel positions for first and second block in x axis
	pixel_c pos_x1 = 4;
	pixel_c pos_x2 = 8;
	
	// Iterate i, j in CELL COORDINATES
	for (cell_c i = 0; i < SCENE_CELLS_X; i++) {
		for (cell_c j = 0; j < SCENE_CELLS_Y; j++) {

			// For indexing cells in SCENE
			cell_c scene_cell_x = i;
			cell_c scene_cell_y = j;
			
			// Coordinates of scene[scene_cell_y][scene_cell_x] in world coordinates in current frame
			cell_c world_cell_x = block_to_cell_x(world_block_x0) + i;
			cell_c world_cell_y = block_to_cell_y(world_block_y0) + j;

			// Coordinates of scene[scene_cell_y][scene_cell_x] in world coordinates in previous frame
			cell_c old_world_cell_x = block_to_cell_x(old_world_block_x0) + i;
			cell_c old_world_cell_y = block_to_cell_y(old_world_block_y0) + j;

			// Cell that we are currently drawing
			cell_t current_scene_cell = SCENE[scene_cell_y][scene_cell_x];
			//cell_t current_world_cell = WORLD[world_cell_y][world_cell_x];

			cell_t old_world_cell = WORLD[old_world_cell_y][old_world_cell_x];

			// Skip the block(s) if it hasn't changed - TODO check each block separately
			bool skip_left = false, skip_right = false;


			if (!init) {
				if (old_world_cell == current_scene_cell) {
					continue;
				} else {
					if (upper(old_world_cell) == upper(current_scene_cell)) {
						skip_left = true;
					} else if (lower(old_world_cell) == lower(current_scene_cell)) {
						skip_right = true;
					}
				}
			}



			block_t left_block = upper(current_scene_cell);
			block_t right_block = lower(current_scene_cell);

			// Height in blocks determining where ground begins
			block_c ground_height = LVL1_HMAP[world_cell_x];
			block_c current_height = world_cell_y;

			// y coordinate of current block
			pixel_c pos_y = block_to_pixel(j) + 1;

			// Check for tree
			coord pos = { x: pos_x2, y: pos_y };
			if (left_block == _tree || right_block == _tree) {
				draw_tree_normal(&pos);
			}

//			illumination = compute_illumination(scene_cell_x, scene_cell_y);

			// Render left and right blocks
			if (!skip_left) {
				render_block(left_block, pos_x1, pos_y, illumination, ground_height, current_height);
			}

			if (!skip_right) {
				render_block(right_block, pos_x2, pos_y, illumination, ground_height, current_height);
			}

		}

		// Each cell in x-direction in SCENE = move 2 blocks on screen
		pos_x1 += 8;
		pos_x2 += 8;
	}

}





void free_destroyable(destroyable *destroyable) {
	free_block(destroyable->block);
	free(destroyable);
}

void free_bg_material(bg_material *mat) {
	free_block(mat->block);
	free(mat);
}

/* Frees the memory of a block pointer */
void free_block(block *block) {
	free(block);
}

