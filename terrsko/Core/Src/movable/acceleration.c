/*
 * acceleration.c
 *
 *  Created on: 8 Mar 2023
 *      Author: gabrijel
 */




#include "acceleration.h"


void set_acceleration(acceleration* acc, short x, short y) {
	acc->x = x;
	acc->y = y;
}
