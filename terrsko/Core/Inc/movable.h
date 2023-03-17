/*
 * movable.h
 *
 *  Created on: 15 Mar 2023
 *      Author: gabrijel
 */

#ifndef INC_MOVABLE_H_
#define INC_MOVABLE_H_

#include <stdlib.h>

#include "cow.h"
#include "librarian.h"

#include "life_points.h"
#include "acceleration.h"
#include "velocity.h"
#include "move.h"
#include "position.h"


#define	COW_INDEX		1

// TODO: index movables so that they will be randomly spawned based on index they have
// TODO: reserve memory only for the specific movable and its properties!

typedef struct _movable{

	cow*				header_cow;
	cow*				tail_cow;
	librarian* 	header_librarian;
	librarian* 	tail_librarian;

} movable;

movable* new_movable();
void insert_cow (movable* beeings, cow* krava);

#endif /* INC_MOVABLE_H_ */
