/*
 * joystick_level_control.c
 *
 *  Created on: 16 Feb 2023
 *      Author: sebas
 */

#include "joystick_level_control.h"

coord_t joystick_raw = {0,0}, joystick_new = {0,0}, joystick_prev = {0,0};
joystick_t joystick;

