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

typedef struct _position {
	uint16_t	x;
	uint8_t		y;
} position;

void set_postition (position* pos, uint16_t x, uint8_t y);


#endif /* INC_POSITION_H_ */
