/*
 * movable.c
 *
 *  Created on: 15 Mar 2023
 *      Author: gabrijel
 */

/*
 * kako se med seboj razlikujejo npc-ji (drugače):
 * se premikajo:
 * speed
 * algoritem
 * zgledajo
 * demage
 * passive / active
 * lajf
 * način napada
 * habitat -- too much!
 * različna pogostost
 * drop items
 *
 * Each NPC looks, moves, attacks, ... different.
 * Ideas of implementation:
 * attack shall be implemented as function (guysko*, npc*) { calculate }
 * All the NPC shall be kept in a "list"
 * Some NPC shall attack guysko in certain vicinity. (they should know how to come close to him?)
 *
 * kaj jim je skupno?
 * helath points
 * vel, move, position
 */
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>

/* Include my libraries here */

#include "scene.h"
#include "movable.h"

bool movable_in_scene (pixel_position* pos) {
	int up_border = block_to_pixel(camera_y_block) - SCENE_HEIGHT_PIXELS / 2;
	int right_border = block_to_pixel(camera_x_block) + SCENE_WIDTH_PIXELS / 2;
	int bottom_border = block_to_pixel(camera_y_block) + SCENE_HEIGHT_PIXELS / 2;
	int left_border = block_to_pixel(camera_x_block) - SCENE_WIDTH_PIXELS / 2;

	posx_pixel x_pos = pos->x;
	posy_pixel y_pos = pos->y;

	// if postion is not in scene - do not mind drawing it
	if (!(x_pos < right_border && x_pos > left_border)) {
		return false;
	}
	if (!(y_pos > up_border && y_pos < bottom_border )) {
		return false;
	}
}

