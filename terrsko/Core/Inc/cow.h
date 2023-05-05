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
#include "velocity.h"
#include "move.h"
#include "position.h"

#define COW_IMG_X			24
#define COW_IMG_Y			12
#define COW_IMG_SIZE	(COW_IMG_X * COW_IMG_Y)

#define COW_SPAWN_PROBABILITY 50
#define COW_MAX_LP						100

#define COW_JUMP_ACCELERATION 50

#define COW_MAX_UP_VELOCITY 200
#define COW_MAX_DOWN_VELOCITY -500
#define COW_MAX_RIGHT_VELOCITY 100
#define COW_MAX_LEFT_VELOCITY -100

#define COW_WALK_VEL_INC 10

extern uint16_t cow_colors_0[];
extern uint8_t cow_r_0[];
extern uint8_t cow_l_0[];

typedef struct cow {
	life_points* lp;

	uint8_t state;
	velocity* vel;
	pixel_position* pos;
	move* mov;

	struct cow* next;
	struct cow* prev;
} cow;

void draw_cow(cow* krava);
cow* new_cow (life_points* lp, velocity* vel, pixel_position* pos, move* mov);

#endif /* INC_COW_H_ */
