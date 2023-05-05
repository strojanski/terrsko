/*
 * rand_range.c
 *
 *  Created on: 17 Mar 2023
 *      Author: gabrijel
 */





#include "rand_range.h"

int rand_range(int least, int max) {
	int random_number = ((rand() % (max - least + 1)) + least);
	return random_number;
}
