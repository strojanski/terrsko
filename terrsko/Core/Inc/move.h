/*
 * move.h
 *
 *  Created on: 8 Mar 2023
 *      Author: gabrijel
 */

#ifndef INC_MOVE_H_
#define INC_MOVE_H_

#include "action.h"

typedef struct _move {
	short x;
	short y;
	float x_remainder;
	float y_remainder;
} move;

void set_move (move* mov, short x, short y);

#endif /* INC_MOVE_H_ */