void camouflage_movable (pixel_position* pos, uint16_t prev_movable_x, uint16_t prev_movable_y, Npcs species) {

	if (!movable_in_scene(pos)) {
			return;
	}

	int size_x = 0;
	int size_y = 0;

	switch (species) {
		case _cow:
			size_x = COW_IMG_X;
			size_y = COW_IMG_Y;
			break;
		case _fashionist:
			size_x = FASHIONIST_IMG_X;
			size_y = FASHIONIST_IMG_Y;
			break;
		case _miner:
			size_x = MINER_IMG_Y;
			size_y = MINER_IMG_Y;
			break;
		case _merchant:
			size_x = MERCHANT_IMG_X;
			size_y = MERCHANT_IMG_Y;
			break;
	}

	short camera_diff_x_pixel = block_to_pixel(old_camera_x - camera_x_block);
	short camera_diff_y_pixel = block_to_pixel(old_camera_y - camera_y_block);
	if (camera_diff_x_pixel == 0 && camera_diff_y_pixel == 0) return;

	pixel_c movable_x0 = prev_movable_x;
	pixel_c movable_y0 = prev_movable_y;
	pixel_c movable_x1 = pos->x;
	pixel_c movable_y1 = pos->y;

	// calculate the starting points of guysko image starting point
	// IN SCENE IN PIXELS on previous and current frame
	int prev_draw_startPoint_x = world_pixel_to_scene_pixel_x_band(movable_x0 - size_x - 5);
	int prev_draw_startPoint_y = world_pixel_to_scene_pixel_y_band(movable_y0 - size_y - 5);
	int draw_startPoint_x = world_pixel_to_scene_pixel_x_band(movable_x1 - size_x - camera_diff_x_pixel);
	int draw_startPoint_y = world_pixel_to_scene_pixel_y_band(movable_y1 - size_y - camera_diff_y_pixel);

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
		overdraw_background_rectangle(world_pixel_to_world_pixel_x_no_band_param(movable_x0, -size_x - padding), world_pixel_to_world_pixel_y_no_band_param(movable_y0, -size_y),
				world_pixel_to_world_pixel_x_no_band_param(movable_x1, -size_x),world_pixel_to_world_pixel_y_no_band_param (movable_y1, 0));

		overdraw_background_rectangle(world_pixel_to_world_pixel_x_no_band_param(movable_x1, 0), world_pixel_to_world_pixel_y_no_band_param(movable_y1, -size_y),
						world_pixel_to_world_pixel_x_no_band_param(movable_x1 + 4, 0),world_pixel_to_world_pixel_y_no_band_param (movable_y1, 0));
	} else if (x_diff < 0) {	// left
		overdraw_background_rectangle(world_pixel_to_world_pixel_x_no_band_param(movable_x0, padding), world_pixel_to_world_pixel_y_no_band_param(movable_y0, -size_y),
				world_pixel_to_world_pixel_x_no_band_param(movable_x1, 0), world_pixel_to_world_pixel_y_no_band_param(movable_y1, 0));

		overdraw_background_rectangle(world_pixel_to_world_pixel_x_no_band_param(movable_x1 - size_x, 0), world_pixel_to_world_pixel_y_no_band_param(movable_y1, -size_y),
								world_pixel_to_world_pixel_x_no_band_param(movable_x1 - size_x - 4, 0),world_pixel_to_world_pixel_y_no_band_param (movable_y1, 0));
	}

	if (y_diff > 0) {	// down
		overdraw_background_rectangle(world_pixel_to_world_pixel_x_no_band_param(movable_x0, -size_x), world_pixel_to_world_pixel_y_no_band_param(movable_y0, -size_y - padding),
				world_pixel_to_world_pixel_x_no_band_param(movable_x1, 0), world_pixel_to_world_pixel_y_no_band_param(movable_y1, (-1) * size_y));
	} else if (y_diff < 0) {	// up
		overdraw_background_rectangle(world_pixel_to_world_pixel_x_no_band_param(movable_x0, -size_x), world_pixel_to_world_pixel_y_no_band_param(movable_y0, padding),
				world_pixel_to_world_pixel_x_no_band_param(movable_x1, 0), world_pixel_to_world_pixel_y_no_band_param(movable_y1, 0));

		overdraw_background_rectangle(world_pixel_to_world_pixel_x_no_band_param(movable_x1 - size_x, 0), world_pixel_to_world_pixel_y_no_band_param(movable_y1, -size_y-5),
				world_pixel_to_world_pixel_x_no_band_param(movable_x1, 0), world_pixel_to_world_pixel_y_no_band_param(movable_y1, -size_y));

	}

}

void draw_movable(uint8_t* pic, uint16_t* pic_colors, pixel_position* pos, Npcs species) {


	if (!movable_in_scene(pos)) {
		return;
	}

	posx_pixel x_pos = pos->x;
	posy_pixel y_pos = pos->y;

	int size_x = 0;
	int size_y = 0;

	switch (species) {
		case _cow:
			size_x = COW_IMG_X;
			size_y = COW_IMG_Y;
			break;
		case _fashionist:
			size_x = FASHIONIST_IMG_X;
			size_y = FASHIONIST_IMG_Y;
			break;
		case _miner:
			size_x = MINER_IMG_X;
			size_y = MINER_IMG_Y;
			break;
		case _merchant:
			size_x = MERCHANT_IMG_X;
			size_y = MERCHANT_IMG_Y;
			break;
	}
	int size = size_x * size_y;

	int index = 0;
	uint16_t draw_startPoint_x 	= world_pixel_to_scene_pixel_x_band(x_pos - size_x);
	uint16_t draw_startPoint_y 	= world_pixel_to_scene_pixel_y_band(y_pos - size_y);
	uint8_t offset_x;
	uint8_t offset_y;
	int frst_nibble;
	int scnd_nibble;

	for (int i = 0; i < size / 2; i += 1) {
		offset_x = index % (size_x / 2);
		offset_y = index / (size_x / 2);
		index++;
		frst_nibble = (pic[i] & 0b11110000) >> 4;
		scnd_nibble = (pic[i] & 0b00001111) >> 0;
		if (pic_colors[frst_nibble] != 0) {
			UG_DrawPixel(draw_startPoint_x + 2 * offset_x, draw_startPoint_y + offset_y, pic_colors[frst_nibble]);
		} else {
			overdraw_background_pixel(draw_startPoint_x + 2 * offset_x,  draw_startPoint_y + offset_y);
		}
		if (pic_colors[frst_nibble] != 0) {
			UG_DrawPixel(draw_startPoint_x + 2 * offset_x + 1, draw_startPoint_y + offset_y, pic_colors[scnd_nibble]);
		} else {
			overdraw_background_pixel(draw_startPoint_x + 2 * offset_x + 1, draw_startPoint_y + offset_y);
		}
	}

}

