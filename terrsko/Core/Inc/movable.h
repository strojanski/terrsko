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

#include "life_points.h"
#include "acceleration.h"
#include "velocity.h"
#include "move.h"
#include "position.h"

#define MAX_MOVABLE_CAPACTIY	10

// TODO: index movables so that they will be randomly spawned based on index they have
// TODO: reserve memory only for the specific movable and its properties!

typedef struct _movable{

	cow*				header_cow;
	cow*				tail_cow;
	librarian* 	header_librarian;
	librarian* 	tail_librarian;

} movable;

void draw_movable(uint8_t* pic, uint16_t* pic_colors, uint16_t x_pos, uint8_t y_pos, uint8_t size_x, uint8_t size_y, uint16_t size);
movable* new_movable();
void insert_cow (movable* beeings, cow* krava);
void remove_cow (cow* krava);

#endif /* INC_MOVABLE_H_ */
