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

velocity* set_velocity(guysko* player, float x, float y) {
	velocity* vel = malloc(sizeof(velocity));

	vel->x = x;
	vel->y = y;

	return vel;
}

void update_guysko_moving (guysko* player) {

	/*
	 * At this point it is crucial to check if "guysko" has anything
	 * under its feet.
	 * 1.) It can not start moving left or right or jump if it has
	 * nothing underneath. If there is nothing underneath:
	 * 		a) Update velocity in y direction
	 *
	 * 2.) It can go left right or jump if it has a solid block underneath
	 * its feet
	 *
	 * For now it will be saved in bool stable just for testing reasons
	 */

	bool stable = true;

	if (move_right && stable) {
		set_velocity(player, 11, player->vel->y);
		action_reset(MOVE_RIGHT_INDEX);
	} else if (move_left && stable) {
		set_velocity(player, -1, player->vel->y);
		action_reset(MOVE_LEFT_INDEX);
	}

	if (move_up && stable) {
		set_velocity(player, player->vel->x, 3);
		action_reset(MOVE_UP_INDEX);
	} else if (move_down && stable) {
//		set_velocity(player, player->vel->x, -1);
//		action_reset(MOVE_DOWN_INDEX);
	}
}

void update_guysko_position (guysko* player, uint8_t FPS) {
		uint16_t frame_rate = 1000 / FPS;

		player->x_remainder += 1 / (frame_rate / player->vel->x);
		player->y_remainder += 1 / (frame_rate / player->vel->y);

		uint8_t add_x = 0;
		if (player->x_remainder >= 1.0) {
			add_x = 1;
			player->x_remainder -= 1.0;
		}
		uint8_t add_y = 0;
		if (player->y_remainder >= 1.0) {
			add_y = 1;
			player->y_remainder -= 1.0;
		}

		// TODO: dodaj +- glede na smer
		uint8_t diff_x = 1 / (frame_rate / player->vel->x) + add_x;
		uint8_t diff_y = 1 / (frame_rate / player->vel->y) + add_y;


}

void draw_guysko (guysko* player) {
	int index = 0;
	uint16_t draw_startPoint_x = player->position->x - GUYSKO_IMG_X;
	uint8_t draw_startPoint_y = player->position->y - GUYSKO_IMG_Y;

	for (int i = 0; i < GUYSKO_IMG_SIZE; i += 2) {
			uint8_t offset_x = index % GUYSKO_IMG_X;
			uint8_t offset_y = index / GUYSKO_IMG_X;
			index++;
			UG_DrawPixel(draw_startPoint_x + offset_x, draw_startPoint_y + offset_y, (guysko_right_pos2[i] * 256 + guysko_right_pos2[i + 1]));
	}
}

// function "new_guysko" initializes guysko with its properties
guysko* new_guysko(uint8_t life_points, bool direction, coord* position) {
	guysko* player = malloc(sizeof(guysko));

	player->life_points = life_points;

	player->direction 	= direction;

	player->position 		= malloc(sizeof(position));
	player->position->x = position->x;
	player->position->y = position->y;

	player->vel 				= malloc(sizeof(velocity));
	player->vel->x 			= 0;
	player->vel->y 			= 0;
	return player;
}