/*
 * Insert cow to linked list of movables
 */
void insert_cow (movable* beings, cow* movable_cow) {
	cow* new_cow = (cow*) malloc(sizeof(cow));
	new_cow = movable_cow;
	new_cow->prev = beings->tail_cow->prev;
	new_cow->next = beings->tail_cow;

	cow* penultimate_cow = beings->tail_cow;
	penultimate_cow = penultimate_cow->prev;
	penultimate_cow->next = new_cow;
	beings->tail_cow->prev = new_cow;
}
void insert_fashionist (movable* beings, fashionist* fash) {
	fashionist* new_fashionist = (fashionist*) malloc(sizeof(fashionist));
	new_fashionist = fash;
	new_fashionist->prev = beings->tail_fashionist->prev;
	new_fashionist->next = beings->tail_fashionist;

	fashionist* penultimate_fashionist = beings->tail_fashionist;
	penultimate_fashionist = penultimate_fashionist->prev;
	penultimate_fashionist->next = new_fashionist;
	beings->tail_fashionist->prev = new_fashionist;
}
void insert_miner (movable* beings, miner* min) {
	miner* new_miner = (miner*) malloc(sizeof(miner));
	new_miner = min;
	new_miner->prev = beings->tail_miner->prev;
	new_miner->next = beings->tail_miner;

	miner* penultimate_miner = beings->tail_miner;
	penultimate_miner = penultimate_miner->prev;
	penultimate_miner->next = new_miner;
	beings->tail_miner->prev = new_miner;
}
void insert_merchant (movable* beings, merchant* merch) {
	merchant* new_merchant = (merchant*) malloc(sizeof(merchant));
	new_merchant = merch;
	new_merchant->prev = beings->tail_merchant->prev;
	new_merchant->next = beings->tail_merchant;

	merchant* penultimate_merchant = beings->tail_merchant;
	penultimate_merchant = penultimate_merchant->prev;
	penultimate_merchant->next = new_merchant;
	beings->tail_merchant->prev = new_merchant;
}

/*
 * Remove cow that is given with struct 'movable_cow' from linked list in movable
 */
void remove_cow (cow* movable_cow) {
	cow* preceding_movable_cow = movable_cow->prev;
	cow* following_movable_cow = movable_cow->next;
	preceding_movable_cow->next = movable_cow->next;
	following_movable_cow->prev = movable_cow->prev;
	free(movable_cow);
}

/*
 * Function tries to spawn each of the movables to the world
 * according to their spawn probabilty and current capacity
 * of beings
 */
