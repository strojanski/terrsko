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
	* the parameters the function gets are given in: GLOBAL (WORLD) PIXELS
	* optimization idea: go iterate through columns and "fillFrame" for the largest
	* possible area, so that the time consuming function "UG_DRAW" is used as least
	* as possible times
*/
void overdraw_background_rectangle (posx_pixel x1, posx_pixel y1, posy_pixel x2, posy_pixel y2) {

	pixel_c a = MIN(x1, x2);
	pixel_c	b = MAX(x1, x2);
	pixel_c c = MIN(y1, y2);
	pixel_c d = MAX(y1, y2);

	// transform the pixels in position world to pixels in scene
	posx_pixel left = world_pixel_to_scene_pixel_x_no_band(a);
	posx_pixel right = world_pixel_to_scene_pixel_x_no_band(b);
	posy_pixel top = world_pixel_to_scene_pixel_y_no_band(c);
	posy_pixel bottom = world_pixel_to_scene_pixel_y_no_band(d);

	uint8_t index_i = 0;
	// iterate through columns
	for (posx_pixel i = top; i <= bottom; i++) {
		// iterate through rows
		uint8_t index_j = 0;
		for (posy_pixel j = left; j <= right; j++) {
			int x = a + index_j;
			int y = c + index_i;

			block_t block = get_block_with_pixels_from_WORLD(x, y);
			uint16_t* material_xy = map_name_to_material(block);
 			UG_DrawPixel(j, i, material_xy[0]);
 			index_j++;
		}
		index_i++;
	}
}

void overdraw_background_pixel(posx_pixel x1, posx_pixel y1) {

	// transform the pixels in position world to pixels in scene
	posx_pixel overdraw_x = world_pixel_to_scene_pixel_y_no_band(x1);
	posx_pixel overdraw_y = world_pixel_to_scene_pixel_y_no_band(y1);

	block_t block = get_block_with_pixels_from_WORLD(overdraw_x, overdraw_y);
	uint16_t* material_xy = map_name_to_material(block);
	UG_DrawPixel(overdraw_x, overdraw_y, material_xy[0]);
}
