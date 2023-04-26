/*
 * util.c
 *
 *  Created on: 14 Feb 2023
 *      Author: sebas
 */

#include <stdint.h>
#include <scene.h>
#include "structures.h"

/* Returns upper 4 bits of cell_t
 * 		@param block_t block
 * **/
block_t upper(block_t block) {
	return (block & 0xF0) >> 4;
}

/* Returns lower 4 bits of cell_t */
block_t lower(block_t block) {
	return block & 0x0F;
}

void breakpoint () {
	uint8_t ustau_se = 0;
	if (ustau_se > 5) {
		ustau_se--;
	}
	ustau_se++;
	return;
}
/*
 * PIXEL COORDINATES:
 * 		- global : Start with world, 1 block = 4x4 pixels
 * 		- local : Actual pixels on screen
 *
 * CELL COORDINATES:
 * 		- corresponds to 1 cell in WORLD, which holds values FOR 2 BLOCKS IN X DIRECTION (uint8_t)
 *
 * BLOCK COORDINATES:
 * 		- correspond to 4x4 pixel areas, into which the world is divided into

  ALL ARRAY INDICES (BLOCKS, CELLS, PIXELS) BEGIN WITH 0

 */


block_c pixel_to_block(pixel_c pixel_coord) {
	return pixel_coord / BLOCK_WIDTH;
}

pixel_c block_to_pixel(block_c block_coord) {
	return BLOCK_WIDTH + block_coord * BLOCK_WIDTH - 1;
}

// cell represents the
block_c cell_x_to_block_left(cell_c cell_coord) {
	return 2 * cell_coord;
}

block_c cell_x_to_block_right(cell_c cell_coord) {
	return 2 * cell_coord + 1;
}

block_c cell_y_to_block(cell_c cell_coord) {
	return cell_coord;
}

cell_c block_to_cell_x(block_c block_coord) {
	return block_coord / 2;
}

cell_c block_to_cell_y(block_c block_coord) {
	return block_coord;
}

cell_c pixel_to_cell_x(pixel_c pixel_coord) {
	return block_to_cell_x(
			pixel_to_block(pixel_coord));
}

cell_c pixel_to_cell_y(pixel_c pixel_coord) {
	return pixel_to_block(pixel_coord);
}

pixel_c cell_x_to_pixel(cell_c cell_coord) {
	return CELL_WIDTH + cell_coord * CELL_WIDTH - 1;
}

pixel_c cell_y_to_pixel(cell_c cell_coord) {
	return block_to_pixel(cell_coord);
}
