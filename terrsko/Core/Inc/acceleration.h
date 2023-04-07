/*
 * acceleration.h
 *
 *  Created on: 8 Mar 2023
 *      Author: gabrijel
 */

#ifndef INC_ACCELERATION_H_
#define INC_ACCELERATION_H_

#include "action.h"



typedef struct _acceleration {
	short x;
	short y;
} acceleration;

void set_acceleration (acceleration* acc, short x, short y);

#endif /* INC_ACCELERATION_H_ */
