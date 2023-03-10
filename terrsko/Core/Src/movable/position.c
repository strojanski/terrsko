/*
 * position.c
 *
 *  Created on: 8 Mar 2023
 *      Author: gabrijel
 */




#include "position.h"

void set_postition (position* pos, uint16_t x, uint8_t y) {
	pos->x = x;
	pos->y = y;

}

void update_guysko_position (position* pos, move* mov) {
	set_postition(pos, pos->x + mov->x, pos->y - mov->y);
//	set_postition(pos, 100, 50);
}
