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
#include "models.h"
#include "action.h"

#define RIGHT true
#define LEFT 	false

#define AIR_RESISTANCE		10
#define GRAVITY						30
#define WALK_ACCELERATION	30
#define JUMP_ACCELERATION	30
#define MAX_X_VELOCITY 		60
#define MAX_Y_VELOCITY 		100


/*
 * GUYSKO's speed (while walking) is at most 60 pixels per second
 * depending on game's FPS it is calculated how many pixels it will move
 * after one frame
 * The least it can move in difference of a frame is 1 pixel which means that
 * the remainder of division will not play any role. Therefore some framerate
 * settings will differ in an acutaul speed of guysko. 60FPS and 70FPS settings
 * will have different speed of guysko: 60p/s and 70p/s respectevley.
 * Another solution would be to move it one more pixel every n frames, but it may
 * turn out as a bloat.
 * p/F === pixels per frame
 * p/s === pixels per second
 *
 * 						.
 * 						.
 * 30 FPS -> 2 p/F -> 60 p/s
 * 40 FPS -> 1 p/F -> 40 p/s
 * 50 FPS -> 1 p/F -> 50 p/s
 * 60 FPS -> 1 p/F -> 60 p/s
 * 70 FPS -> 1 p/F -> 70 p/s
 * 80 FPS -> 1 p/F -> 80 p/s
 * 						.
 * 						.
 */
typedef struct _velocity {
	float x;
	float y;
} velocity;

/*
 * direction:
 * true		= right
 * false 	= left
 */

typedef struct _guysko {
	uint8_t life_points;

	float x_remainder;
	float y_remainder;
	bool direction;
	coord* position;
	velocity* vel;

} guysko;

velocity* set_velocity(guysko* player, float x, float y);
void update_guysko_moving (guysko* player);
void update_guysko_position (guysko* player, uint8_t FPS);
void draw_guysko ();
guysko* new_guysko(uint8_t life_points, bool direction, coord* position);

#endif /* INC_GUYSKO_H_ */
