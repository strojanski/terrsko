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

#define MAX_MOVABLE_CAPACTIY	5

typedef struct _movable{

	cow*				header_cow;
	cow*				tail_cow;
	librarian* 	header_librarian;
	librarian* 	tail_librarian;

	uint8_t 		beings_quantity;

} movable;

void draw_movable(uint8_t* pic, uint16_t* pic_colors, uint16_t x_pos, uint16_t y_pos, uint16_t size_x, uint16_t size_y, uint16_t size);
movable* new_movables();

void insert_movables(movable* beings);
void draw_movables(movable* beings);
void update_movables(movable* beings);

void insert_cow (movable* beings, cow* krava);
void remove_cow (cow* krava);

#endif /* INC_MOVABLE_H_ */
