/*
 * guysko.c
 *
 *  Created on: 21 Feb 2023
 *      Author: gabrijel
 */




#include <stdlib.h>

#include "guysko.h"
#include "utils.h"

/*
 * Simpy calculated the movement guysko makes based on his move. But when he goes outside of range of world in
 * x axis, he is teleported on the opposite side of the world. This does not apply for the y axis, where he
 * can be stuck at the botton of the world and of course won't fall down againg from sky.
 */
void update_guysko_position (guysko* player) {
	int new_guysko_pos_x = (player->pos->x + player->mov->x) % (WORLD_WIDTH_BLOCKS * BLOCK_WIDTH);
	int new_guysko_pos_y = (player->pos->y - player->mov->y);
	if (new_guysko_pos_x < 0) {
		new_guysko_pos_x = WORLD_WIDTH_BLOCKS * BLOCK_WIDTH + new_guysko_pos_x;
	}
	if (new_guysko_pos_y < 0) {
//		new_guysko_pos_y = WORLD_HEIGHT * 4 + new_guysko_pos_y;
		new_guysko_pos_y = 0;
	} else if (new_guysko_pos_y > WORLD_HEIGHT_BLOCKS * BLOCK_WIDTH) {
		new_guysko_pos_y = WORLD_HEIGHT_BLOCKS * BLOCK_WIDTH;
	}
	set_postition(player->pos, new_guysko_pos_x, new_guysko_pos_y);
}

/*
 * Because it is posible at slow speed and high FPS for guysko to move less than one pixel in difference of
 * one frame, the traveled distance that was not displayed in previos frame is at this frame taken into calculation
 * with respect to value in "player->mov->x_remainder".
 */
void update_guysko_move (guysko* player, int FPS) {
	float mov_x = ((float)1 / ((float) (1000 / FPS) / (float)player->vel->x)) + player->mov->x_remainder;
	float mov_y = ((float)1 / ((float) (1000 / FPS) / (float)player->vel->y)) + player->mov->y_remainder;

	short pix_move_x = (short) mov_x % 100;
	short pix_move_y = (short) mov_y % 100;

	player->mov->x_remainder = mov_x - (float)pix_move_x;
	player->mov->y_remainder = mov_y - (float)pix_move_y;

	set_move(player->mov, pix_move_x, pix_move_y);
}

/*
 * Guysko's velocity is calculated based on force of gravity and his jump acceleration. When calculating
 * velocity of each direction, button input is taken into consideration.
 */
