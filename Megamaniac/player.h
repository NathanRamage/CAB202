#ifndef _PLAYER_H_
#define _PLAYER_H_

/* Standard libraries */
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* External libraries */
#include "cab202_graphics.h"
#include "cab202_timers.h"
#include "cab202_sprites.h"

/* definitions */
#define string char*
#define DEATH_BANNER_SIZE (19)
#define TRUE 't'

/* script wide variables */
sprite_id player;
sprite_id bullet;
bool shot_bullet;

/* draws player */
void draw_player( sprite_id player );

/* creates the player sprite and sets it's default values */
void setup_player( void );

/* moves the player's position */
bool update_player( int key );

/* frees memory allocated for the player sprite */
void cleanup_player( void );

/* reset the players position to the middle of the screen */
void reset_player( void );

/* Draws the banner on the game over screen */
int draw_death_banner ( void );

/* allows the playe to either quit the game or restart the game from the game over screen */
int player_choice( void );

#endif
