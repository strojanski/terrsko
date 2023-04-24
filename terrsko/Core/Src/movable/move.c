/*
 * move.c
 *
 *  Created on: 8 Mar 2023
 *      Author: gabrijel
 */




#include "move.h"

/*
 *Getter functions for remainder
 */
float get_move_x_remainder (move* mov) {
	return mov->x_remainder;
}
float get_move_y_remainder (move* mov) {
	return mov->y_remainder;
}

// TODO: finish and then test this function, I could not finish, since I could not test!
void update_move(move* mov, short x, short y, short x_diff, short y_diff) {
	set_move(mov, x + x_diff, y + y_diff);
}

/*
 * set move of the moveable to x and y (distance in pixels the moveable will make in one frame)
 */
void set_move (move* mov, short x, short y) {
	mov->x = x;
	mov->y = y;
}