void insert_movables(movable* beings) {
	// cows
	if (beings->beings_quantity < MAX_MOVABLE_CAPACTIY) {

		life_points *lp = (life_points*) malloc(sizeof(life_points));
		pixel_position* pos = (pixel_position*) malloc(sizeof(pixel_position));
		velocity *vel = (velocity*) malloc(sizeof(velocity));
		move* mov = (move*) malloc(sizeof(move));

		pos->x = 200;
		pos->y = 50;
		vel->y = 0;
		vel->x = 0;
		mov->x = 0;
		mov->y = 0;
		mov->x_remainder = 0;
		mov->y_remainder = 0;

		// uncomment to make generate movables randomly
//		int random_npc = HAL_GetTick() % MOVABLE_DIFFERENT_SPECIES;
//
//		switch (random_npc) {
//			case _cow:
				lp->life_points = COW_MAX_LP;
				cow* generated_cow = (cow*) malloc(sizeof(cow));
				generated_cow =	new_cow(lp, vel, pos, mov);
				insert_cow(beings, generated_cow);
				beings->beings_quantity++;
//				break;
//			case _fashionist:
				// The position is static for demonstrational puposes, uncomment for dynamic position!
				pixel_position* pos1 = (pixel_position*) malloc(sizeof(pixel_position));
				pos1->x = 410;
				pos1->y = 50;
				lp->life_points = FASHIONIST_MAX_LP;
				fashionist* generated_fashionist1 = (fashionist*) malloc(sizeof(fashionist));
				generated_fashionist1 =	new_fashionist(lp, vel, pos1, mov);
				insert_fashionist(beings, generated_fashionist1);
				beings->beings_quantity++;


//				break;
//			case _miner:
				// The position is static for demonstrational puposes, uncomment for dynamic position!
				pixel_position* pos2 = (pixel_position*) malloc(sizeof(pixel_position));
				pos2->x = 1200;
				pos2->y = 50;
				lp->life_points = MINER_MAX_LP;
				miner* generated_miner = (miner*) malloc(sizeof(miner));
				generated_miner =	new_miner(lp, vel, pos2, mov);
				insert_miner(beings, generated_miner);
				beings->beings_quantity++;
//				break;
//			case _merchant:
				pixel_position* pos3 = (pixel_position*) malloc(sizeof(pixel_position));
				pos3->x = 750;
				pos3->y = 50;
				lp->life_points = MERCHANT_MAX_LP;
				merchant* generated_merchant = (merchant*) malloc(sizeof(merchant));
				generated_merchant =	new_merchant(lp, vel, pos3, mov);
				insert_merchant(beings, generated_merchant);
				beings->beings_quantity++;
//				break;
//		}
	}
}


void update_movables_position(pixel_position* pos, move* mov) {
	update_position_x(pos, pos->x, mov->x);
	update_position_y(pos, pos->y, (-1) * mov->y);
}

void update_movables_move(move* mov, velocity* vel, uint8_t FPS){
	float mov_x = ((float) 1 / ((float) (1000 / FPS) / (float) vel->x)) + mov->x_remainder;
	float mov_y = ((float) 1 / ((float) (1000 / FPS) / (float) vel->y)) + mov->y_remainder;

	short pix_move_x = (short) mov_x % 100;
	short pix_move_y = (short) mov_y % 100;

	mov->x_remainder = mov_x - (float) pix_move_x;
	mov->y_remainder = mov_y - (float) pix_move_y;

	set_move(mov, pix_move_x, pix_move_y);
}

