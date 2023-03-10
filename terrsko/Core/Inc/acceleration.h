/*
 * acceleration.h
 *
 *  Created on: 8 Mar 2023
 *      Author: gabrijel
 */

#ifndef INC_ACCELERATION_H_
#define INC_ACCELERATION_H_

#include <stdbool.h>
#include "ugui.h"
#include "models.h"

#include "action.h"

#define GRAVITY										-5
#define GUYSKO_JUMP_ACCELERATION	200

typedef struct _acceleration {
	short x;
	short y;
} acceleration;

void set_acceleration (acceleration* acc, short x, short y);
void update_npc_acceleration (acceleration* acc, uint16_t pos_x, uint8_t pos_y);
void update_guysko_acceleration (acceleration* acc, uint16_t pos_x, uint8_t pos_y);

#endif /* INC_ACCELERATION_H_ */
