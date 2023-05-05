/*
 * merchant.h
 *
 *  Created on: 5 May 2023
 *      Author: gabrijel
 */

#ifndef INC_MERCHANT_H_
#define INC_MERCHANT_H_

#include <stdint.h>
#include "ugui.h"

#include "life_points.h"
#include "velocity.h"
#include "move.h"
#include "position.h"

#define MERCHANT_IMG_X			16
#define MERCHANT_IMG_Y			28
#define MERCHANT_IMG_SIZE	(MERCHANT_IMG_X * MERCHANT_IMG_Y)

#define MERCHANT_SPAWN_PROBABILITY 50
#define MERCHANT_MAX_LP						100

#define MERCHANT_JUMP_ACCELERATION 20

#define MERCHANT_MAX_UP_VELOCITY 200
#define MERCHANT_MAX_DOWN_VELOCITY -500
#define MERCHANT_MAX_RIGHT_VELOCITY 100
#define MERCHANT_MAX_LEFT_VELOCITY -100

#define MERCHANT_WALK_VEL_INC 20

extern uint16_t merchant_colors_0[];
extern uint8_t merchant_r_0[];
extern uint8_t merchant_l_0[];

typedef struct merchant{
	life_points* lp;

	uint8_t state;
	velocity* vel;
	move* mov;
	pixel_position* pos;

	struct merchant* next;
	struct merchant* prev;
} merchant;

merchant* new_merchant (life_points* lp, velocity* vel, pixel_position* pos, move* mov);

#endif /* INC_MERCHANT_H_ */
