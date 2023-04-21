/*
 * velocity.c
 *
 *  Created on: 8 Mar 2023
 *      Author: gabrijel
 */




#include "velocity.h"


// update velocity of struct vel
void set_velocity(velocity* vel, short x, short y) {
	vel->x = x;
	vel->y = y;
}



