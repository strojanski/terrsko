/*
 * action.h
 *
 *  Created on: 24 Feb 2023
 *      Author: gabrijel
 */

#ifndef INC_ACTION_H_
#define INC_ACTION_H_

#include <stdbool.h>
#include "joystick.h"

/*
 * Each action is indexed so that it can be quickly
 * reset after it is executed. From 0 to 9 respectively
 */

#define ACT_UP_INDEX 			0
#define ACT_RIGHT_INDEX 	1
#define ACT_DOWN_INDEX 		2
#define ACT_LEFT_INDEX 		3
#define OK_INDEX 					4
#define ESC_INDEX 				5
#define MOVE_UP_INDEX 		6
#define MOVE_RIGHT_INDEX	7
#define MOVE_DOWN_INDEX 	8
#define MOVE_LEFT_INDEX 	9
#define MOVE_ENTER 				10

extern bool act_up;			// 0
extern bool act_right;	// 1
extern bool act_down;		// 2
extern bool act_left;		// 3

extern bool ok;					// 4
extern bool esc;				// 5

extern bool move_up;		// 6
extern bool move_right;	// 7
extern bool move_down;	// 8
extern bool move_left;	// 9
extern bool move_enter;	// 10

void action_set(coord_t* joystick_raw);
void action_reset(uint8_t action_index);

#endif /* INC_ACTION_H_ */
