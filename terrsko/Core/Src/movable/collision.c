/*
 * collision.c
 *
 *  Created on: 26 Apr 2023
 *      Author: gabrijel
 */


#include "scene.h"
#include "guysko.h"
#include "collision.h"
#include "material_classes.h"


/*
 * 1. Get blocks surrounding guysko in each direction
 * 2. For each block in range [movable_width]:
 * 			-> check collision
 * 			-> return true/false
 * **/

block_pos get_colliding_block_down(pixel_position* pos) {
	// Move a pixel in direction, map to block
	pixel_c cx = pos->x;
	pixel_c cy = pos->y;

	// Move down
	cy += 1;

	block_pos result_block_c = {
			x: pixel_to_block(cx),
			y: pixel_to_block(cy)
	};

	return result_block_c;
}

block_pos get_colliding_block_up(pixel_position* pos, pixel_c movable_height) {
	// Move a pixel in direction, map to block
	pixel_c cx = pos->x;
	pixel_c cy = world_pixel_to_world_pixel_y_no_band_param(pos->y, -(movable_height + 1));

	block_pos result_block_c = {
			x: pixel_to_block(cx),
			y: pixel_to_block(cy)
	};

	return result_block_c;
}

block_pos get_colliding_block_left(pixel_position* pos, pixel_c movable_width) {
	// Move a pixel in direction, map to block
	pixel_c cx = world_pixel_to_world_pixel_x_no_band_param(pos->x, (-1) * (movable_width));
	pixel_c cy = pos->y;

	block_pos result_block_c = {
			x: pixel_to_block(cx),
			y: pixel_to_block(cy)
	};

	return result_block_c;
}

block_pos get_colliding_block_right(pixel_position* pos) {
	// Move a pixel in direction, map to block
	pixel_c cx = world_pixel_to_world_pixel_x_no_band_param(pos->x, 1);
	pixel_c cy = pos->y;

	block_pos result_block_c = {
			x: pixel_to_block(cx),
			y: pixel_to_block(cy)
	};

	return result_block_c;
}


bool collision(uint8_t property, uint8_t direction, pixel_position* pos, uint8_t movable_width, uint8_t movable_height) {
	// Iterate over blocks and check collision
		block_pos block_position;

		// Convert movable width from pixels to blocks
		movable_width = pixel_to_block(movable_width);
		movable_height = pixel_to_block(movable_height);

		uint8_t offset = 2;

		switch (direction) {
			case _up:
				block_position = get_colliding_block_up(pos, movable_height);
				offset = movable_width;
				break;
			case _right:
				block_position = get_colliding_block_right(pos);
				offset = movable_height;
				break;
			case _down:
				block_position = get_colliding_block_down(pos);
				offset = movable_width;
				break;
			case _left:
				block_position = get_colliding_block_left(pos, movable_width);
				offset = movable_height;
				break;
		}


		for (int i = 0; i < offset; i++) {

			cell_t cell = WORLD[block_to_cell_y(block_position.y)][block_to_cell_x(block_position.x)];
			block_t material;

			if (i % 2 == 0) {
				material = upper(cell);
			} else {
				material = lower(cell);
			}

			switch (property) {
				case _destructible:
					if (is_destructible(material)) {
						return true;
					}
					break;

				case _solid:
					if (is_solid(material)) {
						return true;
					}
					break;

				case _harmful:
					if (is_harmuful(material)) {
						return true;
					}
					break;
			}

			switch (direction) {
				case _up:
					block_position.x -= 1;
					break;
				case _right:
					block_position.y -= 1;
					break;
				case _down:
					block_position.x -= 1;
					break;
				case _left:
					block_position.y -= 1;
					break;
			}
		}

		return false;
}

