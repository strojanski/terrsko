/*
 * life_points.c
 *
 *  Created on: 9 Mar 2023
 *      Author: gabrijel
 */




#include "life_points.h"

void set_life_points(life_points* lp, uint32_t points) {
	lp->life_points = points;
}

uint32_t get_life_points(life_points* lp) {
	return lp->life_points;
}
