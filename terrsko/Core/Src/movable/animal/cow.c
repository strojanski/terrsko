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

	cow* new_cow (life_points* lp, velocity* vel, pixel_position* pos, move* mov) {
		cow* krava = (cow*)malloc(sizeof(cow));

		krava->state = 0;
		krava->lp		= (life_points*)malloc(sizeof(life_points));
		krava->lp		= lp;
		krava->vel	= (velocity*)malloc(sizeof(velocity));
		krava->vel	= vel;
		krava->pos	= (pixel_position*)malloc(sizeof(pixel_position));
		krava->pos  = pos;
		krava->mov 	= (move*) malloc(sizeof(pixel_position));
		krava->mov 	= mov;

		krava->next = (cow*)malloc(sizeof(cow));
		krava->next = NULL;
		krava->prev = (cow*)malloc(sizeof(cow));
		krava->prev = NULL;

		return krava;
	}
