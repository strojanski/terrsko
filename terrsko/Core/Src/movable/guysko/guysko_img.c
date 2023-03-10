/*
 * guysko_img.c
 *
 *  Created on: 24 Feb 2023
 *      Author: gabrijel
 */


#include "guysko_img.h"


uint16_t GUYSKO_R_0[] = {
	0x0, 0x2a3, 0x7ea, 0xfffa, 0xffff, 0x3983, 0xccad, 0xdb1e, 0x29b3, 0xb387, 0x1f, 0x9edd, 0x4228, 0x632e
};

uint8_t guysko_r_0[] = {
		0, 0, 0, 0, 0, 0,
		0, 1, 1, 1, 17, 0,
		0, 17, 34, 17, 33, 16,
		1, 18, 33, 34, 34, 1,
		18, 34, 34, 34, 2, 16,
		2, 34, 2, 0, 48, 16,
		1, 32, 48, 69, 48, 0,
		0, 16, 51, 69, 48, 0,
		0, 0, 99, 53, 51, 0,
		0, 119, 6, 51, 48, 0,
		7, 119, 112, 0, 7, 128,
		7, 112, 119, 119, 119, 128,
		7, 112, 135, 119, 119, 128,
		7, 112, 135, 119, 120, 0,
		9, 147, 8, 119, 128, 48,
		3, 51, 8, 136, 128, 48,
		3, 51, 8, 136, 128, 48,
		0, 0, 171, 0, 176, 0,
		0, 0, 171, 0, 176, 0,
		0, 10, 187, 176, 176, 0,
		0, 10, 187, 192, 192, 0,
		0, 10, 172, 205, 13, 0,
		0, 0, 0, 0, 0, 0,
};

uint16_t GUYSKO_R_1[] = {
		0x0, 0x7320, 0xb520, 0xff80, 0xe54f, 0xffff, 0x3983, 0x9ac7, 0xccad, 0x4b7e, 0x29b3, 0x5bd, 0xaedc, 0x8430
};

uint8_t guysko_r_1[] = {
		0, 0, 1, 0, 0, 0,
		0, 2, 2, 2, 34, 0,
		0, 34, 51, 34, 50, 32,
		2, 35, 50, 51, 51, 2,
		35, 51, 51, 51, 3, 32,
		3, 51, 3, 0, 64, 32,
		2, 48, 64, 86, 71, 0,
		0, 32, 68, 86, 71, 0,
		0, 7, 132, 70, 68, 112,
		0, 7, 120, 68, 71, 0,
		0, 9, 151, 119, 121, 0,
		0, 153, 153, 153, 153, 0,
		9, 151, 121, 153, 151, 112,
		9, 148, 71, 153, 148, 71,
		9, 148, 71, 153, 148, 71,
		0, 7, 122, 153, 151, 112,
		0, 0, 170, 170, 160, 0,
		0, 0, 11, 11, 204, 0,
		0, 0, 11, 11, 204, 0,
		0, 0, 176, 188, 204, 0,
		0, 0, 176, 188, 205, 0,
		0, 0, 176, 187, 221, 208,
		0, 0, 0, 0, 0, 0
};

uint16_t GUYSKO_R_2[] = {
		0x0, 0xb520, 0x20, 0xe54f, 0xffff, 0x3983, 0x9ac7, 0xccad, 0x4b7e, 0x29b3, 0x9edd, 0xaedc, 0x1c54, 0x5bd, 0x4228
};

uint8_t guysko_r_2[] = {
		0, 0, 0, 0, 0, 0,
		0, 1, 1, 1, 17, 0,
		0, 17, 17, 17, 17, 16,
		1, 17, 17, 17, 17, 1,
		33, 17, 17, 17, 1, 16,
		1, 17, 1, 0, 48, 16,
		1, 16, 48, 69, 54, 0,
		0, 16, 51, 69, 54, 0,
		0, 6, 115, 53, 51, 96,
		0, 136, 103, 51, 54, 0,
		8, 136, 134, 102, 104, 144,
		8, 136, 136, 136, 136, 144,
		8, 136, 102, 104, 137, 144,
		8, 136, 51, 118, 137, 96,
		0, 136, 51, 118, 137, 48,
		0, 0, 102, 104, 137, 48,
		0, 8, 153, 153, 153, 0,
		0, 0, 10, 186, 160, 0,
		0, 0, 192, 186, 160, 0,
		0, 12, 170, 221, 172, 0,
		0, 170, 224, 13, 204, 160,
		0, 206, 238, 0, 0, 0,
		0, 0, 0, 0, 0, 0
};