void update_movables_velocity(velocity* vel, pixel_position* pos, uint8_t species){

	uint16_t img_size_x = 0;
	uint16_t img_size_y = 0;

	short walk_vel_inc = 0;
	short jump_vel_inc = 0;

	short max_up_vel = 0;
	short max_right_vel = 0;
	short max_down_vel = 0;
	short max_left_vel = 0;

	switch (species) {
		case _cow:
			img_size_x = COW_IMG_X;
			img_size_y = COW_IMG_Y;

			max_up_vel = COW_MAX_UP_VELOCITY;
			max_right_vel = COW_MAX_RIGHT_VELOCITY;
			max_down_vel = COW_MAX_DOWN_VELOCITY;
			max_left_vel = COW_MAX_LEFT_VELOCITY;

			walk_vel_inc = COW_WALK_VEL_INC;
			jump_vel_inc = COW_JUMP_ACCELERATION;
			break;
		case _fashionist:
			img_size_x = FASHIONIST_IMG_X;
			img_size_y = FASHIONIST_IMG_Y;

			max_up_vel = FASHIONIST_MAX_UP_VELOCITY;
			max_right_vel = FASHIONIST_MAX_RIGHT_VELOCITY;
			max_down_vel = FASHIONIST_MAX_DOWN_VELOCITY;
			max_left_vel = FASHIONIST_MAX_LEFT_VELOCITY;

			walk_vel_inc = FASHIONIST_WALK_VEL_INC;
			jump_vel_inc = FASHIONIST_JUMP_ACCELERATION;
			break;
		case _miner:
			img_size_x = MINER_IMG_X;
			img_size_y = MINER_IMG_Y;

			max_up_vel = MINER_MAX_UP_VELOCITY;
			max_right_vel = MINER_MAX_RIGHT_VELOCITY;
			max_down_vel = MINER_MAX_DOWN_VELOCITY;
			max_left_vel = MINER_MAX_LEFT_VELOCITY;

			walk_vel_inc = MINER_WALK_VEL_INC;
			jump_vel_inc = MINER_JUMP_ACCELERATION;
			break;
		case _merchant:
			img_size_x = MERCHANT_IMG_X;
			img_size_y = MERCHANT_IMG_Y;

			max_up_vel = MERCHANT_MAX_UP_VELOCITY;
			max_right_vel = MERCHANT_MAX_RIGHT_VELOCITY;
			max_down_vel = MERCHANT_MAX_DOWN_VELOCITY;
			max_left_vel = MERCHANT_MAX_LEFT_VELOCITY;

			walk_vel_inc = MERCHANT_WALK_VEL_INC;
			jump_vel_inc = MERCHANT_JUMP_ACCELERATION;
			break;
	}

	bool collision_up = collision(_solid, _up, pos, img_size_x, img_size_y);
	bool collision_right = collision(_solid, _right, pos, img_size_x, img_size_y);
	bool collision_down = collision(_solid, _down, pos, img_size_x, img_size_y);
	bool collision_left = collision(_solid, _left, pos, img_size_x, img_size_y);
//
	bool rand_up = false;
	bool rand_right = false;
	bool rand_left = false;
	bool rand_down = false;

// set rand_x to true to make moving dynamical
	volatile uint32_t vertical_move = HAL_GetTick();
	if (vertical_move % 100 < 20) {
		rand_up = false;
	} else if (vertical_move % 100 > 80) {

	}


	volatile uint32_t horizontal_move = HAL_GetTick();
	if (horizontal_move % 100 < 80) {
		rand_right = false;
	} else if (horizontal_move % 100 > 80){
		rand_left = false;
	}

	/* ----------------------------------------------- HORIZONTAL ----------------------------------------------- */
	if (rand_right) {
//		player->orientation = true;
			if (vel->x < 0) {
				set_velocity(vel, 0 + walk_vel_inc, vel->y);		// Walk right
			} else {
				set_velocity(vel, vel->x + walk_vel_inc, vel->y);
			}
			// Single step collision
			if (collision_right && collision_down && !collision_up) { // && pixel_to_block(player->pos->y) <= LVL1_HMAP[pixel_to_block(player->pos->x)]) {
				update_position_y(pos, pos->y, -BLOCK_WIDTH);
			}

			// MAX VELOCITY IN X DIRECTION
			if (vel->x > max_right_vel) {
				set_velocity(vel, max_right_vel, vel->y);
			}
			// Stop if collision
			if (collision_right) {
				set_velocity(vel, 0, vel->y);
			}

		} else if (rand_left) {
//			player->orientation = false;

			// Walk left
			if (vel->x > 0) {
				set_velocity(vel, 0 - walk_vel_inc, vel->y);
			} else {
				set_velocity(vel, vel->x - walk_vel_inc, vel->y);
			}

			// Single step
			if (collision_left && collision_down && !collision_up) {// && pixel_to_block(player->pos->y) <= LVL1_HMAP[pixel_to_block(player->pos->x)]) {
				update_position_y(pos, pos->y, -BLOCK_WIDTH);
			}

			// MAX VELOCITY IN X DIRECTION
			if (vel->x < max_left_vel) {
				set_velocity(vel, max_left_vel, vel->y);
			}

			// Stop if collision
			if (collision_left) {
				set_velocity(vel, 0, vel->y);
			}
		} else {
			// Stand still
			set_velocity(vel, 0, vel->y);
		}

	/* -----------------------------------------------            ----------------------------------------------- */


	/* ----------------------------------------------- VERTICAL ----------------------------------------------- */
		set_velocity(vel, vel->x, vel->y + GRAVITY);

		if (collision_down) {
			// jump
			if (rand_up) {
				set_velocity(vel, vel->x, vel->y + jump_vel_inc);
				if (collision_up) {
					set_velocity(vel, vel->x, 0);
				}
			} else {
				set_velocity(vel, vel->x, 0);
			}
		}

		// Check for collision upwards regardless of the ground
		if (collision_up) {
			set_velocity(vel, vel->x, 0);
		}

		// MAX VELOCITY IN Y DIRECTION
		if (vel->y < max_down_vel) {
			set_velocity(vel, vel->x, max_down_vel);
		} else if (vel->y > max_up_vel) {
			set_velocity(vel, vel->x, max_down_vel);
		}

		/* -----------------------------------------------          ----------------------------------------------- */
}

