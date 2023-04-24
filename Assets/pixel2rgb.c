#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>


/*
This program serves as encoder of RGB565 images. It is meant to encode the structure named figure, example below
*/

int figure[] = {
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x42,0x28,0x00,0x00,0x00,0x00,0x42,0x28,0x00,0x00,0x42,0x28,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x42,0x28,0xaf,0x23,0x42,0x28,0x42,0x28,0xaf,0x23,0x42,0x28,0x42,0x28,0x42,0x28,0x42,0x28,0x42,0x28,0x00,0x00
,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x42,0x28,0xaf,0x23,0xaf,0x23,0xaf,0x23,0xaf,0x23,0xaf,0x23,0xaf,0x23,0xaf,0x23,0xaf,0x23,0xaf,0x23,0xaf,0x23,0x42,0x28
,0x00,0x00,0x00,0x00,0x42,0x28,0x42,0x28,0xaf,0x23,0xaf,0x23,0xaf,0x23,0xaf,0x23,0xaf,0x23,0xaf,0x23,0xaf,0x23,0xaf,0x23,0xaf,0x23,0xaf,0x23,0x42,0x28,0x00,0x00
,0x00,0x00,0x42,0x28,0xaf,0x23,0xaf,0x23,0xaf,0x23,0xaf,0x23,0xaf,0x23,0xaf,0x23,0xaf,0x23,0xaf,0x23,0xaf,0x23,0xaf,0x23,0xaf,0x23,0xaf,0x23,0xaf,0x23,0x42,0x28
,0x00,0x00,0x00,0x00,0x42,0x28,0xaf,0x23,0xaf,0x23,0xaf,0x23,0xaf,0x23,0xaf,0x23,0xaf,0x23,0xaf,0x23,0xaf,0x23,0x42,0x28,0xaf,0x23,0xaf,0x23,0xaf,0x23,0x42,0x28
,0x00,0x00,0x00,0x00,0x00,0x00,0x42,0x28,0xaf,0x23,0xaf,0x23,0xaf,0x23,0x42,0x28,0x42,0x28,0x42,0x28,0x42,0x28,0xe6,0x50,0x42,0x28,0x42,0x28,0xaf,0x23,0x42,0x28
,0x00,0x00,0x00,0x00,0x42,0x28,0xaf,0x23,0xaf,0x23,0xaf,0x23,0x42,0x28,0xe6,0x50,0xe6,0x50,0xff,0xff,0x42,0x28,0xe6,0x50,0x42,0x28,0x00,0x00,0x42,0x28,0x00,0x00
,0x00,0x00,0x00,0x00,0x00,0x00,0x42,0x28,0x42,0x28,0xaf,0x23,0x42,0x28,0xe6,0x50,0xe6,0x50,0xff,0xff,0x42,0x28,0xe6,0x50,0x42,0x28,0x00,0x00,0x00,0x00,0x00,0x00
,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x42,0x28,0x42,0x28,0x42,0x28,0xe6,0x50,0xe6,0x50,0xe6,0x50,0xe6,0x50,0xe6,0x50,0xe6,0x50,0x42,0x28,0x00,0x00,0x00,0x00
,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x42,0x28,0x74,0xda,0x74,0xda,0x42,0x28,0xe6,0x50,0xe6,0x50,0xe6,0x50,0xe6,0x50,0x42,0x28,0x00,0x00,0x00,0x00,0x00,0x00
,0x00,0x00,0x00,0x00,0x00,0x00,0x42,0x28,0x74,0xda,0x74,0xda,0x74,0xda,0x74,0xda,0x42,0x28,0x42,0x28,0x42,0x28,0x42,0x28,0x29,0xb3,0x42,0x28,0x00,0x00,0x00,0x00
,0x00,0x00,0x00,0x00,0x00,0x00,0x42,0x28,0x74,0xda,0x74,0xda,0x42,0x28,0x29,0xb3,0x29,0xb3,0x29,0xb3,0x29,0xb3,0x29,0xb3,0x29,0xb3,0x42,0x28,0x00,0x00,0x00,0x00
,0x00,0x00,0x00,0x00,0x00,0x00,0x42,0x28,0x74,0xda,0x74,0xda,0x42,0x28,0x29,0xb3,0x29,0xb3,0x29,0xb3,0x29,0xb3,0x29,0xb3,0x29,0xb3,0x42,0x28,0x00,0x00,0x00,0x00
,0x00,0x00,0x00,0x00,0x00,0x00,0x42,0x28,0x74,0xda,0x74,0xda,0x42,0x28,0x29,0xb3,0x29,0xb3,0x29,0xb3,0x29,0xb3,0x29,0xb3,0x29,0xb3,0x42,0x28,0x00,0x00,0x00,0x00
,0x00,0x00,0x00,0x00,0x00,0x00,0x42,0x28,0xe6,0x50,0xe6,0x50,0xe6,0x50,0x42,0x28,0x29,0xb3,0x29,0xb3,0x29,0xb3,0x29,0xb3,0x42,0x28,0xe6,0x50,0x42,0x28,0x00,0x00
,0x00,0x00,0x00,0x00,0x00,0x00,0x42,0x28,0xe6,0x50,0xe6,0x50,0xe6,0x50,0x42,0x28,0x42,0x28,0x42,0x28,0x29,0xb3,0x42,0x28,0x42,0x28,0xe6,0x50,0x42,0x28,0x00,0x00
,0x00,0x00,0x00,0x00,0x00,0x00,0x42,0x28,0xe6,0x50,0xe6,0x50,0xe6,0x50,0x42,0x28,0x29,0xb3,0x29,0xb3,0x29,0xb3,0x29,0xb3,0x42,0x28,0xe6,0x50,0x42,0x28,0x00,0x00
,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x42,0x28,0x42,0x28,0x42,0x28,0xd3,0xc7,0xd3,0xc7,0xd3,0xc7,0x42,0x28,0xd3,0xc7,0x42,0x28,0x42,0x28,0x00,0x00,0x00,0x00
,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x42,0x28,0xd3,0xc7,0xd3,0xc7,0xd3,0xc7,0x42,0x28,0xd3,0xc7,0x42,0x28,0x00,0x00,0x00,0x00,0x00,0x00
,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x42,0x28,0x84,0x30,0xd3,0xc7,0xd3,0xc7,0xd3,0xc7,0x42,0x28,0xd3,0xc7,0x42,0x28,0x00,0x00,0x00,0x00,0x00,0x00
,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x42,0x28,0x84,0x30,0xd3,0xc7,0xd3,0xc7,0x9a,0xc7,0x42,0x28,0x9a,0xc7,0x42,0x28,0x00,0x00,0x00,0x00,0x00,0x00
,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x42,0x28,0x84,0x30,0x84,0x30,0x9a,0xc7,0x84,0x30,0x84,0x30,0x42,0x28,0x84,0x30,0x42,0x28,0x00,0x00,0x00,0x00
,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x42,0x28,0x42,0x28,0x42,0x28,0x42,0x28,0x42,0x28,0x42,0x28,0x42,0x28,0x42,0x28,0x42,0x28,0x00,0x00,0x00,0x00


};


