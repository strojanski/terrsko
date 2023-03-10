/*
 * velocity.h
 *
 *  Created on: 8 Mar 2023
 *      Author: gabrijel
 */

#ifndef INC_VELOCITY_H_
#define INC_VELOCITY_H_

#include <stdlib.h>
#include "ugui.h"
#include "models.h"

#include "action.h"
#include "acceleration.h"

#define GUYSKO_MAX_X_VELOCITY 				200
#define GUYSKO_MAX_Y_UP_VELOCITY 			150
#define GUYSKO_MAX_Y_DOWN_VELOCITY 		-250

#define GUYSKO_WALK_VEL_INC						10

typedef struct _velocity {
	short x;
	short y;
} velocity;

void set_velocity(velocity* vel, short x, short y);
void update_npc_velocity(velocity* vel, acceleration* acc, uint16_t pos_x, uint8_t pos_y);
void update_guysko_velocity(velocity* vel, acceleration* acc, uint16_t pos_x, uint8_t pos_y);

#endif /* INC_VELOCITY_H_ */