void refresh_movables(movable* beings, uint8_t FPS) {
	insert_movables(beings);

	/* -------------------------------- travers through COWS -------------------------------- */
	cow* travers_cow = (cow*) malloc(sizeof(cow));
	travers_cow = beings->header_cow->next;

	fashionist* travers_fashionist = (fashionist*) malloc(sizeof(fashionist));
	travers_fashionist = beings->header_fashionist->next;

	miner* travers_miner = (miner*) malloc(sizeof(miner));
	travers_miner = beings->header_miner->next;

	merchant* travers_merchant = (merchant*) malloc(sizeof(merchant));
	travers_merchant = beings->header_merchant->next;

	bool cows_finished = false;
	bool fash_finished = false;
	bool miner_finished = false;
	bool merchant_finished = false;

	while(1) {

		if (travers_cow != beings->tail_cow && travers_cow != NULL) {
			uint16_t prev_cow_x = travers_cow->pos->x;
			uint16_t prev_cow_y = travers_cow->pos->y;
			update_movables_velocity(travers_cow->vel, travers_cow->pos, _cow);
			update_movables_move(travers_cow->mov, travers_cow->vel, FPS);
			update_movables_position(travers_cow->pos, travers_cow->mov);
			draw_movable(cow_r_0, cow_colors_0, travers_cow->pos, _cow);
			camouflage_movable(travers_cow->pos, prev_cow_x, prev_cow_y, _cow);
			/* if dynamic movement of npc-s: uncomment if sentence and put in the draw and camo function */
//			if (!(prev_cow_x == travers_cow->pos->x && prev_cow_y == travers_cow->pos->y)) {
//			}
			travers_cow = travers_cow->next;
		} else {
			cows_finished = true;
		}

		if (travers_fashionist != beings->tail_fashionist && travers_fashionist != NULL) {
			uint16_t prev_fashionist_x = travers_fashionist->pos->x;
			uint16_t prev_fashionist_y = travers_fashionist->pos->y;
			update_movables_velocity(travers_fashionist->vel, travers_fashionist->pos, _fashionist);
			update_movables_velocity(travers_fashionist->vel, travers_fashionist->pos, _fashionist);
			update_movables_move(travers_fashionist->mov, travers_fashionist->vel, FPS);
			update_movables_position(travers_fashionist->pos, travers_fashionist->mov);
			draw_movable(fashionist_r_0, fashionist_colors_0, travers_fashionist->pos, _fashionist);
			camouflage_movable(travers_fashionist->pos, prev_fashionist_x, prev_fashionist_y, _fashionist);
			/* if dynamic movement of npc-s: uncomment if sentence and put in the draw and camo function */
//			if (!(prev_fashionist_x == travers_fashionist->pos->x && prev_fashionist_y == travers_fashionist->pos->y)) {
//			}
			travers_fashionist = travers_fashionist->next;
		} else {
			fash_finished = true;
		}

		if (travers_miner != beings->tail_miner && travers_miner != NULL) {
			uint16_t prev_miner_x = travers_miner->pos->x;
			uint16_t prev_miner_y = travers_miner->pos->y;
			update_movables_velocity(travers_miner->vel, travers_miner->pos, _miner);
			update_movables_move(travers_miner->mov, travers_miner->vel, FPS);
			update_movables_position(travers_miner->pos, travers_miner->mov);
			draw_movable(miner_r_0, miner_colors_0, travers_miner->pos, _miner);
			camouflage_movable(travers_miner->pos, prev_miner_x, prev_miner_y, _miner);
			/* if dynamic movement of npc-s: uncomment if sentence and put in the draw and camo function */
			//			if (!(prev_miner_x == travers_miner->pos->x && prev_miner_y == travers_miner->pos->y)) {
//			}
			travers_miner = travers_miner->next;
		} else {
			 miner_finished = true;
		}

		if (travers_merchant != beings->tail_merchant && travers_merchant != NULL) {
			uint16_t prev_merchant_x = travers_merchant->pos->x;
			uint16_t prev_merchant_y = travers_merchant->pos->y;
			update_movables_velocity(travers_merchant->vel, travers_merchant->pos, _merchant);
			update_movables_move(travers_merchant->mov, travers_merchant->vel, FPS);
			update_movables_position(travers_merchant->pos, travers_merchant->mov);
			draw_movable(merchant_r_0, merchant_colors_0, travers_merchant->pos, _merchant);
			camouflage_movable(travers_merchant->pos, prev_merchant_x, prev_merchant_y, _merchant);
			/* if dynamic movement of npc-s: uncomment if sentence and put in the draw and camo function */
//			if (!(prev_merchant_x == travers_merchant->pos->x && prev_merchant_y == travers_merchant->pos->y)) {
//			}
			travers_merchant = travers_merchant->next;
		} else {
			merchant_finished = true;
		}

		if (cows_finished && fash_finished && miner_finished && merchant_finished) break;
	}
}
<<<<<<< HEAD

