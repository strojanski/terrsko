/*
 * move.c
 *
 *  Created on: 8 Mar 2023
 *      Author: gabrijel
 */




#include "move.h"

/*
 * Update postion of struct mov
 */
void set_move (move* mov, short x, short y) {
	mov->x = x;
	mov->y = y;
}


