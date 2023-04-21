/*
 * utils.h
 *
 *  Created on: 14 Feb 2023
 *      Author: sebas
 */

#ifndef INC_UTILS_H_
#define INC_UTILS_H_

block_t upper(block_t block);

block_t lower(block_t block);

block_c pixel_to_block(pixel_c pixel_coord);

pixel_c block_to_pixel(block_c block_coord);

block_c cell_x_to_block_left(cell_c cell_coord);

block_c cell_x_to_block_right(cell_c cell_coord);

block_c cell_y_to_block(cell_c cell_coord);

cell_c block_to_cell_x(block_c block_coord);

cell_c block_to_cell_y(block_c block_coord);

cell_c pixel_to_cell_x(pixel_c pixel_coord);

cell_c pixel_to_cell_y(pixel_c pixel_coord);

pixel_c cell_x_to_pixel(cell_c cell_coord);

pixel_c cell_y_to_pixel(cell_c cell_coord);

#endif /* INC_UTILS_H_ */
