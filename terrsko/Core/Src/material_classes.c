/*
 * material_classes.c
 *
 *  Created on: 23 Feb 2023
 *      Author: sebas
 */


#include "enums.h"
#include "material_classes.h"

block_t light_sources[N_LIGHT_SOURCES] = {
		_lava,
		_fire,
		_sky,
		_empty		// currently sky defaults to _empty, we want to see some blocks under ground
};

block_t solid_materials[] = {
		_dirt,
		_grass,
		_wood,
		_sand,
		_rock,
		_red_wood,
		_gold,
		_diamond
};

block_t destructible_materials[] = {
		_dirt ,
		_grass ,
		_wood ,
		_sand ,
		_rock ,
		_red_wood,
		_gold,
		_diamond,
};

block_t harmuful_materials[] = {
		_lava,
		_fire
};

bool is_destructible(block_t material) {
	for (uint8_t i = 0; i < sizeof(destructible_materials) / sizeof(block_t); i++) {
		if (destructible_materials[i] == material) {
			return true;
		}
	}
	return false;
}

bool is_harmuful(block_t material) {
	for (uint8_t i = 0; i < sizeof(harmuful_materials) / sizeof(block_t); i++) {
		if (harmuful_materials[i] == material) {
			return true;
		}
	}
	return false;
}

bool is_solid(block_t material) {
	for (uint8_t i = 0; i < sizeof(solid_materials) / sizeof(block_t); i++) {
		if (solid_materials[i] == material) {
			return true;
		}
	}
	return false;
}


bool is_light_source(uint8_t material) {
	uint8_t full_cell = material << 4 | material;
	for (uint8_t i = 0; i < N_LIGHT_SOURCES; i++) {
		if (light_sources[i] == material) {
			return true;
		}

		uint8_t full_source = light_sources[i] << 4 | light_sources[i];
		if (full_source == full_cell) {
			return true;
		}

	}
	return false;
}



//void init_light_sources_array(uint8_t size) {
//	light_sources = (uint8_t) malloc(size * sizeof(uint8_t));
//}
//
//void add_item_to_light_sources(uint8_t item) {
//
//}
