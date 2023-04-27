/*
 * interactions.c
 *
 *  Created on: 26 Apr 2023
 *      Author: gabrijel
 */

#include "guysko.h"
#include "scene.h"
#include "structures.h"
#include "utils.h"


void dig_down(pixel_position* pos) {

	// Move 1 pixel down first
	pos->y += 1;

	cell_c wx = pixel_to_cell_x(pos->x);
	cell_c wy = pixel_to_cell_y(pos->y);

	uint8_t hole_size = (GUYSKO_IMG_X / BLOCK_WIDTH) / 2;

	for (int i = 0; i < hole_size; i++) {
		// Determine background
		block_t background = _dirt_bg;
		if (wy < LVL1_HMAP[wx]) {
			background = _sky;
		}

		// Get block under
		cell_t cell = WORLD[wy][wx];

		block_t block_left = upper(cell), block_right = lower(cell);

		// Destroy block
		if (is_destructible(block_left)) {
			block_left = background;
		}
		if (is_destructible(block_right)) {
			block_right = background;
		}

		WORLD[wy][wx] = build_cell(block_left, block_right);
		wx--;

	}

}

void dig_left(pixel_position* pos) {

	// Move 1 + guysko width left first
	pos->x = world_pixel_to_world_pixel_x_no_band_param(pos->x, -(GUYSKO_IMG_X/2 + 1));

	cell_c wx = pixel_to_cell_x(pos->x);
	cell_c wy = pixel_to_cell_y(pos->y);

	uint8_t hole_size = (GUYSKO_IMG_Y / BLOCK_WIDTH);

	for (int i = 0; i < hole_size; i++) {
		// Determine background
		block_t background = _dirt_bg;
		if (wy < LVL1_HMAP[wx]) {
			background = _sky;
		}

		// Get block left
		cell_t cell = WORLD[wy][wx];

		block_t block_left = upper(cell), block_right = lower(cell);

		// Destroy block
		if (is_destructible(block_right)) {
			block_right = background;
		}
		if (is_destructible(block_left)) {
			block_left = background;
		}

		WORLD[wy][wx] = build_cell(block_left, block_right);
		wy--;

	}

}

void dig_right(pixel_position* pos) {

	// Move 1 px to the right
	pos->x = world_pixel_to_world_pixel_x_no_band_param(pos->x, 1);

	cell_c wx = pixel_to_cell_x(pos->x);
	cell_c wy = pixel_to_cell_y(pos->y);

	uint8_t hole_size = (GUYSKO_IMG_Y / BLOCK_WIDTH);

	for (int i = 0; i < hole_size; i++) {
		// Determine background
		block_t background = _dirt_bg;
		if (wy < LVL1_HMAP[wx]) {
			background = _sky;
		}

		// Get block right
		cell_t cell = WORLD[wy][wx];

		block_t block_left = upper(cell), block_right = lower(cell);

		// Destroy block
		if (is_destructible(block_right)) {
			block_right = background;
		}
		if (is_destructible(block_left)) {
			block_left = background;
		}

		WORLD[wy][wx] = build_cell(block_left, block_right);
		wy--;

	}

}


