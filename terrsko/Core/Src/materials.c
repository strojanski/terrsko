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
uint16_t C_DIRT[4] =  {C_DARK_BROWN, C_DARK_BROWN, C_DARK_BROWN, C_DARK_BROWN};
uint16_t C_WOOD[4] = {0xBCCD, 0xBCCD, 0xBCCD, 0xBCCD};
uint16_t C_RED_WOOD[4] = {0xA2CA, 0xA2CA, 0xA2CA, 0xA2CA};
uint16_t C_ROCK[4] = {0x8410, 0x8410, 0x8410, 0x8410};
uint16_t C_LAVA[4] = {0xFA20, 0xFA20, 0xFA20, 0xFA20};
uint16_t C_WATER[4] = {0x1D58, 0x1D58, 0x00B4, 0x00B4};

// BACKGROUNDS
uint16_t C_SKY[4] = {0x5FF, C_DEEP_SKY_BLUE, C_DEEP_SKY_BLUE, C_DEEP_SKY_BLUE};
uint16_t C_NIGHT_SKY[4] = {0x0011, 0x0011, 0x0011, 0x0011};
uint16_t C_BG_DIRT[4] = {0x2CA3, 0x2CA3, 0x2CA3, 0x2CA3};	// Caves
uint16_t C_STAR[4] = {0xCDA7, 0xCDA7, 0xCDA7, 0xCDA7};

//#define COLOR_DIRT {C_BROWN, C_BROWN, C_SADDLE_BROWN, C_BROWN}
//#define COLOR_GRASS {C_GREEN, C_GREEN, C_DARK_GREEN, C_GREEN}
