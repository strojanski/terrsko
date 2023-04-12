/*
 * render.c
 *
 *  Created on: 12 Apr 2023
 *      Author: gabrijel
 */




#include "render.h"

void overdraw_background (uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
	uint16_t a = MIN(x1, x2);
	uint16_t b = MAX(x1, x2);
	uint16_t c = MIN(y1, y2);
	uint16_t d = MAX(y1, y2);

	_HW_FillFrame_(a, c, b, d, 0x0);

//	// moving right --------------------
//	// figure out the exact pixel of drawing of drawing in previous frame:
//	uint16_t prev_draw_startPoint_x = draw_startPoint_x - x_diff;
//	uint16_t prev_draw_startPoint_y = draw_startPoint_y - y_diff;
//
//	// postion of which block is in the starting pixel:
//	uint16_t right_block_x = prev_draw_startPoint_x / BLOCK_WIDTH;
//	uint16_t right_block_y = prev_draw_startPoint_y / BLOCK_WIDTH;
//
//	// iterate through all the changed in x axis
//
//	for (uint8_t i = 0; i < x_diff; i++) {
//		// actual block:
//		short starting_point_x = prev_draw_startPoint_x + i;
//		short starting_point_y = prev_draw_startPoint_y - 4;
//		if (starting_point_y < 0) starting_point_y = 0;
//
//		short index_x = starting_point_x / 8;
//		short remainder = starting_point_x % 8;
//		short prev_block = (SCENE[right_block_y][index_x] & 0xF0) >> 4;
//		if (remainder >= 4) prev_block = SCENE[right_block_y][index_x] & 0x0F;
//
//		// iterate through all the changed in y axis
//		for (uint16_t j = 0; j < GUYSKO_IMG_Y + 4; j++) {
//			short block = (SCENE[right_block_y + j][index_x] & 0xF0) >> 4;
//			if (remainder >= 4) block = SCENE[right_block_y + j][index_x] & 0x0F;
//
//			if (block != prev_block) {
//				UG_DrawFrame(starting_point_x, starting_point_y, starting_point_x + 1, starting_point_y + j , C_DIRT[0]);
//				starting_point_y = starting_point_y + j;
//				prev_block = block;
//			}
//			if (j == GUYSKO_IMG_Y - 1 || starting_point_y + j >= 240) {
//				UG_DrawFrame(starting_point_x, starting_point_y, starting_point_x + 1, starting_point_y + j , C_DIRT[0]);
//				break;
//			}
//		}
//	}


	// ---------------------------------
//
//	for (uint16_t i = 0; i < x_diff; i++) {
//		uint16_t prev_draw_x = right_x;
//		uint16_t prev_draw_y = right_y;
//		for (uint16_t j = 0; j < GUYSKO_IMG_Y; j++) {
//			uint8_t block = WORLD[right_block_x / 2][right_y + j / BLOCK_WIDTH] & 0x0F;
//			if (right_block_x % 2 == 0) block = (WORLD[right_block_x / 2 + 1][right_block_y + j] & 0xF0) >> 4;
//			if (prev_block != block) {
//				UG_DrawFrame(prev_draw_x, prev_draw_y, prev_draw_x + 1, prev_draw_y + j, C_DIRT[0]);
//				prev_draw_y = right_y + j;
//			}
//			prev_block = block;
//		}
//	}
}
