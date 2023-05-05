/*
 * miner.h
 *
 *  Created on: 5 May 2023
 *      Author: gabrijel
 */

#ifndef INC_MINER_H_
#define INC_MINER_H_

#include <stdint.h>
#include "ugui.h"

#include "life_points.h"
#include "velocity.h"
#include "move.h"
#include "position.h"

#define MINER_IMG_X			20
#define MINER_IMG_Y			20
#define MINER_IMG_SIZE	(MINER_IMG_X * MINER_IMG_Y)

#define MINER_SPAWN_PROBABILITY 50
#define MINER_MAX_LP						100

#define MINER_JUMP_ACCELERATION 50

#define MINER_MAX_UP_VELOCITY 200
#define MINER_MAX_DOWN_VELOCITY -500
#define MINER_MAX_RIGHT_VELOCITY 100
#define MINER_MAX_LEFT_VELOCITY -100

#define MINER_WALK_VEL_INC 5

extern uint16_t miner_colors_0[];
extern uint8_t miner_r_0[];
extern uint8_t miner_l_0[];

typedef struct miner{
	life_points* lp;

	uint8_t state;
	velocity* vel;
	move* mov;
	pixel_position* pos;

	struct miner* next;
	struct miner* prev;
} miner;

miner* new_miner (life_points* lp, velocity* vel, pixel_position* pos, move* mov);

#endif /* INC_MINER_H_ */
