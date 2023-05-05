/*
 * librarian.h
 *
 *  Created on: 16 Mar 2023
 *      Author: gabrijel
 */

#ifndef INC_FASHIONIST_H_
#define INC_FASHIONIST_H_

#include <stdint.h>
#include "ugui.h"

#include "life_points.h"
#include "velocity.h"
#include "move.h"
#include "position.h"

#define FASHIONIST_IMG_X			16
#define FASHIONIST_IMG_Y			24
#define FASHIONIST_IMG_SIZE	(FASHIONIST_IMG_X * FASHIONIST_IMG_Y)

#define FASHIONIST_SPAWN_PROBABILITY 50
#define FASHIONIST_MAX_LP						100

#define FASHIONIST_JUMP_ACCELERATION 150

#define FASHIONIST_MAX_UP_VELOCITY 200
#define FASHIONIST_MAX_DOWN_VELOCITY -500
#define FASHIONIST_MAX_RIGHT_VELOCITY 100
#define FASHIONIST_MAX_LEFT_VELOCITY -100

#define FASHIONIST_WALK_VEL_INC 10

extern uint16_t fashionist_colors_0[];
extern uint8_t fashionist_r_0[];
extern uint8_t fashionist_l_0[];

typedef struct fashionist{
	life_points* lp;

	uint8_t state;
	velocity* vel;
	move* mov;
	pixel_position* pos;

	struct fashionist* next;
	struct fashionist* prev;
} fashionist;

#endif /* INC_FASHIONIST_H_ */

fashionist* new_fashionist (life_points* lp, velocity* vel, pixel_position* pos, move* mov);

