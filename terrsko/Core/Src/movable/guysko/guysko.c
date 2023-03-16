/*
 * guysko.c
 *
 *  Created on: 21 Feb 2023
 *      Author: gabrijel
 */




#include <stdlib.h>

#include "guysko.h"

void update_guysko_position (guysko* player) {
	set_postition(player->pos, player->pos->x + player->mov->x, player->pos->y - player->mov->y);
}

void update_guysko_move (guysko* player, int FPS) {
	float mov_x = ((float)1 / ((float) (1000 / FPS) / (float)player->vel->x)) + player->mov->x_remainder;
	float mov_y = ((float)1 / ((float) (1000 / FPS) / (float)player->vel->y)) + player->mov->y_remainder;

	short pix_move_x = (short) mov_x % 100;
	short pix_move_y = (short) mov_y % 100;

	player->mov->x_remainder = mov_x - (float)pix_move_x;
	player->mov->y_remainder = mov_y - (float)pix_move_y;

	set_move(player->mov, pix_move_x, pix_move_y);
}

void update_guysko_velocity(guysko* player) {
	// TODO:
	//	if(pos_y > 220 /*Preverba ali se zaleti v solid*/) {
	//		set_velocity(vel, vel->x, 0);
	//	} else { ..
	//			..
	//	}


	// x axis
	/*
	 * TODO: it can move right and left if and only if there is a solid
	 * block underneath
	 */
	if (move_right) {
		if (player->vel->x < 0) set_velocity(player->vel, 0 + GUYSKO_WALK_VEL_INC, player->vel->y);
		else set_velocity(player->vel, player->vel->x + GUYSKO_WALK_VEL_INC, player->vel->y);
	// MAX VELOCITY IN X DIRECTION
		if (player->vel->x > GUYSKO_MAX_RIGHT_VELOCITY) set_velocity(player->vel, GUYSKO_MAX_RIGHT_VELOCITY, player->vel->y);
		action_reset(MOVE_RIGHT_INDEX);
	} else if (move_left) {
		if (player->vel->x > 0) set_velocity(player->vel, 0 - GUYSKO_WALK_VEL_INC, player->vel->y);
		else set_velocity(player->vel, player->vel->x - GUYSKO_WALK_VEL_INC, player->vel->y);
	// MAX VELOCITY IN X DIRECTION
		if (player->vel->x < GUYSKO_MAX_LEFT_VELOCITY) set_velocity(player->vel, GUYSKO_MAX_LEFT_VELOCITY, player->vel->y);
		action_reset(MOVE_LEFT_INDEX);
	} else {
		set_velocity(player->vel, 0, player->vel->y);
	}

	// y axis
	set_velocity(player->vel, player->vel->x, player->vel->y + player->acc->y);
	// MAX VELOCITY IN Y DIRECTION
	if (player->vel->y < GUYSKO_MAX_DOWN_VELOCITY) {
		set_velocity(player->vel, player->vel->x, GUYSKO_MAX_DOWN_VELOCITY);
	} else if (player->vel->y > GUYSKO_MAX_UP_VELOCITY) {
		set_velocity(player->vel, player->vel->x, GUYSKO_MAX_UP_VELOCITY);
	}

	// TODO: DIFFERENCE OF PREVIOUS AND NEW VELOCITY: FOR DAMAGE OF HIGH FALL
}

void update_guysko_acceleration (guysko* player) {
	if (true /*pos_y >= 220 ce guysko nima nič "solid" pod seboj*/) {
		if (move_up) {
			set_acceleration(player->acc, player->acc->x, player->acc->y + GUYSKO_JUMP_ACCELERATION + GRAVITY);
			action_reset(MOVE_UP_INDEX);
		} else {
			set_acceleration(player->acc, player->acc->x, GRAVITY);
		}
	} else {
			set_acceleration(player->acc, player->acc->x, player->acc->y + GRAVITY);
	}

	// MAX ACCLERATION IN Y DIRECTION
	if (player->acc->y < GUYSKO_MAX_DOWN_ACC) {
		set_acceleration(player->acc, player->acc->x, GUYSKO_MAX_DOWN_ACC);
	} else if (player->acc->y > GUYSKO_MAX_UP_ACC) {
		set_acceleration(player->acc, player->acc->x, GUYSKO_MAX_UP_ACC);
	}
}


