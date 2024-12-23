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

#include "utils.h"
#include "structures.h"

// posx_pixel and posy_pixel declare the exect point in pixels where the movables are positioned
typedef short posx_pixel;
typedef short posy_pixel;
/*
 * Max postion in x and y axis should be the world height and width
 * That for, the position should always be: previsous position % WORLD_SIZE(x/y)
 */

typedef struct _position {
	posx_pixel	x;
	posx_pixel	y;
} pixel_position;

void update_position (pixel_position* pos, posx_pixel x, posy_pixel y, posx_pixel x_diff, posy_pixel y_diff);
void update_position_y (pixel_position* pos, posy_pixel y, posy_pixel y_diff);
void update_position_x (pixel_position* pos, posx_pixel x, posx_pixel x_diff);

void set_position (pixel_position* pos, posx_pixel x, posx_pixel y);


#endif /* INC_POSITION_H_ */
