/*
 * guysko.c
 *
 *  Created on: 21 Feb 2023
 *      Author: gabrijel
 */




#include <stdlib.h>
#include <stdbool.h>
#include <ugui.h>

#include "guysko.h"
#include "utils.h"

bool in_lava = false;

bool building_mode = false;
block_t building_material = _dirt;

void display_guysko_hp (guysko* player) {
	float hp_percentage = (float) get_life_points(player->lp) / (float) GUYSKO_MAX_LP;
	float hp_line_length = hp_percentage * GUYSKO_HP_BAR_PIXEL_X;

	// TODO: change to _HW_FillFrame_
	UG_FillFrame(1, 1, 2 + GUYSKO_HP_BAR_PIXEL_X + 1, 2 + GUYSKO_HP_BAR_PIXEL_Y + 1, C_BLACK);
	UG_FillFrame(2, 2, 2 + hp_line_length - 1, 2 + GUYSKO_HP_BAR_PIXEL_Y, C_RED);
}

void update_guysko_hp (guysko* player, short y_diff) {
//	set_life_points(player->lp, get_life_points(player->lp) + y_diff);
	player->lp->life_points -= y_diff;
}

/*
 * Simpy calculated the movement guysko makes based on his move. But when he goes outside of range of world in
 * x axis, he is teleported on the opposite side of the world. This does not apply for the y axis, where he
 * can be stuck at the botton of the world and of course won't fall down againg from sky.
 */
void update_guysko_position (guysko* player) {
	update_position_x(player->pos, player->pos->x, player->mov->x);
	update_position_y(player->pos, player->pos->y, (-1) * player->mov->y);
}

/*
 * Because it is posible at slow speed and high FPS for guysko to move less than one pixel in difference of
 * one frame, the traveled distance that was not displayed in previos frame is at this frame taken into calculation
 * with respect to value in "player->mov->x_remainder".
 */
void update_guysko_move (guysko* player, int FPS) {
	float mov_x = ((float) 1 / ((float) (1000 / FPS) / (float) player->vel->x)) + player->mov->x_remainder;
	float mov_y = ((float) 1 / ((float) (1000 / FPS) / (float) player->vel->y)) + player->mov->y_remainder;

	short pix_move_x = (short) mov_x % 100;
	short pix_move_y = (short) mov_y % 100;

	player->mov->x_remainder = mov_x - (float) pix_move_x;
	player->mov->y_remainder = mov_y - (float) pix_move_y;

	set_move(player->mov, pix_move_x, pix_move_y);
}

/*
 * Guysko's velocity is calculated based on force of gravity and his jump acceleration. When calculating
 * velocity of each direction, button input is taken into consideration.
 */
