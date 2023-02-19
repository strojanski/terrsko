/*
 * enums.h
 *
 *  Created on: 14 Feb 2023
 *      Author: sebas
 */

#ifndef INC_ENUMS_H_
#define INC_ENUMS_H_


typedef enum {
	_empty		= 	0b0000,
	_dirt 		= 	0b0001,
	_grass 		= 	0b0010,
	_wood 		= 	0b0011,
	_brick 		= 	0b0100,
	_water 		= 	0b0101,
	_sand 		= 	0b0110,
	_rock 		= 	0b0111,
	_lava 		= 	0b1000,
	_red_wood 	= 	0b1001,
	_gold		= 	0b1010,
	_diamond 	= 	0b1011,
	_dirt_bg 	= 	0b1100,
} Material;

typedef enum {
	_sky 		= 0100001,
	_cave_bg	= 0b10011	// Might give a seperate color?
} BackgroundMaterial;
/*
 * -water
 * -wood
 * rocks (2)
 * lava
 * moon_bg
 * sand
 * brick
 * fire
 * */


#endif /* INC_ENUMS_H_ */
