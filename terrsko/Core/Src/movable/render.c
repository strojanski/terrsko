/*
 * render.c
 *
 *  Created on: 12 Apr 2023
 *      Author: gabrijel
 */


// TODO: check for special cases

#include "render.h"

/*
	* PROCEDURE:
	* iterate through the give rectangle and overdraw the background where object
	* left trace
	* the parameters the function gets are give in: GLOBAL (WORLD) PIXELS
	* optimization idea: go iterate through columns and "fillFrame" for the largest
	* possible area, so that the time consuming function "UG_DRAW" is used as least
	* as possible times
*/
void overdraw_background (pixel_c x1, pixel_c y1, pixel_c x2, pixel_c y2) {
	pixel_c a = MIN(x1, x2);
	pixel_c b = MAX(x1, x2);
	pixel_c c = MIN(y1, y2);
	pixel_c d = MAX(y1, y2);

	// transform the pixels in position world to pixels in scene
	pixel_c left = w_pixel_to_scene_pixel_x(a);
	pixel_c right = w_pixel_to_scene_pixel_x(b);
	pixel_c top = w_pixel_to_scene_pixel_y(c);
	pixel_c bottom = w_pixel_to_scene_pixel_y(d);

	UG_FillFrame(left, top, right, bottom, C_BLACK);

	// iterate through every row
	for (pixel_c i = top; i <= bottom; i++) {
		for (pixel_c j = left; j <= right; j++) {
			block_t block = get_block(j, i);
			uint16_t* material_xy = map_name_to_material(block);
			UG_DrawPixel(j, i, material_xy[0]);
		}
	}
}
