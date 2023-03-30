/*
 * cow.c
 *
 *  Created on: 16 Mar 2023
 *      Author: gabrijel
 */




#include "cow.h"

uint16_t cow_colors_0[] = {
		0x4228, 0xffff, 0x0, 0xe4b4, 0xfedb, 0xdbaf
};
uint8_t cow_r_0[] = {
	34, 34, 34, 34, 34, 34, 0, 0, 16, 1, 0, 0,
	34, 34, 34, 34, 34, 34, 1, 17, 17, 17, 17, 16,
	34, 34, 34, 34, 34, 34, 0, 1, 33, 17, 16, 0,
	32, 17, 17, 17, 17, 17, 17, 1, 33, 18, 16, 34,
	33, 17, 17, 17, 17, 17, 17, 1, 33, 18, 16, 34,
	33, 17, 17, 17, 17, 17, 17, 3, 51, 51, 48, 34,
	34, 17, 33, 33, 33, 17, 32, 67, 35, 50, 52, 2,
	34, 34, 17, 18, 17, 18, 32, 67, 51, 51, 52, 2,
	34, 0, 85, 82, 34, 32, 34, 0, 51, 51, 0, 34,
	34, 0, 37, 34, 34, 0, 34, 34, 0, 0, 34, 34,
	34, 0, 34, 34, 34, 0, 34, 34, 34, 34, 34, 34,
	34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34
};
uint8_t cow_l_0[] = {
	0, 0, 16, 1, 0, 0, 34, 34, 34, 34, 34, 34,
	1, 17, 17, 17, 17, 16, 34, 34, 34, 34, 34, 34,
	0, 1, 17, 18, 16, 0, 34, 34, 34, 34, 34, 34,
	34, 1, 33, 18, 16, 17, 17, 17, 17, 17, 17, 2,
	34, 1, 33, 18, 16, 17, 17, 17, 17, 17, 17, 18,
	34, 3, 51, 51, 48, 17, 17, 17, 17, 17, 17, 18,
	32, 67, 35, 50, 52, 2, 17, 18, 18, 18, 17, 34,
	32, 67, 51, 51, 52, 2, 33, 17, 33, 17, 34, 34,
	34, 0, 51, 51, 0, 34, 2, 34, 37, 85, 0, 34,
	34, 34, 0, 0, 34, 34, 0, 34, 34, 82, 0, 34,
	34, 34, 34, 34, 34, 34, 0, 34, 34, 34, 0, 34,
	34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34
};

void draw_cow(cow* krava) {
	int index = 0;
	uint16_t draw_startPoint_x = krava->pos->x - (COW_IMG_X / 2);
	uint8_t draw_startPoint_y = krava->pos->y - COW_IMG_Y;
	uint8_t offset_x;
	uint8_t offset_y;
	int frst_nibble;
	int scnd_nibble;

	for (int i = 0; i < COW_IMG_SIZE / 2; i += 1) {
		offset_x = index % (COW_IMG_X / 2);
		offset_y = index / (COW_IMG_X / 2);
		index++;
		// TODO: BEAUTIFY THIS SECTION OF CODE
		frst_nibble =	(cow_r_0[i] & 0b11110000) >> 4;
		scnd_nibble =	(cow_r_0[i] & 0b00001111) >> 0;
		UG_DrawPixel(draw_startPoint_x + 2 * offset_x, draw_startPoint_y + offset_y, cow_colors_0[frst_nibble]);
		UG_DrawPixel(draw_startPoint_x + 2 * offset_x + 1, draw_startPoint_y + offset_y, cow_colors_0[scnd_nibble]);
	}
}

cow* new_cow (life_points* lp, velocity* vel, position* pos) {
	cow* krava = (cow*)malloc(sizeof(cow));

	krava->state = 0;
	krava->lp		= (life_points*)malloc(sizeof(life_points));
	krava->lp		= lp;
	krava->vel	= (velocity*)malloc(sizeof(velocity));
	krava->vel	= vel;
	krava->pos	= (position*)malloc(sizeof(position));
	krava->pos  = pos;

	krava->next = (cow*)malloc(sizeof(cow));
	krava->next = NULL;
	krava->prev = (cow*)malloc(sizeof(cow));
	krava->prev = NULL;

	return krava;
}
