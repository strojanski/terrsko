/*
 * guysko.c
 *
 *  Created on: 21 Feb 2023
 *      Author: gabrijel
 */




#include <stdlib.h>

#include "ugui.h"
#include "models.h"
#include "guysko.h"


//void update_guysko_acceleration(guysko* player) {
//		/*
//	 * At this point it is crucial to check if "guysko" has anything
//	 * under its feet.
//	 * 1.) It can not start moving left or right or jump if it has
//	 * nothing underneath. If there is nothing underneath:
//	 * 		a) Update acceleration in y direction
//	 *
//	 * 2.) It can go left, right or jump if it has a solid block underneath
//	 * its feet
//	 */
//		/*
//	 * For now setting will be stable meaning that guysko has solid
//	 * block underneath.
//	 */
//
//	/*
//	 * 1.) update acceleration from previous frames
//	 * 2.) add acceleration based on user input
//	 */
//
//	player->acc->y = player->acc->y - GRAVITY;
//	bool stable = true;
//
//	if (stable) { // if it guysko stands on solid ground
//
//	} else {
//
//	}
//
//	if (stable) {
//		if (move_right) {
//			set_acceleration(player, player->acc->x + GUYSKO_WALK_ACCELERATION, player->acc->y);
//			set_acceleration(player, player->acc->y);
//			action_reset(MOVE_RIGHT_INDEX);
//		} else if (move_left) {
//			set_velocity(player, -1, player->vel->y);
//			action_reset(MOVE_LEFT_INDEX);
//		}
//
//	} else {
//
//	}
//
//	if (move_up) {
//		set_velocity(player, player->vel->x, 3);
//		action_reset(MOVE_UP_INDEX);
//	} else if (move_down) {
////		set_velocity(player, player->vel->x, -1);
////		action_reset(MOVE_DOWN_INDEX);
//	}
//}

//
//void update_guysko_position (guysko* player, uint8_t FPS) {
//		uint16_t frame_rate = 1000 / FPS;
//
//		player->x_remainder += 1 / (frame_rate / player->vel->x);
//		player->y_remainder += 1 / (frame_rate / player->vel->y);
//
//		uint8_t add_x = 0;
//		if (player->x_remainder >= 1.0) {
//			add_x = 1;
//			player->x_remainder -= 1.0;
//		}
//		uint8_t add_y = 0;
//		if (player->y_remainder >= 1.0) {
//			add_y = 1;
//			player->y_remainder -= 1.0;
//		}
//
//		// TODO: dodaj +- glede na smer
//		uint8_t diff_x = 1 / (frame_rate / player->vel->x) + add_x;
//		uint8_t diff_y = 1 / (frame_rate / player->vel->y) + add_y;
//
//
//}

