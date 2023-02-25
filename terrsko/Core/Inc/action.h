/*
 * action.h
 *
 *  Created on: 24 Feb 2023
 *      Author: gabrijel
 */

#ifndef INC_ACTION_H_
#define INC_ACTION_H_

#include <stdbool.h>

extern bool act_up;
extern bool act_right;
extern bool act_down;
extern bool act_left;

extern bool ok;
extern bool esc;

extern bool move_up;
extern bool move_right;
extern bool move_down;
extern bool move_left;

typedef struct _action_signals {
} action_signals;

action_signals* new_signals();
void action_get(action_signals* sigs);


#endif /* INC_ACTION_H_ */