void update_guysko_velocity(guysko* player) {

	// TODO: preverba ali se je zaletel v solid levo desno gor, dol?
	// TODO: update movement based on that

	short old_free_fall_speed = player->vel->y;

	bool collision_up = collision(_solid, _up, player->pos, GUYSKO_IMG_X, GUYSKO_IMG_Y);
	bool collision_right = collision(_solid, _right, player->pos, GUYSKO_IMG_X, GUYSKO_IMG_Y);
	bool collision_down = collision(_solid, _down, player->pos, GUYSKO_IMG_X, GUYSKO_IMG_Y);
	bool collision_left = collision(_solid, _left, player->pos, GUYSKO_IMG_X, GUYSKO_IMG_Y);

//	if (collision_up) {
//		_HW_FillFrame_(20, 20, 30, 30, C_GREEN);
//	}
//
//	if (collision_down) {
//		_HW_FillFrame_(30, 20, 40, 30, C_BLUE);
//	}
//
	if (collision_left) {
		_HW_FillFrame_(40, 40, 50, 50, C_RED);
	} else {
		_HW_FillFrame_(40, 40, 50, 50, C_GREEN);
	}
//
//	if (collision_right) {
//		_HW_FillFrame_(50, 20, 60, 30, C_PURPLE);
//	}
	if (move_right) {
		player->orientation = true;
		if (player->vel->x < 0) {
			set_velocity(player->vel, 0 + GUYSKO_WALK_VEL_INC, player->vel->y);		// Walk right
		} else {
			set_velocity(player->vel, player->vel->x + GUYSKO_WALK_VEL_INC, player->vel->y);
		}
		// Single step collision
		if (collision_right && collision_down && !collision_up) { // && pixel_to_block(player->pos->y) <= LVL1_HMAP[pixel_to_block(player->pos->x)]) {
			update_position_y(player->pos, player->pos->y, -BLOCK_WIDTH);
		}

		// MAX VELOCITY IN X DIRECTION
		if (player->vel->x > GUYSKO_MAX_RIGHT_VELOCITY) {
			set_velocity(player->vel, GUYSKO_MAX_RIGHT_VELOCITY, player->vel->y);
		}

		action_reset(MOVE_RIGHT_INDEX);

		// Stop if collision
		if (collision_right) {
			set_velocity(player->vel, 0, player->vel->y);
		}

	} else if (move_left) {
		player->orientation = false;

		// Walk left
		if (player->vel->x > 0) {
			set_velocity(player->vel, 0 - GUYSKO_WALK_VEL_INC, player->vel->y);
		} else {
			set_velocity(player->vel, player->vel->x - GUYSKO_WALK_VEL_INC, player->vel->y);
		}

		// Single step
		if (collision_left && collision_down && !collision_up) {// && pixel_to_block(player->pos->y) <= LVL1_HMAP[pixel_to_block(player->pos->x)]) {
			update_position_y(player->pos, player->pos->y, -BLOCK_WIDTH);
		}

		// MAX VELOCITY IN X DIRECTION
		if (player->vel->x < GUYSKO_MAX_LEFT_VELOCITY) {
			set_velocity(player->vel, GUYSKO_MAX_LEFT_VELOCITY, player->vel->y);
		}
		action_reset(MOVE_LEFT_INDEX);

		// Stop if collision
		if (collision_left) {
			set_velocity(player->vel, 0, player->vel->y);
		}
	} else {
		// Stand still
		set_velocity(player->vel, 0, player->vel->y);
	}

	// y axis - gravity
	set_velocity(player->vel, player->vel->x, player->vel->y + GRAVITY);


	if (collision_down) {
		// jump
		if (move_up) {
			set_velocity(player->vel, player->vel->x, player->vel->y + GUYSKO_JUMP_ACCELERATION);
			action_reset(MOVE_UP_INDEX);
			if (collision_up) {
				set_velocity(player->vel, player->vel->x, 0);
			}
		} else {
			set_velocity(player->vel, player->vel->x, 0);
		}
		if (old_free_fall_speed < - 400) {
			in_lava = false;
			update_guysko_hp(player, FALL_DAMAGE);
		}
	}
	// Check for collision upwards regardless of the ground
	if (collision_up) {
		set_velocity(player->vel, player->vel->x, 0);
	}

	// MAX VELOCITY IN Y DIRECTION
	if (player->vel->y < GUYSKO_MAX_DOWN_VELOCITY) {
		set_velocity(player->vel, player->vel->x, GUYSKO_MAX_DOWN_VELOCITY);
	} else if (player->vel->y > GUYSKO_MAX_UP_VELOCITY) {
		set_velocity(player->vel, player->vel->x, GUYSKO_MAX_UP_VELOCITY);
	}

	bool collision_lava = collision(_harmful, _down, player->pos, GUYSKO_IMG_X, GUYSKO_IMG_Y) ||
			collision(_harmful, _left, player->pos, GUYSKO_IMG_X, GUYSKO_IMG_Y) ||
			collision(_harmful, _up, player->pos, GUYSKO_IMG_X, GUYSKO_IMG_Y) ||
			collision(_harmful, _right, player->pos, GUYSKO_IMG_X, GUYSKO_IMG_Y);
	if (collision_lava && in_lava) {
		update_guysko_hp(player, LAVA_DAMAGE);
		in_lava = false;
		//		update_position_y(player->pos, player->pos->y, -5*BLOCK_WIDTH);
//		player->vel->y = 200;
	}

	return;
	// TODO: DIFFERENCE OF PREVIOUS AND NEW VELOCITY: FOR DAMAGE OF HIGH FALL
}

/*
 * Function draw_guysko takes guysko's postion and draws it on screen. The postion of guysko denotes the bottom right
 * corner of guysko's image. The postion of guysko cannot be taken as the postion of guysko on screen,
 * because the screen is only a small part of world. So the starting drawing postion is calculated based on
 * guysko's postion and difference between the START of the left and top corner of the screen and left and
 * top corner of the world.
 */
