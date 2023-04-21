/*
 * material_classes.c
 *
 *  Created on: 23 Feb 2023
 *      Author: sebas
 */


#include "enums.h"
#include "material_classes.h"

uint8_t light_sources[N_LIGHT_SOURCES] = {
		_lava,
		_fire,
		_sky,
		_empty		// currently sky defaults to _empty, we want to see some blocks under ground
};

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
