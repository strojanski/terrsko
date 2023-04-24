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
void overdraw_background (posx_pixel x1, posx_pixel y1, posy_pixel x2, posy_pixel y2) {

//	posx_pixel a = MIN(x1, x2);
//	posx_pixel b	= MAX(x1, x2);
//	posy_pixel c = MIN(y1, y2);
//	posy_pixel d = MAX(y1, y2);
	pixel_c a = MIN(x1, x2);
	pixel_c	b = MAX(x1, x2);
	pixel_c c = MIN(y1, y2);
	pixel_c d = MAX(y1, y2);

	if (a < 0) a = 0;
	if (b < 0) b = 0;
	if (c < 0) c = 0;
	if (d < 0) d = 0;

	if (a >= WORLD_WIDTH_BLOCKS * BLOCK_WIDTH) a = WORLD_WIDTH_BLOCKS * BLOCK_WIDTH - 1;
	if (b >= WORLD_WIDTH_BLOCKS * BLOCK_WIDTH) b = WORLD_WIDTH_BLOCKS * BLOCK_WIDTH - 1;
	if (c >= WORLD_HEIGHT_BLOCKS * BLOCK_WIDTH) c = WORLD_HEIGHT_BLOCKS * BLOCK_WIDTH - 1;
	if (d >= WORLD_HEIGHT_BLOCKS * BLOCK_WIDTH) d = WORLD_HEIGHT_BLOCKS * BLOCK_WIDTH - 1;

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
//	for (pixel_c i = c; i <= d; i++) {
//		for (pixel_c j = a; j <= b; j++) {
//			block_t block = get_block_with_pixels_from_WORLD(j, i);
//			uint16_t* material_xy = map_name_to_material(block);
// 			UG_DrawPixel(j, i, material_xy[0]);
//		}
//	}
}
