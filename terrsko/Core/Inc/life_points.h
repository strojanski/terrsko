/*
 * life_points.h
 *
 *  Created on: 9 Mar 2023
 *      Author: gabrijel
 */

#ifndef INC_LIFE_POINTS_H_
#define INC_LIFE_POINTS_H_

#include <stdint.h>

typedef short HP;

typedef struct _life_points {
	HP life_points;
} life_points;

void update_life_points(life_points* lp, HP damage, HP heal);
void set_life_points(life_points* lp, HP points);
HP get_life_points(life_points* lp);

#endif /* INC_LIFE_POINTS_H_ */