void update_guysko_velocity(guysko* player) {

	// TODO: preverba ali se je zaletel v solid levo desno gor, dol?
	// TODO: update movement based on that

	uint8_t material_u = get_block(player->pos->x, player->pos->y - GUYSKO_IMG_Y - 1);
	uint8_t material_r = get_block(player->pos->x + 1, player->pos->y);
	uint8_t material_d = get_block(player->pos->x, player->pos->y + 1);
	uint8_t material_l = get_block(player->pos->x - GUYSKO_IMG_X - 1, player->pos->y);

	// down left and down right
	uint8_t mat_d_l = upper(material_d);
	uint8_t mat_d_r = lower(material_d);


	if (move_right) {
		if (player->vel->x < 0) set_velocity(player->vel, 0 + GUYSKO_WALK_VEL_INC, player->vel->y);
		else set_velocity(player->vel, player->vel->x + GUYSKO_WALK_VEL_INC, player->vel->y);
	// MAX VELOCITY IN X DIRECTION
		if (player->vel->x > GUYSKO_MAX_RIGHT_VELOCITY) {
			set_velocity(player->vel, GUYSKO_MAX_RIGHT_VELOCITY, player->vel->y);
		}

		action_reset(MOVE_RIGHT_INDEX);

		// Collision in right direction
		if (isSolid(material_r)) {
			set_velocity(player->vel, 0, player->vel->y);
		}

	} else if (move_left) {
		if (player->vel->x > 0) set_velocity(player->vel, 0 - GUYSKO_WALK_VEL_INC, player->vel->y);
		else set_velocity(player->vel, player->vel->x - GUYSKO_WALK_VEL_INC, player->vel->y);
	// MAX VELOCITY IN X DIRECTION
		if (player->vel->x < GUYSKO_MAX_LEFT_VELOCITY) set_velocity(player->vel, GUYSKO_MAX_LEFT_VELOCITY, player->vel->y);
		action_reset(MOVE_LEFT_INDEX);
		if (isSolid(material_l)) set_velocity(player->vel, 0, player->vel->y);
	} else {
		set_velocity(player->vel, 0, player->vel->y);
	}

	// y axis
	set_velocity(player->vel, player->vel->x, player->vel->y + GRAVITY);
	if (isSolid(mat_d_l) || isSolid(mat_d_r) || player->pos->y / 4 > LVL1_HMAP[player->pos->x / 4]) {
		if (move_up) {
			set_velocity(player->vel, player->vel->x, player->vel->y + GUYSKO_JUMP_ACCELERATION);
			action_reset(MOVE_UP_INDEX);
			if (isSolid(material_u)) set_velocity(player->vel, player->vel->x, 0);
		} else {
			set_velocity(player->vel, player->vel->x, 0);
		}
	}
	// MAX VELOCITY IN Y DIRECTION
	if (player->vel->y < GUYSKO_MAX_DOWN_VELOCITY) {
		set_velocity(player->vel, player->vel->x, GUYSKO_MAX_DOWN_VELOCITY);
	} else if (player->vel->y > GUYSKO_MAX_UP_VELOCITY) {
		set_velocity(player->vel, player->vel->x, GUYSKO_MAX_UP_VELOCITY);
	}
	return;

	// TODO: DIFFERENCE OF PREVIOUS AND NEW VELOCITY: FOR DAMAGE OF HIGH FALL
}


/*
 * Function takes guysko's postion and draws it on screen. The postion of guysko denotes the bottom right
 * corner of guysko's image. The postion of guysko cannot be taken as the postion of guysko on screen,
 * because the screen is only a small part of world. So the starting drawing postion is calculated based on
 * guysko's postion and difference between the START of the left and top corner of the screen and left and
 * top corner of the world.
 */

void draw_guysko (guysko* player) {
	int index = 0;
	uint16_t draw_startPoint_x = player->pos->x - GUYSKO_IMG_X - (camera_x * BLOCK_WIDTH - ((SCENE_WIDTH_BLOCKS / 2) * BLOCK_WIDTH));
	uint16_t draw_startPoint_y = player->pos->y - GUYSKO_IMG_Y - (camera_y * BLOCK_WIDTH - ((SCENE_HEIGHT_BLOCKS / 2) * BLOCK_WIDTH));
	for (int i = 0; i < GUYSKO_IMG_SIZE / 2; i += 1) {
		uint8_t offset_x = index % (GUYSKO_IMG_X / 2);
		uint8_t offset_y = index / (GUYSKO_IMG_X / 2);
		index++;
		// TODO: BEAUTIFY THIS SECTION OF CODE
		int frst_nibble =	(guysko_r_0[i] & 0b11110000) >> 4;
		int scnd_nibble =	(guysko_r_0[i] & 0b00001111) >> 0;
		if (frst_nibble != 0) UG_DrawPixel(draw_startPoint_x + 2 * offset_x, draw_startPoint_y + offset_y, GUYSKO_R_0[frst_nibble]);
		if (scnd_nibble != 0) UG_DrawPixel(draw_startPoint_x + 2 * offset_x + 1, draw_startPoint_y + offset_y, GUYSKO_R_0[scnd_nibble]);
	}
	player->state++;
	if (player->state >= 3) player->state = player->state % 3;
}

