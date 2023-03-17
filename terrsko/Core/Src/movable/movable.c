/*
 * movable.c
 *
 *  Created on: 15 Mar 2023
 *      Author: gabrijel
 */




/*
 * kako se med seboj razlikujejo npc-ji (drugače):
 * se premikajo:
 * 	speed
 * 	algoritem
 * zgledajo
 * demage
 * passive / active
 * lajf
 * način napada
 * habitat -- too much!
 * različna pogostost
 * drop items
 *
 * Each NPC looks, moves, attacks, ... different.
 * Ideas of implementation:
 * attack shall be implemented as function (guysko*, npc*) { calculate }
 * All the NPC shall be kept in a "list"
 * Some NPC shall attack guysko in certain vicinity. (they should know how to come close to him?)
 *
 * kaj jim je skupno?
 * helath points
 * vel, move, position
 */

#include "movable.h"

void insert_cow (movable* beeings, cow* krava) {
	krava->prev = beeings->tail_cow->prev;
	krava->next = beeings->tail_cow;

	cow* penultimate_cow = (cow*)malloc(sizeof(cow));
	penultimate_cow = beeings->tail_cow->prev;
	penultimate_cow->next = krava;
	beeings->tail_cow->prev = krava;
	free(penultimate_cow);
}

movable* new_movable() {
	movable* beeings = (movable*)malloc(sizeof(movable));

	cow* h_cow = (cow*)malloc(sizeof(cow));
	cow* t_cow = (cow*)malloc(sizeof(cow));

	h_cow->next = t_cow;
	h_cow->prev = NULL;
	t_cow->next = NULL;
	t_cow->prev = h_cow;

	return beeings;
}

