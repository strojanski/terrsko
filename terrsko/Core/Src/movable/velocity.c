/*
 * velocity.c
 *
 *  Created on: 8 Mar 2023
 *      Author: gabrijel
 */




#include "velocity.h"

void set_velocity(velocity* vel, short x, short y) {
	vel->x = x;
	vel->y = y;
}

void update_npc_velocity(velocity* vel, acceleration* acc, uint16_t pos_x, uint8_t pos_y) {

}

void update_guysko_velocity(velocity* vel, acceleration* acc, uint16_t pos_x, uint8_t pos_y) {
	// x axis
	if (move_right) {
		if (vel->x < 0) set_velocity(vel, 0, vel->y);
		else set_velocity(vel, vel->x + GUYSKO_WALK_VEL_INC, vel->y);
		if (vel->x >= GUYSKO_MAX_X_VELOCITY) set_velocity(vel, GUYSKO_MAX_X_VELOCITY, vel->y);
		action_reset(MOVE_RIGHT_INDEX);
	} else if (move_left) {
		if (vel->x > 0) set_velocity(vel, 0, vel->y);
		else set_velocity(vel, vel->x - GUYSKO_WALK_VEL_INC, vel->y);
		if (vel->x <= (-1) * GUYSKO_MAX_X_VELOCITY) set_velocity(vel, (-1) * GUYSKO_MAX_X_VELOCITY, vel->y);
		action_reset(MOVE_LEFT_INDEX);
	} else {
		set_velocity(vel, 0, vel->y);
	}

	// y axis
//	if(pos_y > 220 /*Preverba ali se zaleti v solid*/) {
//		set_velocity(vel, vel->x, 0);
//	} else {
		set_velocity(vel, vel->x, vel->y + acc->y);
		if (vel->y <= GUYSKO_MAX_Y_DOWN_VELOCITY) set_velocity(vel, vel->x, GUYSKO_MAX_Y_DOWN_VELOCITY);
		else if (vel->y >= GUYSKO_MAX_Y_UP_VELOCITY) set_velocity(vel, vel->x, GUYSKO_MAX_Y_UP_VELOCITY);
//	}

	// TODO: DIFFERENCE OF PREVIOUS AND NEW VELOCITY: FOR DAMAGE OF HIGH FALL
}

