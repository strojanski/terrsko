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

#include "guysko_img.h"


#include "life_points.h"
#include "acceleration.h"
#include "velocity.h"
#include "move.h"
#include "position.h"

#define GUYSKO_SPAWN_X								WORLD_WIDTH/3
#define GUYSKO_SPAWN_Y								camera_y

#define GUYSKO_MAX_UP_VELOCITY 				150
#define GUYSKO_MAX_RIGHT_VELOCITY 		500
#define GUYSKO_MAX_DOWN_VELOCITY 			-300
#define GUYSKO_MAX_LEFT_VELOCITY 			-500
#define GUYSKO_WALK_VEL_INC						50
#define GUYSKO_MAX_LP									1000

#define GUYSKO_X_VISIBLE_WINDOW				115

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

typedef struct _guysko {
	life_points* lp;

	uint8_t state;
	acceleration* acc;
	velocity* vel;
	move* mov;
	position* pos;

	uint16_t standing_bits; // the pixels on which the guysko has feet on (where he stands);
} guysko;

void refresh_guysko(guysko* player, int FPS);

void draw_guysko (guysko* player);
void update_guysko_position (guysko* player);
void update_guysko_move (guysko*, int FPS);
void update_guysko_velocity(guysko* player);
void update_guysko_acceleration (guysko* player);

guysko* new_guysko();


#endif /* INC_GUYSKO_H_ */
