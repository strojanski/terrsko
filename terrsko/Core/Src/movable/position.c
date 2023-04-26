/*
 * position.c
 *
 *  Created on: 8 Mar 2023
 *      Author: gabrijel
 */




#include "position.h"
#include "utils.h"
#include "scene.h"
#include "guysko.h"

/*
 * This function is meant only for moveables. It handles the overflow and size of the map
 * so that it cannot go outside of the map
 */
void update_position_x (pixel_position* pos, posx_pixel x, posx_pixel x_diff) {
	posx_pixel final_x = x + x_diff;

	if (final_x < 0) {
		final_x = WORLD_WIDTH_PIXELS - abs(final_x);
	} else if (final_x > WORLD_WIDTH_PIXELS) {
		final_x = final_x - WORLD_WIDTH_PIXELS;
	}

	set_position(pos, final_x, pos->y);
}
void update_position_y (pixel_position* pos, posy_pixel y, posy_pixel y_diff) {
	posy_pixel final_y = y + y_diff;

	if (final_y < 0) {
//		final_y = WORLD_HEIGHT_PIXELS - abs(final_y);
		final_y = 0 + GUYSKO_IMG_Y;
	} else if (final_y > WORLD_HEIGHT_PIXELS) {
		final_y = final_y - WORLD_HEIGHT_PIXELS;
	}

	set_position(pos, pos->x, final_y);
}

/* update postion of struct position: !!! it is different from coord, since it is used for
 * movables. The values represent the position in pixels.
 */
void set_position (pixel_position* pos, posx_pixel x, posy_pixel y) {
	pos->x = x;
	pos->y = y;
}


