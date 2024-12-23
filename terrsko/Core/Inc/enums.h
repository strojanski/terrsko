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
	_sky 		= 	0b0100,
	_water 		= 	0b0101,
	_sand 		= 	0b0110,
	_rock 		= 	0b0111,
	_lava 		= 	0b1000,
	_red_wood 	= 	0b1001,
	_gold		= 	0b1010,
	_diamond 	= 	0b1011,
	_dirt_bg 	= 	0b1100,
	_fire		= 	0b1101,
	_tree		= 	0b1110,
	_paint		= 	0b1111
} Material;

//typedef enum {
//	_tree_normal 		=	 	17,
//	_tree_high_green 	= 		0b10,
//	_tree_high_yellow 	= 		0b11
//} Tree;


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
