/*
 * guysko.h
 *
 *  Created on: 21 Feb 2023
 *      Author: gabrijel
 */

#ifndef INC_GUYSKO_H_
#define INC_GUYSKO_H_

#include <stdlib.h>
#include <stdbool.h>

#include "ugui.h"
#include "models.h"

#define RIGHT true
#define LEFT 	false

typedef struct _velocity {
	short x;
	short y;
} velocity;

/*
 * direction:
 * true		= right
 * false 	= left
 */
typedef struct _guysko {
	uint8_t life_points;

	uint8_t frame;
	bool direction;
	coord* position;
	velocity* vel;

} guysko;

guysko* new_guysko(uint8_t life_points, bool direction, coord* position);

void draw_mc ();

#endif /* INC_GUYSKO_H_ */
