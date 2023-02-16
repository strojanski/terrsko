/*
 * structures.h
 *
 *  Created on: 15 Feb 2023
 *      Author: sebas
 */

#ifndef INC_STRUCTURES_H_
#define INC_STRUCTURES_H_

// value & 0b11110000

// 4B
typedef struct _coord {
	uint16_t x;
	uint8_t y;
} coord;

// 12B
typedef struct _block {
	coord pos; // 3B
	uint16_t colors[4]; // 6B
	//uint8_t type;
} block;

// 4B
typedef struct _destroyable {
	//uint8_t type;
	block* block;
} destroyable;

// 4B
typedef struct _bg_material {
	//uint8_t type;
	block* block;
} bg_material;	// background material, can't be interacted with


typedef struct {
	uint8_t material;
} destroyable_inventory;

#endif /* INC_STRUCTURES_H_ */
