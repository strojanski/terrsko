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

// TODO: index movables so that they will be randomly spawned based on index they have

// TODO: reserve memory only for the specific movable and its properties!
typedef struct _movable {

	char*				name;
	cow*				cow;
	librarian* 	librarian;

	movable*		previous;
	movable*		next;

} movable;


#endif /* INC_MOVABLE_H_ */
