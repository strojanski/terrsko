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

#include "guysko_img.h"


#include "life_points.h"
#include "acceleration.h"
#include "velocity.h"
#include "move.h"
#include "position.h"




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
 * 30 FPS -> 2 p/F -> 60 p/s
 * 40 FPS -> 1 p/F -> 40 p/s
 * 50 FPS -> 1 p/F -> 50 p/s
 * 60 FPS -> 1 p/F -> 60 p/s
 * 70 FPS -> 1 p/F -> 70 p/s
 * 80 FPS -> 1 p/F -> 80 p/s
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

} guysko;

void draw_guysko ();
guysko* new_guysko(life_points* lp, uint8_t state, acceleration*acc, velocity* vel, move* mov, position* pos);


#endif /* INC_GUYSKO_H_ */
