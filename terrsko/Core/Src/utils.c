/*
 * util.c
 *
 *  Created on: 14 Feb 2023
 *      Author: sebas
 */

#include <stdint.h>

// Returns upper 4 bits of uint8_t
uint8_t upper(uint8_t block) {
	return (block & 0xF0) >> 4;
}

uint8_t lower(uint8_t block) {
	return block & 0x0F;
}
