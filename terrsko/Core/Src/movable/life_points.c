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
void update_life_points(life_points* lp, HP damage, HP heal) {
	set_life_points(lp, lp->life_points + damage + heal);
}

/*
 * Update number of life points of movable in struct lp
 */
void set_life_points(life_points* lp, HP points) {
	lp->life_points = points;
}

/*
 * Getter function to get life points of movable
 */
HP get_life_points(life_points* lp) {
	return lp->life_points;
}
