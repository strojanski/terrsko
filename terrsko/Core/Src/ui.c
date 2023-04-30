/*
 * ui.c
 *
 *  Created on: 30 Apr 2023
 *      Author: sebas
 */

#include <stdbool.h>

#include "ugui.h"


#define MAX_OBJECTS 10

UG_WINDOW main_menu_window;

void main_menu_callback(UG_MESSAGE* msg) {
	if (msg->type == MSG_TYPE_OBJECT) {
		if (msg->id == OBJ_TYPE_BUTTON) {
			switch (msg->sub_id) {
			case BTN_ID_0:
				// Destroy window, start game
				UG_WindowDelete(&main_menu_window);
//				draw_scene();
				break;

			case BTN_ID_1:
				// Destroy window, turn off
				//UG_WindowDelete(&main_menu_window);

				break;
			}
		}
	}
	return;
}


void main_menu() {
	// Declare objects
	UG_BUTTON btn_play;
	UG_BUTTON btn_exit;
	UG_OBJECT object_buff_main_menu[MAX_OBJECTS];

	// Create window
	char* title = "Terrsko";
	UG_WindowCreate(&main_menu_window, object_buff_main_menu, MAX_OBJECTS, main_menu_callback);

	// Set title
	UG_WindowSetTitleText(&main_menu_window, title);
	UG_WindowSetTitleTextFont(&main_menu_window, &FONT_16X26);

	// Create play and exit button
	UG_ButtonCreate(&main_menu_window, &btn_play, BTN_ID_0, 30, 30, 310, 80);	// Play
	UG_ButtonCreate(&main_menu_window, &btn_exit, BTN_ID_1, 30, 90, 310, 140);	// Exit

	// Configure button 1
	UG_ButtonSetFont(&main_menu_window, BTN_ID_0, &FONT_12X16);
	UG_ButtonSetText(&main_menu_window, BTN_ID_0, "Play");

	// Configure button 2
	UG_ButtonSetFont(&main_menu_window, BTN_ID_1, &FONT_12X16);
	UG_ButtonSetText(&main_menu_window, BTN_ID_1, "Exit");

	// Show window
	UG_WindowShow(&main_menu_window);
}

