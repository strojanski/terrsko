#ifndef TERRSKO_MAIN_CHARACTER_H
#define TERRSKO_MAIN_CHARACTER_H

#include <stdio.h>
#include <stdlib.h>
#include "ugui.h"
/*
 * Skin is yet to be defined - might be customizable. It is defined with "skin"
 *
 * Main character can be turned in 5 different directions
 * Directions are defined with propery "direction" of type integer
 * direction [0, 4]: 	0 - face front / going down
 * 						1 - face up / going up
 * 						2 - face right / going right way
 * 						3 - face left / going left way
 * 						4 - holds still / no movement
 * direction depends on joystick input value AND action buttons (on the right)
 *
 * Main character has n (100?) lifepoints - defined with "life_points"
 * lifepoints [0, 100]
 *
 * Main character has level n
 */

typedef struct _main_character {
	uint8_t direction;
	uint8_t life_points;
	uint8_t level;

	uint16_t x_pos;
	uint16_t y_pos;
} main_character;

main_character* new_main_character(uint8_t direction, uint8_t life_points, uint8_t level, uint16_t start_x_pos, uint16_t start_y_pos);

void draw_main_character(main_character* player);
void move_main_character(main_character* player, uint8_t x_move, uint8_t y_move);

#endif