void draw_guysko (guysko* player) {
	int index = 0;
	uint16_t draw_startPoint_x = player->pos->x - (GUYSKO_IMG_X / 2);
	uint8_t draw_startPoint_y = player->pos->y - GUYSKO_IMG_Y;

	for (int i = 0; i < GUYSKO_IMG_SIZE / 2; i += 1) {
		uint8_t offset_x = index % (GUYSKO_IMG_X / 2);
		uint8_t offset_y = index / (GUYSKO_IMG_X / 2);
		index++;
		// TODO: BEAUTIFY THIS SECTION OF CODE
		int frst_nibble =	(guysko_r_0[i] & 0b11110000) >> 4;
		int scnd_nibble =	(guysko_r_0[i] & 0b00001111) >> 0;
		UG_DrawPixel(draw_startPoint_x + 2 * offset_x, draw_startPoint_y + offset_y, GUYSKO_R_0[frst_nibble]);
		UG_DrawPixel(draw_startPoint_x + 2 * offset_x + 1, draw_startPoint_y + offset_y, GUYSKO_R_0[scnd_nibble]);
//		if (player->vel->x >= 0) {
//			if (player->state == 0) {
//			} else if (player->state == 1) {
//				int frst_nibble =	(guysko_r_1[i] & 0b11110000) >> 4;
//				int scnd_nibble =	(guysko_r_1[i] & 0b00001111) >> 0;
//				UG_DrawPixel(draw_startPoint_x + 2 * offset_x, draw_startPoint_y + offset_y, GUYSKO_R_1[frst_nibble]);
//				UG_DrawPixel(draw_startPoint_x + 2 * offset_x + 1, draw_startPoint_y + offset_y, GUYSKO_R_1[scnd_nibble]);
//			} else if (player->state == 2) {
//				int frst_nibble =	(guysko_r_2[i] & 0b11110000) >> 4;
//				int scnd_nibble =	(guysko_r_2[i] & 0b00001111) >> 0;
//				UG_DrawPixel(draw_startPoint_x + 2 * offset_x, draw_startPoint_y + offset_y, GUYSKO_R_2[frst_nibble]);
//				UG_DrawPixel(draw_startPoint_x + 2 * offset_x + 1, draw_startPoint_y + offset_y, GUYSKO_R_2[scnd_nibble]);
//			}
//		} else {
//			if (player->state == 0) {
//				int frst_nibble =	(guysko_l_0[i] & 0b11110000) >> 4;
//				int scnd_nibble =	(guysko_l_0[i] & 0b00001111) >> 0;
//				UG_DrawPixel(draw_startPoint_x + 2 * offset_x, draw_startPoint_y + offset_y, GUYSKO_L_0[frst_nibble]);
//				UG_DrawPixel(draw_startPoint_x + 2 * offset_x + 1, draw_startPoint_y + offset_y, GUYSKO_L_0[scnd_nibble]);
//			} else if (player->state == 1) {
//				int frst_nibble =	(guysko_l_1[i] & 0b11110000) >> 4;
//				int scnd_nibble =	(guysko_l_1[i] & 0b00001111) >> 0;
//				UG_DrawPixel(draw_startPoint_x + 2 * offset_x, draw_startPoint_y + offset_y, GUYSKO_L_1[frst_nibble]);
//				UG_DrawPixel(draw_startPoint_x + 2 * offset_x + 1, draw_startPoint_y + offset_y, GUYSKO_L_1[scnd_nibble]);
//			} else if (player->state == 2) {
//				int frst_nibble =	(guysko_l_2[i] & 0b11110000) >> 4;
//				int scnd_nibble =	(guysko_l_2[i] & 0b00001111) >> 0;
//				UG_DrawPixel(draw_startPoint_x + 2 * offset_x, draw_startPoint_y + offset_y, GUYSKO_L_2[frst_nibble]);
//				UG_DrawPixel(draw_startPoint_x + 2 * offset_x + 1, draw_startPoint_y + offset_y, GUYSKO_L_2[scnd_nibble]);
//			}
//		}
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

	player->lp					= malloc(sizeof(life_points));
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
