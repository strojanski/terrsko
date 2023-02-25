/*
 * guysko.c
 *
 *  Created on: 21 Feb 2023
 *      Author: gabrijel
 */




#include <stdlib.h>

#include "ugui.h"
#include "guysko.h"
#include "models.h"
#include "guysko_img.h"



velocity* set_velocity(short x, short y){
	velocity* vel = malloc(sizeof(velocity));

	vel->x = x;
	vel->y = y;

	return vel;
}

// function "new_guysko" initializes guysko with its properties
guysko* new_guysko(uint8_t life_points, bool direction, coord* position) {
	guysko* player = malloc(sizeof(guysko));

	player->life_points = life_points;

	player->frame 			= 0;
	player->direction 	= direction;

	player->position 		= malloc(sizeof(position));
	player->position->x = position->x;
	player->position->y = position->y;

	player->vel 				= malloc(sizeof(velocity));
	player->vel->x 			= 0;
	player->vel->y 			= 0;
	return player;
}

void draw_guysko (guysko* player) {
	int index = 0;
	if (player->frame == 0) {
		for (int i = 0; i < GUYSKO_IMG_SIZE; i += 2) {
				uint8_t draw_startPoint_x = player->position->x - 12;
				uint8_t draw_startPoint_y = player->position->y - 23;
				int place_x = index % 12;
				int place_y = index / 12;
				index++;
				UG_DrawPixel(place_x + draw_startPoint_x, place_y + draw_startPoint_y, (guysko_right_pos0[i] * 256 + guysko_right_pos0[i + 1]));
			}
	}
	player->frame++;
	player->frame %= 3;
}
