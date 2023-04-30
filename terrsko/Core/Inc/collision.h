/*
 * collision.h
 *
 *  Created on: 26 Apr 2023
 *      Author: gabrijel
 */

#ifndef INC_COLLISION_H_
#define INC_COLLISION_H_

#include <stdbool.h>

typedef struct {
	block_c x;
	block_c y;
} block_pos;


enum direction {
	_up,
	_right,
	_down,
	_left
};

enum property {
	_destructible,
	_harmful,
	_solid
};


block_pos get_colliding_block_down(pixel_position* pos);

bool check_blocks_down(uint8_t property, pixel_position* pos, uint8_t movable_width);

bool collision(uint8_t property, uint8_t direction, pixel_position* pos, uint8_t movable_width, uint8_t movable_height);

#endif /* INC_COLLISION_H_ */