#define MAX_COLORS 	16
#define WIDTH		16
#define HEIGHT		24

int main () {
	int filled 	= 0;
	int compresssed[MAX_COLORS];
	int pre_zipped[WIDTH * HEIGHT];
	int pre_zipped_mirror[WIDTH * HEIGHT];
	int figure_final[WIDTH * HEIGHT];
	int index = 0;

	for (int i = 0; i < WIDTH * HEIGHT * 2; i = i + 2) {
		figure_final[index] = figure[i] * 256 + figure[i + 1];
		index++;
	}

	bool too_many_colors = false;
	index = 0;
	for (int i = 0; i < WIDTH * HEIGHT; i++) {
		bool used = false;
		for (int j = 0; j < filled; j++) {
			if (compresssed[j] == figure_final[i]) {
				used = true;
				pre_zipped[index] = j;
				break;
			} 
		}
		if (!used) {
			compresssed[filled] = figure_final[i];			
			pre_zipped[index] = filled;
			filled++;
			if (filled >= MAX_COLORS) {
				too_many_colors = true;
			}
		}
		index++;
	}
	if (too_many_colors) {
		printf("too many different colors! : %d\n", filled);
		exit(1);
	}


	int zipped[(WIDTH * HEIGHT) / 2];
	index = 0;
	for (int i = 0; i < WIDTH * HEIGHT; i = i + 2) {
		zipped[index] = pre_zipped[i] * 16 + pre_zipped[i + 1];
		index++;
	}

	printf("\n");
	for (int i = 0; i < WIDTH * HEIGHT / 2; i++) {
		printf("%d, ", zipped[i]);
		if ((i + 1) % (WIDTH / 2) == 0) printf("\n");
	}
	printf("\n");

	printf("uint16_t FIGURE[] = {\n");
	for (int i = 0; i < filled; i++) {
		if (i != 0) printf(", ");
		printf("0x%x", compresssed[i]);
	}
	printf("\n};\n");

	for (int i = 0; i < WIDTH * HEIGHT; i++) {
		pre_zipped_mirror[i] = pre_zipped[((i / WIDTH) * WIDTH) + ((WIDTH - 1) - (i % WIDTH))];
	} 

	int zipped_mirror[(WIDTH * HEIGHT) / 2];
	index = 0;
	for (int i = 0; i < WIDTH * HEIGHT; i = i + 2) {
		zipped_mirror[index] = pre_zipped_mirror[i] * 16 + pre_zipped_mirror[i + 1];
		index++;
	}

	printf("mirrored: \n");
	for (int i = 0; i < WIDTH * HEIGHT / 2; i++) {
		printf("%d, ", zipped_mirror[i]);
		if ((i + 1) % (WIDTH / 2) == 0) printf("\n");
	}
	printf("\n");


	return 0;
}
