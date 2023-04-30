/*
 * interactions.h
 *
 *  Created on: 26 Apr 2023
 *      Author: gabrijel
 */

#ifndef INC_INTERACTIONS_H_
#define INC_INTERACTIONS_H_

#include "position.h"

void dig_down(pixel_position* pos);

void dig_left(pixel_position* pos);

void dig_up(pixel_position* pos);

void dig_right(pixel_position* pos);

void place_block(pixel_position* pos, block_t material, block_c offset_x, block_c offset_y);

void cycle_building_material();

void display_material_name();


#endif /* INC_INTERACTIONS_H_ */
