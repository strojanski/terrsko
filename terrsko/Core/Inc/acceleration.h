/*
 * acceleration.h
 *
 *  Created on: 8 Mar 2023
 *      Author: gabrijel
 */

#ifndef INC_ACCELERATION_H_
#define INC_ACCELERATION_H_

#include "action.h"

#define GRAVITY												-5
#define GUYSKO_JUMP_ACCELERATION			100
#define GUYSKO_MAX_DOWN_ACC						-200
#define GUYSKO_MAX_UP_ACC							200

typedef struct _acceleration {
	short x;
	short y;
} acceleration;

void set_acceleration (acceleration* acc, short x, short y);

#endif /* INC_ACCELERATION_H_ */
