/*
 * movable.c
 *
 *  Created on: 15 Mar 2023
 *      Author: gabrijel
 */




/*
 * kako se med seboj razlikujejo npc-ji (drugače):
 * se premikajo:
 * 	speed
 * 	algoritem
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
void draw_movable(uint8_t* pic, uint16_t* pic_colors, uint16_t x_pos, uint16_t y_pos, uint16_t size_x, uint16_t size_y, uint16_t size) {

	int index = 0;
	uint16_t draw_startPoint_x 	= x_pos - (size_x / 2);
	uint8_t draw_startPoint_y 	= y_pos - size_y;
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
		UG_DrawPixel(draw_startPoint_x + 2 * offset_x, draw_startPoint_y + offset_y, pic_colors[frst_nibble]);
		UG_DrawPixel(draw_startPoint_x + 2 * offset_x + 1, draw_startPoint_y + offset_y, pic_colors[scnd_nibble]);
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
	if (20 < COW_SPAWN_PROBABILITY && beings->beings_quantity < MAX_MOVABLE_CAPACTIY) {
		life_points *lp = malloc(sizeof(life_points));
		lp->life_points = COW_MAX_LP;
		position* cow_pos = malloc(sizeof(position));
		cow_pos->x = ((beings->beings_quantity * 40) + 1) % 320;
		cow_pos->y = 150;
		velocity *cow_vel = malloc(sizeof(velocity));
		cow_vel->y = 0;
		cow_vel->x = 0;
		cow* generated_cow = (cow*) malloc(sizeof(cow));
		generated_cow =	new_cow(lp, cow_vel, cow_pos);
		insert_cow(beings, generated_cow);
		beings->beings_quantity++;
	}
}

/*
 * TODO: comment
 */
void draw_movables(movable* beings) {
	// cows
	cow* travers = (cow*) malloc(sizeof(cow));
	travers = beings->header_cow->next;
	while(travers != beings->tail_cow && travers != NULL) {
		draw_movable(cow_r_0, cow_colors_0, travers->pos->x, travers->pos->y, COW_IMG_X, COW_IMG_Y, COW_IMG_SIZE);
		travers = travers->next;
	}
}

void update_movables(movable* beings) {


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

