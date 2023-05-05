/*
 * movable.h
 *
 *  Created on: 15 Mar 2023
 *      Author: gabrijel
 */

#ifndef INC_MOVABLE_H_
#define INC_MOVABLE_H_

#include <stdlib.h>
#include <time.h>

#include "cow.h"
#include "scene.h"
#include "life_points.h"
#include "velocity.h"
#include "move.h"
#include "position.h"
#include "collision.h"
#include "fashionist.h"
#include "miner.h"
#include "merchant.h"

#define GRAVITY -5

#define MAX_MOVABLE_CAPACTIY	4
#define MOVABLE_DIFFERENT_SPECIES 4 // cow, fash, miner, merchant

#define LAVA_DAMAGE 100
#define FALL_DAMAGE 200

typedef struct _movable{

	cow* header_cow;
	cow* tail_cow;

	fashionist* header_fashionist;
	fashionist* tail_fashionist;

	miner* header_miner;
	miner* tail_miner;

	merchant*	header_merchant;
	merchant* tail_merchant;

	uint8_t beings_quantity;

} movable;

typedef enum {
	_cow = 0b0000,
	_fashionist = 0b0001,
	_miner = 0b0010,
	_merchant = 0b0011
} Npcs;

movable* new_movables();

void insert_movables(movable* beings);

void camouflage_movable (pixel_position* pos, uint16_t prev_movable_x, uint16_t prev_movable_y, Npcs species);
void draw_movable(uint8_t* pic, uint16_t* pic_colors, pixel_position* pos, Npcs species);
void update_movables_velocity(velocity* vel, pixel_position* pos, uint8_t species);
void update_movables_move(move* mov, velocity* vel, uint8_t FPS);
void update_movables_position(pixel_position* pos, move* mov);
void refresh_movables(movable* beings, uint8_t FPS);

void insert_fashionist(movable* beings, fashionist* fash);
void insert_miner(movable* beings, miner* min);
void insert_merchant(movable* beings, merchant* merchant);
void insert_cow (movable* beings, cow* krava);
void remove_cow (cow* krava);

#endif /* INC_MOVABLE_H_ */
