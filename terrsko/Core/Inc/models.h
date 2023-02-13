#include <stdio.h>
#include <stdlib.h>

#define MODELS_H

typedef struct _ground {
    int center_x;
    int center_y;
    int adjacent_pixels[100];
} ground;


ground* define_floor(int center_x, int center_y);

void free_floor(ground* floor);