void draw_guysko (guysko* player) {
	int index = 0;
	posx_pixel draw_startPoint_x = world_pixel_to_scene_pixel_x_band(player->pos->x - GUYSKO_IMG_X);
	posy_pixel draw_startPoint_y = world_pixel_to_scene_pixel_y_band(player->pos->y - GUYSKO_IMG_Y);
	// TODO: if guysko is on the edge of world, do not draw the whole guysko!

	uint8_t* picture_pointer;
	uint16_t* pallete_pointer;
	if (player->orientation){
		picture_pointer = guysko_r_0;
		pallete_pointer = GUYSKO_R_0;
	} else {
		picture_pointer = guysko_l_0;
		pallete_pointer = GUYSKO_L_0;
	}

	for (int i = 0; i < GUYSKO_IMG_SIZE / 2; i += 1) {
		uint8_t offset_x = index % (GUYSKO_IMG_X / 2);
		uint8_t offset_y = index / (GUYSKO_IMG_X / 2);
		index++;
//		int frst_nibble =	(guysko_r_0[i] & 0b11110000) >> 4;
//		int scnd_nibble =	(guysko_r_0[i] & 0b00001111) >> 0;
		int frst_nibble =	(picture_pointer[i] & 0b11110000) >> 4;
		int scnd_nibble =	(picture_pointer[i] & 0b00001111) >> 0;
		pixel_c draw_on_screen_x = world_pixel_to_world_pixel_x_no_band_param(draw_startPoint_x, 2 * offset_x);
		pixel_c draw_on_screen_y = world_pixel_to_world_pixel_y_no_band_param(draw_startPoint_y, offset_y);

		if (frst_nibble != 0) {
			UG_DrawPixel(draw_on_screen_x, draw_on_screen_y, pallete_pointer[frst_nibble]);
		} else if (false) {
				// drug object uzadi
		} else {
			overdraw_background_pixel(draw_on_screen_x, draw_on_screen_y);
		}

		if (scnd_nibble != 0) {
			UG_DrawPixel(draw_on_screen_x + 1, draw_on_screen_y, pallete_pointer[scnd_nibble]);
		} else if (false) {
			// drug object uzadi
		} else {
			overdraw_background_pixel(draw_on_screen_x + 1, draw_on_screen_y);
		}
	}
}

/*
 * TODO: fix
 */
void camouflage (guysko* player, uint16_t prev_guysko_x, uint16_t prev_guysko_y) {

	pixel_c guysko_x0 = prev_guysko_x;
	pixel_c guysko_y0 = prev_guysko_y;
	pixel_c guysko_x1 = player->pos->x;
	pixel_c guysko_y1 = player->pos->y;

	// calculate the starting points of guysko image starting point
	// IN SCENE IN PIXELS on previous and current frame
//	int prev_draw_startPoint_x = prev_guysko_x - GUYSKO_IMG_X - (camera_x_block * BLOCK_WIDTH - (SCENE_WIDTH_BLOCKS / 2) * BLOCK_WIDTH);
	int prev_draw_startPoint_x = world_pixel_to_scene_pixel_x_band(prev_guysko_x - GUYSKO_IMG_X);
//	int prev_draw_startPoint_y = prev_guysko_y - GUYSKO_IMG_Y - (camera_y_block * BLOCK_WIDTH - (SCENE_HEIGHT_BLOCKS / 2) * BLOCK_WIDTH);
	int prev_draw_startPoint_y = world_pixel_to_scene_pixel_y_band(prev_guysko_y - GUYSKO_IMG_Y);
//	int draw_startPoint_x = player->pos->x - GUYSKO_IMG_X - (camera_x_block * BLOCK_WIDTH - (SCENE_WIDTH_BLOCKS / 2)* BLOCK_WIDTH);
	int draw_startPoint_x = world_pixel_to_scene_pixel_x_band(player->pos->x - GUYSKO_IMG_X);
//	int draw_startPoint_y = player->pos->y - GUYSKO_IMG_Y - (camera_y_block * BLOCK_WIDTH - (SCENE_HEIGHT_BLOCKS / 2)* BLOCK_WIDTH);
	int draw_startPoint_y = world_pixel_to_scene_pixel_y_band(player->pos->y - GUYSKO_IMG_Y);
	// calculate the difference the guysko has made since the previous frame (his previous drawing)
	// the difference is in pixels but on screen! Meaning it only checks for the difference it made on
	// screen, not his global moving in WORLD!
	short x_diff = draw_startPoint_x - prev_draw_startPoint_x;
	short y_diff = draw_startPoint_y - prev_draw_startPoint_y;

	// calculate the starting points of guysko image starting point
	// IN WORLD IN PIXELS on previous and current frame
	// make four calls based on which direction the guysko moved in:
	uint8_t padding = 8;
	if (x_diff > 0) {		// right
		overdraw_background_rectangle(world_pixel_to_world_pixel_x_no_band_param(guysko_x0, -GUYSKO_IMG_X - padding), world_pixel_to_world_pixel_y_no_band_param(guysko_y0, -GUYSKO_IMG_Y),
				world_pixel_to_world_pixel_x_no_band_param(guysko_x1, -GUYSKO_IMG_X),world_pixel_to_world_pixel_y_no_band_param (guysko_y1, 0));

		overdraw_background_rectangle(world_pixel_to_world_pixel_x_no_band_param(guysko_x1, 0), world_pixel_to_world_pixel_y_no_band_param(guysko_y1, -GUYSKO_IMG_Y),
						world_pixel_to_world_pixel_x_no_band_param(guysko_x1 + 4, 0),world_pixel_to_world_pixel_y_no_band_param (guysko_y1, 0));

	} else if (x_diff < 0) {	// left
		overdraw_background_rectangle(world_pixel_to_world_pixel_x_no_band_param(guysko_x0, padding), world_pixel_to_world_pixel_y_no_band_param(guysko_y0, -GUYSKO_IMG_Y),
				world_pixel_to_world_pixel_x_no_band_param(guysko_x1, 0), world_pixel_to_world_pixel_y_no_band_param(guysko_y1, 0));

		overdraw_background_rectangle(world_pixel_to_world_pixel_x_no_band_param(guysko_x1 - GUYSKO_IMG_X, 0), world_pixel_to_world_pixel_y_no_band_param(guysko_y1, -GUYSKO_IMG_Y),
								world_pixel_to_world_pixel_x_no_band_param(guysko_x1 - GUYSKO_IMG_X - 4, 0),world_pixel_to_world_pixel_y_no_band_param (guysko_y1, 0));

	}

	if (y_diff > 0) {	// down
		overdraw_background_rectangle(world_pixel_to_world_pixel_x_no_band_param(guysko_x0, -GUYSKO_IMG_X), world_pixel_to_world_pixel_y_no_band_param(guysko_y0, -GUYSKO_IMG_Y - padding),
				world_pixel_to_world_pixel_x_no_band_param(guysko_x1, 0), world_pixel_to_world_pixel_y_no_band_param(guysko_y1, (-1) * GUYSKO_IMG_Y));
	} else if (y_diff < 0) {	// up
		overdraw_background_rectangle(world_pixel_to_world_pixel_x_no_band_param(guysko_x0, -GUYSKO_IMG_X), world_pixel_to_world_pixel_y_no_band_param(guysko_y0, padding),
				world_pixel_to_world_pixel_x_no_band_param(guysko_x1, 0), world_pixel_to_world_pixel_y_no_band_param(guysko_y1, 0));
	}

}

