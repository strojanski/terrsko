/*
 * cow.h
 *
 *  Created on: 16 Mar 2023
 *      Author: gabrijel
 */

#ifndef INC_COW_H_
#define INC_COW_H_

#include <stdint.h>
#include "ugui.h"

#include "life_points.h"
#include "acceleration.h"
#include "velocity.h"
#include "move.h"
#include "position.h"

#define COW_IMG_X			24
#define COW_IMG_Y			12
#define COW_IMG_SIZE	(COW_IMG_X * COW_IMG_Y)

#define COW_SPAWN_PROBABILITY 50
#define COW_MAX_LP						100

extern uint16_t cow_colors_0[];
extern uint8_t cow_r_0[];
extern uint8_t cow_l_0[];

typedef struct _cow {
	life_points* lp;

	uint8_t state;
	velocity* vel;
	position* pos;
//	move* mov

	struct cow* next;
	struct cow* prev;
} cow;

void draw_cow(cow* krava);
cow* new_cow (life_points* lp, velocity* vel, position* pos);

#endif /* INC_COW_H_ */
