/*
 * life_points.h
 *
 *  Created on: 9 Mar 2023
 *      Author: gabrijel
 */

#ifndef INC_LIFE_POINTS_H_
#define INC_LIFE_POINTS_H_

#include <stdint.h>

typedef struct _life_points {
	uint32_t life_points;
} life_points;

void set_life_points(life_points* lp, uint32_t points);
uint32_t get_life_points(life_points* lp);
#endif /* INC_LIFE_POINTS_H_ */
