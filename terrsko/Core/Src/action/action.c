/*
 * action.c
 *
 *  Created on: 24 Feb 2023
 *      Author: gabrijel
 */




#include <stdbool.h>
#include "action.h"

bool act_up			=	false;
bool act_right	=	false;
bool act_down		=	false;
bool act_left		=	false;

bool ok					=	false;
bool esc				=	false;

bool move_up		=	false;
bool move_right	=	false;
bool move_down	=	false;
bool move_left	=	false;

void action_reset(uint8_t action_index) {
	if (action_index == 0) {
		act_up			=	false;
	} else if (action_index == 1) {
		act_right		=	false;
	} else if (action_index == 2) {
		act_down		=	false;
	} else if (action_index == 3) {
		act_left		=	false;
	} else if (action_index == 4) {
		ok					=	false;
	} else if (action_index == 5) {
		esc					=	false;
	} else if (action_index == 6) {
		move_up			= false;
	} else if (action_index == 7) {
		move_right 	= false;
	} else if (action_index == 8) {
		move_down		= false;
	} else if (action_index == 9) {
		move_left		= false;
	}
}

/*
 * This funcion updates joystick input logicaly
 * the other buttons are set to be handled as interrupts
 */
void action_set(coord_t* joystick_raw) {
	if (joystick_raw->x > 2500) {
		move_right 	= true;
		move_left		= false;
	} else if (joystick_raw->x < 1000) {
		move_right 	= false;
		move_left		= true;
	}

	if (joystick_raw->y > 2500) {
		move_up			= true;
		move_down		= false;
	} else if (joystick_raw->y < 1000) {
		move_up			= false;
		move_down		= true;
	}
}


