/*
 * movable.h
 *
 *  Created on: 15 Mar 2023
 *      Author: gabrijel
 */

#ifndef INC_MOVABLE_H_
#define INC_MOVABLE_H_

#include <stdlib.h>

#include "cow.h"
#include "librarian.h"

#include "scene.h"
#include "life_points.h"
#include "velocity.h"
#include "move.h"
#include "position.h"
#include "collision.h"

#define GRAVITY -5

#define MAX_MOVABLE_CAPACTIY	1

#define LAVA_DAMAGE 100
#define FALL_DAMAGE 200

typedef struct _movable{

	cow*				header_cow;
	cow*				tail_cow;
	librarian* 	header_librarian;
	librarian* 	tail_librarian;

	uint8_t 		beings_quantity;

} movable;

typedef enum {
	_cow = 0b0000,
	_librarian = 0b0001,
} Npcs;

movable* new_movables();

void insert_movables(movable* beings);

void draw_movable(uint8_t* pic, uint16_t* pic_colors, pixel_position* pos, Npcs species);
void update_movables_velocity(velocity* vel, pixel_position* pos, uint8_t species);
void update_movables_move(move* mov, velocity* vel, uint8_t FPS);
void update_movables_position(pixel_position* pos, move* mov);
void refresh_movables(movable* beings, uint8_t FPS);

void insert_cow (movable* beings, cow* krava);
void remove_cow (cow* krava);

#endif /* INC_MOVABLE_H_ */
