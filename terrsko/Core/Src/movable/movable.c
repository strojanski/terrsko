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
 * habitat
 * različna pogostost
 * drop items
 *
 * Each NPC looks, moves, attacks, ... different.
 * Ideas of implementation:
 * attack shall be implemented as function (guysko*, npc*) { calculate }
 * All the NPC shall be kept in a "list"
 * Some NPC shall attack guysko in certain vicinity. (they should know how to come close to him?)
 *
 */

