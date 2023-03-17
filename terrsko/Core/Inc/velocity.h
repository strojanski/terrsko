/*
 * velocity.h
 *
 *  Created on: 8 Mar 2023
 *      Author: gabrijel
 */

#ifndef INC_VELOCITY_H_
#define INC_VELOCITY_H_

#include <stdlib.h>

typedef struct _velocity {
	short x;
	short y;
} velocity;

void set_velocity(velocity* vel, short x, short y);

#endif /* INC_VELOCITY_H_ */
