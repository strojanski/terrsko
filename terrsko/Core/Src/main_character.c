#include <stdio.h>
#include <stdlib.h>
#include "ugui.h"


typedef struct _main_character {
	uint8_t direction;
	uint8_t life_points;
	uint8_t level;

	uint16_t x_pos;
	uint16_t y_pos;
} main_character;


// function "new_main_character" initializes main character with its properties
main_character* new_main_character(	uint8_t start_direction, uint8_t start_life_points, uint8_t start_level, uint16_t start_x_pos, uint16_t start_y_pos) {
	main_character* player = malloc(sizeof(main_character));

	player->direction = start_direction;
	player->life_points = start_life_points;
	player->level = start_level;

	player->x_pos = start_x_pos;
	player->y_pos = start_y_pos;

	return player;
}

// function "draw_main_character" prints character based on it's direction and other properties
void draw_main_character(main_character* player) {
	UG_FillFrame(player->x_pos, player->y_pos, player->x_pos + 10, player->y_pos + 10, C_PURPLE);
}

// function "move_main_character" updates main_character x_pos and y_pos properties
void move_main_character(main_character* player, uint8_t x_move, uint8_t y_move) {
	player->x_pos += x_move;
	player->y_pos += y_move;
}

