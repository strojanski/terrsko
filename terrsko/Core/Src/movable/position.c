/*
 * position.c
 *
 *  Created on: 8 Mar 2023
 *      Author: gabrijel
 */




#include "position.h"
#include "utils.h"

// update postion of struct postition
void set_postition (position* pos, pixel_c x, pixel_c y) {
	pos->x = x;
	pos->y = y;

}

