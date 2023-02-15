/*
 * structures.h
 *
 *  Created on: 15 Feb 2023
 *      Author: sebas
 */

#ifndef INC_STRUCTURES_H_
#define INC_STRUCTURES_H_

typedef struct {
	unsigned int val : 4;
} half_byte;


typedef struct {
	uint16_t x;
	uint8_t y;
} coord;


typedef struct {
	coord* pos;
	uint16_t colors[4];
} block;


typedef struct {
	half_byte type;		// dirt, wood ....
	block* block;
} destroyable;


typedef struct {
	half_byte type;
	block* block;
} bg_material;	// background material, can't be interacted with




#endif /* INC_STRUCTURES_H_ */
