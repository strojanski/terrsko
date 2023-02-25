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

action_signals* new_signals() {
	action_signals* sigs = malloc(sizeof(action_signals));
	return sigs;
}


