/*
 * position.h
 *
 *  Created on: 8 Mar 2023
 *      Author: gabrijel
 */

#ifndef INC_POSITION_H_
#define INC_POSITION_H_

#include <stdlib.h>
#include <stdint.h>

#include "utils.h"
#include "structures.h"
/*
 * Max postion in x and y axis should be the world height and width
 * That for, the position should always be: previsous position % WORLD_SIZE(x/y)
 */

typedef struct _position {
	pixel_c	x;
	pixel_c	y;
} position;

void set_postition (position* pos, pixel_c x, pixel_c y);


#endif /* INC_POSITION_H_ */
