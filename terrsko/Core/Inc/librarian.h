/*
 * librarian.h
 *
 *  Created on: 16 Mar 2023
 *      Author: gabrijel
 */

#ifndef INC_LIBRARIAN_H_
#define INC_LIBRARIAN_H_

#include "life_points.h"
#include "velocity.h"
#include "move.h"
#include "position.h"


typedef struct _librarian {
	life_points* lp;

	uint8_t state;
	velocity* vel;
	move* mov;
	position* pos;
} librarian;




#endif /* INC_LIBRARIAN_H_ */
