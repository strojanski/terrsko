/*
 * structures.h
 *
 *  Created on: 15 Feb 2023
 *      Author: sebas
 */

#ifndef INC_STRUCTURES_H_
#define INC_STRUCTURES_H_

// value & 0b11110000

// 3B
typedef struct _coord {
	uint16_t x;
	uint8_t y;
} coord;

// 9B
typedef struct _block {
	coord pos; // 3B
	uint16_t colors[4]; // 6B
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




#endif /* INC_STRUCTURES_H_ */
