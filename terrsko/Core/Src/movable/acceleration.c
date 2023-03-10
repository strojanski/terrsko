/*
 * acceleration.c
 *
 *  Created on: 8 Mar 2023
 *      Author: gabrijel
 */




#include "acceleration.h"


void set_acceleration(acceleration* acc, short x, short y) {

	acc->x = x;
	acc->y = y;
}

void update_npc_acceleration (acceleration* acc, uint16_t pos_x, uint8_t pos_y) {

}

void update_guysko_acceleration (acceleration* acc, uint16_t pos_x, uint8_t pos_y) {
	if (true /*pos_y >= 220 ce guysko nima nič "solid" pod seboj*/) {
		if (move_up) {
			set_acceleration(acc, 0, acc->y + GUYSKO_JUMP_ACCELERATION);
			action_reset(MOVE_UP_INDEX);
		} else {
			set_acceleration(acc, 0, GRAVITY);
		}
	} else {
			set_acceleration(acc, 0, acc->y + GRAVITY);
	}
}
