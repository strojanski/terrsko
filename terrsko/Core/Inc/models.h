#include <stdio.h>
#include <stdlib.h>

#ifndef MODELS_H
#define MODELS_H

#include "scene.h"

#define SCREEN_WIDTH 320
#define SCENE_BLOCKS_X 40

#define SCREEN_HEIGHT 240
#define SCENE_BLOCKS_Y 60

#define BLOCK_WIDTH 4

void rgb565_to_hsl(uint16_t color, float *h, float *s, float *l);

uint16_t hsl_to_rgb565(float h, float s, float l);

uint16_t* apply_shading(uint16_t colors[4], float illumination);

block* create_block(uint16_t x, uint16_t y, uint16_t colors[4], uint8_t type, float illumination);

destroyable* create_destroyable(uint16_t x, uint16_t y, uint16_t colors[4], uint8_t type, float illumination);

bg_material* create_bg_material(uint16_t x, uint16_t y, uint16_t colors[4], uint8_t type, float illumination);

void draw_scene();

void draw_block(block* block);

void draw_detailed_block(block* block);

void free_destroyable(destroyable* destroyable);

void free_bg_material(bg_material* mat);

void free_block(block* block);

#endif
