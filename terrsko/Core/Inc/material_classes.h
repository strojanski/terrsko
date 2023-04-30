/*
 * material_classes.h
 *
 *  Created on: 23 Feb 2023
 *      Author: sebas
 */

#ifndef INC_MATERIAL_CLASSES_H_
#define INC_MATERIAL_CLASSES_H_

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "structures.h"

#define N_LIGHT_SOURCES 3

bool is_light_source(uint8_t material);
bool is_destructible(block_t material);
bool is_harmuful(block_t material);
bool is_solid(block_t material);

extern block_t destructible_materials[];
extern block_t solid_materials[9];

#endif /* INC_MATERIAL_CLASSES_H_ */
