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

#include "movable.h"

/*
 * TODO: refactor this function
 */
void draw_movable(uint8_t* pic, uint16_t* pic_colors, pixel_position* pos, Npcs species) {

	int size_x = 0;
	int size_y = 0;

	posx_pixel x_pos = pos->x;
	posy_pixel y_pos = pos->y;

	switch (species) {
		case _cow:
			size_x = COW_IMG_X;
			size_y = COW_IMG_Y;
			break;
		case _librarian:

			break;
	}

	int size = size_x * size_y;

	int up_border = block_to_pixel(camera_y_block) - SCENE_HEIGHT_PIXELS / 2;
	int right_border = block_to_pixel(camera_x_block) + SCENE_WIDTH_PIXELS / 2;
	int bottom_border = block_to_pixel(camera_y_block) + SCENE_HEIGHT_PIXELS / 2;
	int left_border = block_to_pixel(camera_x_block) - SCENE_WIDTH_PIXELS / 2;

	// if postion is not in scene - do not mind drawing it
	if (!(x_pos < right_border && x_pos > left_border)) {
		return;
	}
	if (!(y_pos > up_border && y_pos < bottom_border )) {
		return;
	}

	int index = 0;
	uint16_t draw_startPoint_x 	= world_pixel_to_scene_pixel_x_band(x_pos - size_x);
	uint8_t draw_startPoint_y 	= world_pixel_to_scene_pixel_y_band(y_pos - size_y);
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
		if (pic_colors[frst_nibble] != 0) UG_DrawPixel(draw_startPoint_x + 2 * offset_x, draw_startPoint_y + offset_y, pic_colors[frst_nibble]);
		if (pic_colors[frst_nibble] != 0) UG_DrawPixel(draw_startPoint_x + 2 * offset_x + 1, draw_startPoint_y + offset_y, pic_colors[scnd_nibble]);
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
		if (20 < COW_SPAWN_PROBABILITY) {

			life_points *lp = (life_points*) malloc(sizeof(life_points));
			lp->life_points = COW_MAX_LP;

			pixel_position* cow_pos = (pixel_position*) malloc(sizeof(pixel_position));

			cow_pos->x = rand_range(0, WORLD_WIDTH_PIXELS);
			cow_pos->y = 50;

			velocity *cow_vel = (velocity*) malloc(sizeof(velocity));
			cow_vel->y = 0;
			cow_vel->x = 0;

			move* cow_mov = (move*) malloc(sizeof(move));
			cow_mov->x = 0;
			cow_mov->y = 0;
			cow_mov->x_remainder = 0;
			cow_mov->y_remainder = 0;

			cow* generated_cow = (cow*) malloc(sizeof(cow));
			generated_cow =	new_cow(lp, cow_vel, cow_pos, cow_mov);

			insert_cow(beings, generated_cow);

			beings->beings_quantity++;
		}
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
		case _librarian:

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

	if (rand_range(0, 100) > 50) {
		rand_up = true;
	} else {

	}

	if (rand_range(0, 100) > 50) {
		rand_right = true;
	} else {
		rand_left = true;
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

	cow* travers = (cow*) malloc(sizeof(cow));
	travers = beings->header_cow->next;
	while(travers != beings->tail_cow && travers != NULL) {
		update_movables_velocity(travers->vel, travers->pos, _cow);
		update_movables_move(travers->mov, travers->vel, FPS);
		update_movables_position(travers->pos, travers->mov);
		draw_movable(cow_r_0, cow_colors_0, travers->pos, _cow);
		travers = travers->next;
	}

}
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

	beings->beings_quantity = 0;

	return beings;
}

