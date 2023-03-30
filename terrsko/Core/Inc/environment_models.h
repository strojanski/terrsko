/*
 * environment_models.h
 *
 *  Created on: 2 Mar 2023
 *      Author: sebas
 *
 *
 *  Define structs for models like trees and such
 */

#ifndef INC_ENVIRONMENT_MODELS_H_
#define INC_ENVIRONMENT_MODELS_H_

#include <stdint.h>
#include "structures.h"

#define TREE_HEIGHT 64
#define TREE_WIDTH 32

#define TREE_TALL_GREEN_HEIGHT 120
#define TREE_TALL_GREEN_WIDTH 25

#define TREE_TALL_YELLOW_HEIGHT 120
#define TREE_TALL_YELLOW_WIDTH 52


extern uint16_t tree[TREE_HEIGHT][TREE_WIDTH];
//extern uint16_t tree_tall_green[120][25];
//extern uint16_t tree_tall_yellow[120][52];


#endif /* INC_ENVIRONMENT_MODELS_H_ */
