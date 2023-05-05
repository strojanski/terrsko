/*
 * guysko.h
 *
 *  Created on: 21 Feb 2023
 *      Author: gabrijel
 */

#ifndef INC_GUYSKO_H_
#define INC_GUYSKO_H_

#include <stdlib.h>
#include <stdbool.h>

#include "ugui.h"
#include "action.h"
#include "scene.h"
#include "movable.h"

#include "guysko_img.h"
#include "render.h"

#include "life_points.h"
#include "velocity.h"
#include "move.h"
#include "position.h"
#include "collision.h"

#define GUYSKO_SPAWN_X 100
#define GUYSKO_SPAWN_Y 10

#define GUYSKO_JUMP_ACCELERATION 150

#define GUYSKO_MAX_UP_VELOCITY 300
#define GUYSKO_MAX_DOWN_VELOCITY -500
#define GUYSKO_MAX_RIGHT_VELOCITY	200
#define GUYSKO_MAX_LEFT_VELOCITY -200

#define GUYSKO_WALK_VEL_INC 10

#define GUYSKO_MAX_LP	1000
#define GUYSKO_HP_BAR_PIXEL_X	50
#define GUYSKO_HP_BAR_PIXEL_Y	8

#define GUYSKO_WINDOW_SPAN_PIXEL_X 75
#define GUYSKO_WINDOW_SPAN_PIXEL_Y 30

#define BUILD_CAMERA_SPEED 2	// how many blocks a click moves in building mode



/*
 * GUYSKO's speed (while walking) is at most GUYSKO_MAX_X_VELOCITY pixels per second
 * depending on game's FPS it is calculated how many pixels it will move in
 * difference of one frame.
 * The least it can move in difference of a frame is 1 one frame. But if frame
 * rate of game is high the guysko will not move in one single frame but will move after
 * more frames.
 * The speed is therefore calculated based on pixels/(real time) seconds. It then move
 * x pixels based on FPS rate.
 */

/*
 * direction:
 * true		= right
 * false 	= left
 */
extern bool in_lava;

typedef struct _guysko {
	life_points* lp;

	uint8_t state;
	velocity* vel;
	move* mov;
	pixel_position* pos;

	uint16_t standing_bits; // the pixels on which the guysko has feet on (where he stands);
	bool orientation;	// right -> true
} guysko;


void display_guysko_hp (guysko* player);
void update_guysko_hp (guysko* player, short y_diff);

void draw_guysko (guysko* player);
void update_guysko_position (guysko* player);
void update_guysko_move (guysko*, int FPS);
void update_guysko_velocity(guysko* player);

void camouflage (guysko* player, uint16_t prev_guysko_x, uint16_t prev_guysko_y);
void refresh_guysko(guysko* player, int FPS);

guysko* new_guysko();

extern bool building_mode;
extern block_t building_material;


#endif /* INC_GUYSKO_H_ */
