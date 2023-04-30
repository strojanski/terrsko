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
#include "material_classes.h"
#include "ugui.h"


void dig_down(pixel_position* pos) {

	// Move 1 pixel down first
	pixel_c y = pos->y + 1;

	cell_c wx = pixel_to_cell_x(pos->x);
	cell_c wy = pixel_to_cell_y(y);

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
	pixel_c x = world_pixel_to_world_pixel_x_no_band_param(pos->x, -(GUYSKO_IMG_X/2 + 1));
	pixel_c y = pos->y;

	cell_c wx = pixel_to_cell_x(x);
	cell_c wy = pixel_to_cell_y(y);

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

void dig_up(pixel_position* pos) {

	// Move 1 + guysko width left first
	pixel_c x = pos->x;
	pixel_c y = world_pixel_to_world_pixel_y_no_band_param(pos->y, -(GUYSKO_IMG_Y + 1));

	cell_c wx = pixel_to_cell_x(x);
	cell_c wy = pixel_to_cell_y(y);

	uint8_t hole_size = (GUYSKO_IMG_X / BLOCK_WIDTH);

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
		wx--;

	}

}


void dig_right(pixel_position* pos) {

	// Move 1 px to the right
	pixel_c x = world_pixel_to_world_pixel_x_no_band_param(pos->x, 1);
	pixel_c y = pos->y;

	cell_c wx = pixel_to_cell_x(x);
	cell_c wy = pixel_to_cell_y(y);

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

void place_block(pixel_position* pos, block_t material, block_c offset_x, block_c offset_y) {
	// Use buttons to determine position, use OK button to place
	// Get position, place material in world, when clicking a button remove it and move it one up/down/left/right
	pixel_c x = world_pixel_to_world_pixel_x_no_band_param(pos->x, 1);	// start one block right to the player
	pixel_c y = pos->y;

	// Place material in 2 vertically stacked cells
	for (int i = 0; i < 2; i++) {

		cell_c wx = pixel_to_cell_x(x + block_to_pixel(offset_x));
		cell_c wy = pixel_to_cell_y(y + block_to_pixel(offset_y));

		camera_x_block = cell_x_to_block_left(wx);
		camera_y_block = cell_y_to_block(wy);
		update_camera_center(camera_x_block, camera_y_block);

		WORLD[wy][wx] = build_cell(material, material);


		y = world_pixel_to_world_pixel_y_no_band_param(y, -BLOCK_WIDTH);

	}
}

void cycle_building_material() {
	// Take next item from the "destructibles"
	for (int i = 0; i < sizeof(solid_materials); i++) {
		if (solid_materials[i] == building_material) {
			if (i == sizeof(solid_materials) - 1) {
				building_material = solid_materials[0];
			} else {
				building_material = solid_materials[i+1];
			}
			return;
		}
	}
}

void overwrite_old_string(pixel_c start_x, pixel_c start_y) {
	_HW_FillFrame_(start_x, start_y, start_x + 100, start_y + 15, C_BLACK);
}


void display_material_name() {
	char* mat = get_material_name(building_material);

	overwrite_old_string(235, 20);
	UG_PutString(240, 20, mat);
}


