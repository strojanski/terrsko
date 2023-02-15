#include <stdio.h>
#include <stdlib.h>

#ifndef MODELS_H
#define MODELS_H

#include "scene.h"

#define SCREEN_WIDTH 320
#define SCENE_BLOCKS_X 80

#define SCREEN_HEIGHT 240
#define SCENE_BLOCKS_Y 60

#define BLOCK_WIDTH 4

block* create_block(uint16_t x, uint8_t y, uint16_t colors[4]);

destroyable* create_destroyable(half_byte type, uint16_t x, uint8_t y, uint16_t colors[4]);

bg_material* create_bg_material(half_byte type, uint16_t x, uint8_t y, uint16_t colors[4]);

void draw_scene();

void draw_block(block* block);

void free_destroyable(destroyable* destroyable);

void free_bg_material(bg_material* mat);

void free_block(block* block);

#endif
