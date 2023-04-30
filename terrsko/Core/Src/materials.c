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
uint16_t C_WOOD_SHADE[4] = {C_DARK_BROWN, C_DARK_BROWN, C_DARK_BROWN, C_DARK_BROWN};
uint16_t C_SAND[4] = {0xEE55, 0xEE55, 0xEE55, 0xEE55};
uint16_t C_GOLDB[4] = {0xFEA0, 0xFEA0, 0xFEA0, 0xFEA0};
uint16_t C_DIAMOND[4] = {0xAEDE, 0xAEDE, 0xAEDE, 0xAEDE};
uint16_t C_PAINT[4] = {0xA06B, 0xA06B, 0xA06B, 0xA06B};
// BACKGROUNDS
uint16_t C_SKY[4] = {0x5FF, C_DEEP_SKY_BLUE, C_DEEP_SKY_BLUE, C_DEEP_SKY_BLUE};
uint16_t C_NIGHT_SKY[4] = {0x0011, 0x0011, 0x0011, 0x0011};
uint16_t C_BG_DIRT[4] = {C_BLACK, C_BLACK, C_BLACK,C_BLACK};//{0x01AD, 0x01AD, 0x01AD, 0x01AD};	// Caves
uint16_t C_STAR[4] = {0xCDA7, 0xCDA7, 0xCDA7, 0xCDA7};

//#define COLOR_DIRT {C_BROWN, C_BROWN, C_SADDLE_BROWN, C_BROWN}
//#define COLOR_GRASS {C_GREEN, C_GREEN, C_DARK_GREEN, C_GREEN}

uint16_t* map_name_to_material (block_t block) {

	//_dirt
	if (block == _dirt) {
		return C_DIRT;
	}
	//_grass
	else if (block == _grass) {
		return C_GRASS;
	}
	//_wood
	else if (block == _wood) {
		return C_WOOD;
	}
	//_sky
	else if (block == _sky) {
		return C_SKY;
	}
	//_water
	else if (block == _water) {
		return C_WATER;
	}
	//_sand
	else if (block == _sand) {
		return C_SAND;
	}
	//_rock
	else if (block == _rock) {
		return C_ROCK;
	}
	//_lava
	else if (block == _lava) {
		return C_LAVA;
	}
	//_red_wood
	else if (block == _red_wood) {
		return C_RED_WOOD;
	}
	//_gold
	else if (block == _gold) {
		return C_GOLDB;
	}
	//_diamond
	else if (block == _diamond) {
		return C_DIAMOND;
	}
	//_dirt_bg
	else if (block == _dirt_bg) {
		return C_BG_DIRT;
	}
	else if (block == _paint) {
		return C_PAINT;
	}
	uint16_t BLANK[4] = {0x0, 0x0, 0x0, 0x0};
	return BLANK;
}

char* get_material_name(block_t block) {
	//_dirt
	if (block == _dirt) {
		return "Dirt";
	}
	//_grass
	else if (block == _grass) {
		return "Grass";
	}
	//_wood
	else if (block == _wood) {
		return "Wood";
	}
	//_sky
	else if (block == _sky) {
		return "Sky";
	}
	//_water
	else if (block == _water) {
		return "Water";
	}
	//_sand
	else if (block == _sand) {
		return "Sand";
	}
	//_rock
	else if (block == _rock) {
		return "Rock";
	}
	//_lava
	else if (block == _lava) {
		return "Lava";
	}
	//_red_wood
	else if (block == _red_wood) {
		return "Red wood";
	}
	//_gold
	else if (block == _gold) {
		return "Gold";
	}
	//_diamond
	else if (block == _diamond) {
		return 	"Diamond";
	}
	//_dirt_bg
	else if (block == _dirt_bg) {
		return "Cave";
	}

	else if (block == _paint) {
		return "Paint";
	}
}