void draw_guysko (guysko* player) {
	int index = 0;
	uint16_t draw_startPoint_x = player->pos->x - (GUYSKO_IMG_X / 2);
	uint8_t draw_startPoint_y = player->pos->y - GUYSKO_IMG_Y;

	for (int i = 0; i < GUYSKO_IMG_SIZE / 2; i += 1) {
		uint8_t offset_x = index % (GUYSKO_IMG_X / 2);
		uint8_t offset_y = index / (GUYSKO_IMG_X / 2);
		index++;
		// TODO: BEAUTIFY THIS SECTION OF CODE
		if (player->vel->x >= 0) {
			if (player->state == 0) {
				int frst_nibble =	(guysko_r_0[i] & 0b11110000) >> 4;
				int scnd_nibble =	(guysko_r_0[i] & 0b00001111) >> 0;
				UG_DrawPixel(draw_startPoint_x + 2 * offset_x, draw_startPoint_y + offset_y, GUYSKO_R_0[frst_nibble]);
				UG_DrawPixel(draw_startPoint_x + 2 * offset_x + 1, draw_startPoint_y + offset_y, GUYSKO_R_0[scnd_nibble]);
			} else if (player->state == 1) {
				int frst_nibble =	(guysko_r_1[i] & 0b11110000) >> 4;
				int scnd_nibble =	(guysko_r_1[i] & 0b00001111) >> 0;
				UG_DrawPixel(draw_startPoint_x + 2 * offset_x, draw_startPoint_y + offset_y, GUYSKO_R_1[frst_nibble]);
				UG_DrawPixel(draw_startPoint_x + 2 * offset_x + 1, draw_startPoint_y + offset_y, GUYSKO_R_1[scnd_nibble]);
			} else if (player->state == 2) {
				int frst_nibble =	(guysko_r_2[i] & 0b11110000) >> 4;
				int scnd_nibble =	(guysko_r_2[i] & 0b00001111) >> 0;
				UG_DrawPixel(draw_startPoint_x + 2 * offset_x, draw_startPoint_y + offset_y, GUYSKO_R_2[frst_nibble]);
				UG_DrawPixel(draw_startPoint_x + 2 * offset_x + 1, draw_startPoint_y + offset_y, GUYSKO_R_2[scnd_nibble]);
			}
		} else {
			if (player->state == 0) {
				int frst_nibble =	(guysko_l_0[i] & 0b11110000) >> 4;
				int scnd_nibble =	(guysko_l_0[i] & 0b00001111) >> 0;
				UG_DrawPixel(draw_startPoint_x + 2 * offset_x, draw_startPoint_y + offset_y, GUYSKO_L_0[frst_nibble]);
				UG_DrawPixel(draw_startPoint_x + 2 * offset_x + 1, draw_startPoint_y + offset_y, GUYSKO_L_0[scnd_nibble]);
			} else if (player->state == 1) {
				int frst_nibble =	(guysko_l_1[i] & 0b11110000) >> 4;
				int scnd_nibble =	(guysko_l_1[i] & 0b00001111) >> 0;
				UG_DrawPixel(draw_startPoint_x + 2 * offset_x, draw_startPoint_y + offset_y, GUYSKO_L_1[frst_nibble]);
				UG_DrawPixel(draw_startPoint_x + 2 * offset_x + 1, draw_startPoint_y + offset_y, GUYSKO_L_1[scnd_nibble]);
			} else if (player->state == 2) {
				int frst_nibble =	(guysko_l_2[i] & 0b11110000) >> 4;
				int scnd_nibble =	(guysko_l_2[i] & 0b00001111) >> 0;
				UG_DrawPixel(draw_startPoint_x + 2 * offset_x, draw_startPoint_y + offset_y, GUYSKO_L_2[frst_nibble]);
				UG_DrawPixel(draw_startPoint_x + 2 * offset_x + 1, draw_startPoint_y + offset_y, GUYSKO_L_2[scnd_nibble]);
			}
		}
	}
	player->state++;
	if (player->state >= 3) player->state = player->state % 3;
}


/* function "new_guysko" initializes guysko with its properties
 * @param 							life_points denotes guysko's health points
 * @param direction 		TRUE RIGHT, FLASE LEFT
 * @param acceleration	guysko's acceleration
 * @param velocity 			guysko's velocity
 * @param move					difference of position guysko will make inbetween frame
 * @param position 			guysko's new position
 */
guysko* new_guysko(life_points* lp, uint8_t state, acceleration*acc, velocity* vel, move* mov, position* pos) {
	guysko* player = malloc(sizeof(guysko));

	player->lp							= malloc(sizeof(life_points));
	player->lp->life_points = lp->life_points;

	player->state 			= 0;

	player->acc			 		= malloc(sizeof(acceleration));
	player->acc					= acc;

	player->vel 				= malloc(sizeof(velocity));
	player->vel					= vel;

	player->mov 				= malloc(sizeof(move));
	player->mov 				= mov;

	player->pos 				= malloc(sizeof(position));
	player->pos		 			= pos;


	return player;
}
