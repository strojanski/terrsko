/*
 * render.h
 *
 *  Created on: 12 Apr 2023
 *      Author: gabrijel
 */

#ifndef INC_RENDER_H_
#define INC_RENDER_H_

#include "scene.h"
#include "materials.h"

void overdraw_background_rectangle (posx_pixel x1, posx_pixel y1, posy_pixel x2, posy_pixel y2);
void overdraw_background_pixel(posx_pixel x1, posx_pixel y1);


#endif /* INC_RENDER_H_ */
