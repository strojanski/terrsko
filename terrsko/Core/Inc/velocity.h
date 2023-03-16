/*
 * velocity.h
 *
 *  Created on: 8 Mar 2023
 *      Author: gabrijel
 */

#ifndef INC_VELOCITY_H_
#define INC_VELOCITY_H_

#include <stdlib.h>

#define GUYSKO_MAX_UP_VELOCITY 				150
#define GUYSKO_MAX_RIGHT_VELOCITY 		100
#define GUYSKO_MAX_DOWN_VELOCITY 			-300
#define GUYSKO_MAX_LEFT_VELOCITY 			-100

#define GUYSKO_WALK_VEL_INC						5

typedef struct _velocity {
	short x;
	short y;
} velocity;

void set_velocity(velocity* vel, short x, short y);

#endif /* INC_VELOCITY_H_ */