void camouflage (guysko* player, uint16_t prev_guysko_x, uint16_t prev_guysko_y) {
	short x_diff = player->pos->x - prev_guysko_x;
	short y_diff = player->pos->y - prev_guysko_y;

	uint16_t prev_draw_startPoint_x = prev_guysko_x - GUYSKO_IMG_X - (camera_x * BLOCK_WIDTH - SCENE_WIDTH_BLOCKS * BLOCK_WIDTH / 2);
	uint16_t prev_draw_startPoint_y = prev_guysko_y - GUYSKO_IMG_Y - (camera_y * BLOCK_WIDTH - SCENE_HEIGHT_BLOCKS * BLOCK_WIDTH / 2);
	uint16_t draw_startPoint_x = player->pos->x - GUYSKO_IMG_X - (camera_x * BLOCK_WIDTH - SCENE_WIDTH_BLOCKS * BLOCK_WIDTH / 2);
	uint16_t draw_startPoint_y = player->pos->y - GUYSKO_IMG_Y - (camera_y * BLOCK_WIDTH - SCENE_HEIGHT_BLOCKS * BLOCK_WIDTH / 2);

	if (x_diff > 0) {
		overdraw_background(draw_startPoint_x, draw_startPoint_y, prev_draw_startPoint_x, prev_draw_startPoint_y + GUYSKO_IMG_Y);
	} else if (x_diff < 0) {
		overdraw_background(draw_startPoint_x + GUYSKO_IMG_X, draw_startPoint_y - 3, prev_draw_startPoint_x + GUYSKO_IMG_X, prev_draw_startPoint_y + GUYSKO_IMG_Y);
	}
	if (y_diff > 0) {
		overdraw_background(draw_startPoint_x, draw_startPoint_y, prev_draw_startPoint_x + GUYSKO_IMG_X, prev_draw_startPoint_y);
	} else if (y_diff < 0) {
		overdraw_background(draw_startPoint_x, draw_startPoint_y + GUYSKO_IMG_Y, prev_draw_startPoint_x + GUYSKO_IMG_X, prev_draw_startPoint_y + GUYSKO_IMG_Y);
	}

}

void refresh_guysko(guysko* player, int FPS) {
	update_guysko_velocity(player);
	update_guysko_move(player, FPS);
	uint16_t prev_guysko_x = player->pos->x;
	uint16_t prev_guysko_y = player->pos->y;
	update_guysko_position(player);

//	camouflage (player, prev_guysko_x, prev_guysko_y);
	draw_guysko(player);

}


/* function "new_guysko" initializes guysko with its properties
 * @param 							life_points denotes guysko's health points
 * @param direction 		TRUE RIGHT, FALSE LEFT
 * @param acceleration	guysko's acceleration
 * @param velocity 			guysko's velocity
 * @param move					difference of position guysko will make inbetween frame
 * @param position 			guysko's new position
 */
guysko* new_guysko() {
	guysko* player = malloc(sizeof(guysko));
	life_points *lp = malloc(sizeof(life_points));
	lp->life_points = GUYSKO_MAX_LP;
	velocity *guysko_vel = malloc(sizeof(velocity));
	guysko_vel->x = 0;
	guysko_vel->y = 0;
	move *guysko_mov = malloc(sizeof(move));
	guysko_mov->x = 0;
	guysko_mov->y = 0;
	guysko_mov->x_remainder = 0;
	guysko_mov->y_remainder = 0;
	position *guysko_pos = malloc(sizeof(position));
	guysko_pos->x = GUYSKO_SPAWN_X;
	guysko_pos->y = GUYSKO_SPAWN_Y;

	player->lp->life_points = lp->life_points;
	player->state 					= 0;
	player->vel							= guysko_vel;
	player->mov 						= guysko_mov;
	player->pos		 					= guysko_pos;
	player->standing_bits		= 0b1111111111111111;

	return player;
}
