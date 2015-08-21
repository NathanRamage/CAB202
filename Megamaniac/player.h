#ifndef _PLAYER_H_
#define _PLAYER_H_

/* Standard libraries */
#include <stdbool.h>
#include <stdlib.h>

/* External libraries */
#include "cab202_graphics.h"
#include "cab202_timers.h"
#include "cab202_sprites.h"

/* script wide variables */
sprite_id player;

/* draws player */
void draw_player( sprite_id player );

/* creates the player sprite and sets it's default values */
void setup_player( void );

/* moves the player's position */
bool update_player( int key );

/* frees memory allocated for the player sprite */
void cleanup_player( void );

#endif
