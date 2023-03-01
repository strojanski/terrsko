/*
 * materials.h
 *
 *  Created on: 14 Feb 2023
 *      Author: sebas
 */

#ifndef INC_MATERIALS_H_
#define INC_MATERIALS_H_

#include "ugui.h"
#include <stdint.h>

#define C_DARK_BROWN 0x5821//0x6000


extern uint16_t C_GRASS[4]; //= {C_GREEN, C_GREEN, C_DARK_GREEN, C_DARK_GREEN};
extern uint16_t C_DIRT[4]; //= {C_BROWN, C_BROWN, C_SADDLE_BROWN, C_BROWN};
extern uint16_t C_WOOD[4];
extern uint16_t C_RED_WOOD[4];
extern uint16_t C_ROCK[4];
extern uint16_t C_LAVA[4];
extern uint16_t C_WATER[4];

extern uint16_t C_SKY[4]; //= {C_DEEP_SKY_BLUE, C_DEEP_SKY_BLUE, C_DEEP_SKY_BLUE, C_DEEP_SKY_BLUE};
extern uint16_t C_NIGHT_SKY[4];
extern uint16_t C_BG_DIRT[4];

extern uint16_t C_STAR[4];

#endif /* INC_MATERIALS_H_ */
