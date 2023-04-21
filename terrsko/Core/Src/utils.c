/*
 * util.c
 *
 *  Created on: 14 Feb 2023
 *      Author: sebas
 */

#include <stdint.h>
#include <scene.h>

// Returns upper 4 bits of uint8_t
uint8_t upper(uint8_t block) {
	return (block & 0xF0) >> 4;
}

uint8_t lower(uint8_t block) {
	return block & 0x0F;
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


uint16_t pixel_to_block(uint16_t pixel_coord) {
	return pixel_coord / BLOCK_WIDTH;
}

uint16_t block_to_pixel(uint16_t block_coord) {
	return BLOCK_WIDTH + block_coord * BLOCK_WIDTH - 1;
}

// cell represents the
uint16_t cell_x_to_block_left(uint16_t cell_coord) {
	return 2 * cell_coord;
}

uint16_t cell_x_to_block_right(uint16_t cell_coord) {
	return 2 * cell_coord + 1;
}

uint16_t cell_y_to_block(uint16_t cell_coord) {
	return cell_coord;
}

uint16_t block_to_cell_x(uint16_t block_coord) {
	return block_coord / 2;
}

uint16_t block_to_cell_y(uint16_t block_coord) {
	return block_coord;
}

uint16_t pixel_to_cell_x(uint16_t pixel_coord) {
	return block_to_cell_x(
			pixel_to_block(pixel_coord));
}

uint16_t pixel_to_cell_y(uint16_t pixel_coord) {
	return pixel_to_block(pixel_coord);
}

uint16_t cell_x_to_pixel(uint16_t cell_coord) {
	return CELL_WIDTH + cell_coord * CELL_WIDTH - 1;
}

uint16_t cell_y_to_pixel(uint16_t cell_coord) {
	return block_to_pixel(cell_coord);
}
