/*
 * life_points.c
 *
 *  Created on: 9 Mar 2023
 *      Author: gabrijel
 */




#include "life_points.h"

/*
 * Update number of life points of movable in struct lp
 */
void set_life_points(life_points* lp, uint32_t points) {
	lp->life_points = points;
}

/*
 * Getter function to get life points of movable
 */
uint32_t get_life_points(life_points* lp) {
	return lp->life_points;
}