/*
 * calls functions that update guysko properties
 */
void refresh_guysko(guysko* player, int FPS) {
	int old_guysko_hp = get_life_points(player->lp);
	update_guysko_velocity(player);
	update_guysko_move(player, FPS);
	uint16_t prev_guysko_x = player->pos->x;
	uint16_t prev_guysko_y = player->pos->y;
	update_guysko_position(player);

	// do the following two if and only if at least on one of the x and y
	// components of guysko position is different
	if (!(prev_guysko_x == player->pos->x && prev_guysko_y == player->pos->y)) {
		// NOT WORKING, TODO: fix black lines
		camouflage (player, prev_guysko_x, prev_guysko_y);
		draw_guysko(player);
	}

//	if (old_guysko_hp != get_life_points(player->lp)) {
////		display_guysko_hp(player);
//	}
		display_guysko_hp(player);
		if (esc || old_guysko_hp != get_life_points(player->lp)) {
			action_reset(ESC_INDEX);
		}
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

	velocity *guysko_vel = malloc(sizeof(velocity));
	guysko_vel->x = 0;
	guysko_vel->y = 0;

	move *guysko_mov = malloc(sizeof(move));
	guysko_mov->x = 0;
	guysko_mov->y = 0;
	guysko_mov->x_remainder = 0;
	guysko_mov->y_remainder = 0;

	pixel_position *guysko_pos = malloc(sizeof(pixel_position));
	guysko_pos->x = GUYSKO_SPAWN_X;
	guysko_pos->y = GUYSKO_SPAWN_Y;

	life_points *lp = malloc(sizeof(life_points));
	lp->life_points = GUYSKO_MAX_LP;

	player->state 					= 0;
	player->vel							= guysko_vel;
	player->mov 						= guysko_mov;
	player->pos		 					= guysko_pos;
	player->lp 							= lp;
//	player->lp = malloc(sizeof(life_points));
//	player->lp->life_points = GUYSKO_MAX_LP;
	player->standing_bits		= 0b1111111111111111;

	player->orientation = true;

	return player;
}
