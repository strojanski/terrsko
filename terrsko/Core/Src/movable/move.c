/*
 * move.c
 *
 *  Created on: 8 Mar 2023
 *      Author: gabrijel
 */




#include "move.h"

void set_move (move* mov, short x, short y) {
	mov->x = x;
	mov->y = y;
}

void update_npc_move () {

}

void update_guysko_move (move* mov, velocity* vel, int FPS) {
	float mov_x = ((float)1 / ((float) (1000 / FPS) / (float)vel->x)) + mov->x_remainder;
	float mov_y = ((float)1 / ((float) (1000 / FPS) / (float)vel->y)) + mov->y_remainder;

	short pix_move_x = (short) mov_x % 100;
	short pix_move_y = (short) mov_y % 100;

	mov->x_remainder = mov_x - (float)pix_move_x;
	mov->y_remainder = mov_y - (float)pix_move_y;

	set_move(mov, pix_move_x, pix_move_y);
}