uint16_t GUYSKO_L_0[] = {
		0x0, 0x2a3, 0x7ea, 0xfffa, 0xffff, 0x3983, 0xccad, 0xdb1e, 0x29b3, 0xb387, 0x1f, 0x9edd, 0x4228, 0x632e
};


uint8_t guysko_l_0[] = {
		0, 0, 0, 0, 0, 0,
		0, 17, 16, 16, 16, 0,
		1, 18, 17, 34, 17, 0,
		16, 34, 34, 18, 33, 16,
		1, 32, 34, 34, 34, 33,
		1, 3, 0, 32, 34, 32,
		0, 3, 84, 3, 2, 16,
		0, 3, 84, 51, 1, 0,
		0, 51, 83, 54, 0, 0,
		0, 3, 51, 96, 119, 0,
		8, 112, 0, 7, 119, 112,
		8, 119, 119, 119, 7, 112,
		8, 119, 119, 120, 7, 112,
		0, 135, 119, 120, 7, 112,
		3, 8, 119, 128, 57, 144,
		3, 8, 136, 128, 51, 48,
		3, 8, 136, 128, 51, 48,
		0, 11, 0, 186, 0, 0,
		0, 11, 0, 186, 0, 0,
		0, 11, 11, 187, 160, 0,
		0, 12, 12, 187, 160, 0,
		0, 208, 220, 202, 160, 0,
		0, 0, 0, 0, 0, 0
};

uint16_t GUYSKO_L_1[] = {
		0x0, 0x7320, 0xb520, 0xff80, 0xe54f, 0xffff, 0x3983, 0x9ac7, 0xccad, 0x4b7e, 0x29b3, 0x5bd, 0xaedc, 0x8430
};


uint8_t guysko_l_1[] = {
		0, 0, 0, 16, 0, 0,
		0, 34, 32, 32, 32, 0,
		2, 35, 34, 51, 34, 0,
		32, 51, 51, 35, 50, 32,
		2, 48, 51, 51, 51, 50,
		2, 4, 0, 48, 51, 48,
		0, 116, 101, 4, 3, 32,
		0, 116, 101, 68, 2, 0,
		7, 68, 100, 72, 112, 0,
		0, 116, 68, 135, 112, 0,
		0, 151, 119, 121, 144, 0,
		0, 153, 153, 153, 153, 0,
		7, 121, 153, 151, 121, 144,
		116, 73, 153, 116, 73, 144,
		116, 73, 153, 116, 73, 144,
		7, 121, 153, 167, 112, 0,
		0, 10, 170, 170, 0, 0,
		0, 204, 176, 176, 0, 0,
		0, 204, 176, 176, 0, 0,
		0, 204, 203, 11, 0, 0,
		0, 220, 203, 11, 0, 0,
		13, 221, 187, 11, 0, 0,
		0, 0, 0, 0, 0, 0
};

uint16_t GUYSKO_L_2[] = {
		0x0, 0xb520, 0x20, 0xe54f, 0xffff, 0x3983, 0x9ac7, 0xccad, 0x4b7e, 0x29b3, 0x9edd, 0xaedc, 0x1c54, 0x5bd, 0x4228
};


uint8_t guysko_l_2[] = {
		0, 0, 0, 0, 0, 0,
		0, 17, 16, 16, 16, 0,
		1, 17, 17, 17, 17, 0,
		16, 17, 17, 17, 17, 16,
		1, 16, 17, 17, 17, 18,
		1, 3, 0, 16, 17, 16,
		0, 99, 84, 3, 1, 16,
		0, 99, 84, 51, 1, 0,
		6, 51, 83, 55, 96, 0,
		0, 99, 51, 118, 136, 0,
		9, 134, 102, 104, 136, 128,
		9, 136, 136, 136, 136, 128,
		9, 152, 134, 102, 136, 128,
		6, 152, 103, 51, 136, 128,
		3, 152, 103, 51, 136, 0,
		3, 152, 134, 102, 0, 0,
		0, 153, 153, 153, 128, 0,
		0, 10, 171, 160, 0, 0,
		0, 10, 171, 12, 0, 0,
		0, 202, 221, 170, 192, 0,
		10, 204, 208, 14, 170, 0,
		0, 0, 0, 238, 236, 0,
		0, 0, 0, 0, 0, 0
};
