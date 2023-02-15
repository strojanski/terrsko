/*
 * materials.c
 *
 *  Created on: 14 Feb 2023
 *      Author: sebas
 */
#include <string.h>
#include <stdlib.h>
#include <stdint.h>


#include "ugui.h"
#include "enums.h"
#include "materials.h"

// MATERIALS
uint16_t C_GRASS[4] = {C_GREEN, C_GREEN, C_DARK_GREEN, C_DARK_GREEN};
uint16_t C_DIRT[4] = {C_BROWN, C_BROWN, C_SADDLE_BROWN, C_BROWN};
uint16_t C_WOOD[4] = {0xBCCD, 0xBCCD, 0xBCCD, 0xBCCD};
uint16_t C_RED_WOOD[4] = {0xA2CA, 0xA2CA, 0xA2CA, 0xA2CA};
uint16_t C_ROCK[4] = { 0x8410, 0xBDF7, 0x8410, 0x8410};



// BACKGROUNDS
uint16_t C_SKY[4] = {C_DEEP_SKY_BLUE, C_DEEP_SKY_BLUE, C_DEEP_SKY_BLUE, C_DEEP_SKY_BLUE};
uint16_t C_BG_DIRT[4] = {C_DARK_BROWN, C_DARK_BROWN, C_DARK_BROWN, C_DARK_BROWN};

//#define COLOR_DIRT {C_BROWN, C_BROWN, C_SADDLE_BROWN, C_BROWN}
//#define COLOR_GRASS {C_GREEN, C_GREEN, C_DARK_GREEN, C_GREEN}