void update_movables(movable* beings) {


}

void refresh_movables(movable* beings, uint8_t FPS) {

}

=======
>>>>>>> refs/remotes/origin/master
/*
 * Initiate linked list with movables. Every species has their own linked list
 */
movable* new_movables() {
	movable* beings = (movable*)malloc(sizeof(movable));

	beings->header_cow  = (cow*)malloc(sizeof(cow));
	beings->tail_cow	 	= (cow*)malloc(sizeof(cow));

	beings->header_cow->next = beings->tail_cow;
	beings->header_cow->prev = NULL;
	beings->tail_cow->next 	= NULL;
	beings->tail_cow->prev 	= beings->header_cow;

	beings->header_fashionist  = (fashionist*)malloc(sizeof(fashionist));
	beings->tail_fashionist	 	= (fashionist*)malloc(sizeof(fashionist));

	beings->header_fashionist->next = beings->tail_fashionist;
	beings->header_fashionist->prev = NULL;
	beings->tail_fashionist->next 	= NULL;
	beings->tail_fashionist->prev 	= beings->header_fashionist;

	beings->header_miner  = (miner*)malloc(sizeof(miner));
	beings->tail_miner	 	= (miner*)malloc(sizeof(miner));

	beings->header_miner->next = beings->tail_miner;
	beings->header_miner->prev = NULL;
	beings->tail_miner->next 	= NULL;
	beings->tail_miner->prev 	= beings->header_miner;

	beings->header_merchant  = (merchant*)malloc(sizeof(merchant));
	beings->tail_merchant	 	= (merchant*)malloc(sizeof(merchant));

	beings->header_merchant->next = beings->tail_merchant;
	beings->header_merchant->prev = NULL;
	beings->tail_merchant->next 	= NULL;
	beings->tail_merchant->prev 	= beings->header_merchant;

	beings->beings_quantity = 0;

	return beings;
}

