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

void draw_movable(uint8_t* pic, uint16_t* pic_colors, uint16_t x_pos, uint8_t y_pos, uint8_t size_x, uint8_t size_y, uint16_t size) {


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
		frst_nibble =	(pic[i] & 0b11110000) >> 4;
		scnd_nibble =	(pic[i] & 0b00001111) >> 0;
		UG_DrawPixel(draw_startPoint_x + 2 * offset_x, draw_startPoint_y + offset_y, pic_colors[frst_nibble]);
		UG_DrawPixel(draw_startPoint_x + 2 * offset_x + 1, draw_startPoint_y + offset_y, pic_colors[scnd_nibble]);
	}
}

void insert_cow (movable* beeings, cow* movable_cow) {
	movable_cow->prev = beeings->tail_cow->prev;
	movable_cow->next = beeings->tail_cow;

	cow* penultimate_cow = beeings->tail_cow;
	penultimate_cow = penultimate_cow->prev;
	penultimate_cow->next = movable_cow;
	beeings->tail_cow->prev = movable_cow;
}

void remove_cow (cow* movable_cow) {
	cow* preceding_movable_cow = movable_cow->prev;
	cow* following_movable_cow = movable_cow->next;
	preceding_movable_cow->next = movable_cow->next;
	following_movable_cow->prev = movable_cow->prev;
	free(movable_cow);
}

movable* new_movable() {
	movable* beeings = (movable*)malloc(sizeof(movable));

	beeings->header_cow = (cow*)malloc(sizeof(cow));
	beeings->tail_cow	 	= (cow*)malloc(sizeof(cow));

	beeings->header_cow->next = beeings->tail_cow;
	beeings->header_cow->prev = NULL;
	beeings->tail_cow->next 	= NULL;
	beeings->tail_cow->prev 	= beeings->header_cow;

	return beeings;
}

